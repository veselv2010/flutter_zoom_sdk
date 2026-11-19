#include "flutter_zoom_sdk_plugin.h"

#include <flutter/event_stream_handler_functions.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>
#include <shobjidl.h>

#include <charconv>
#include <system_error>
#include <sstream>
#include <utility>
#include <vector>

namespace flutter_zoom_sdk {
namespace {
	constexpr UINT kHostMessage = WM_APP + 63;
	constexpr UINT_PTR kTimeoutTimer = 1;
	constexpr ULONGLONG kCommandTimeoutMs = 30000;
	constexpr UINT kTimeoutCheckIntervalMs = 1000;
	constexpr DWORD kHostShutdownTimeoutMs = 1500;
	constexpr size_t kMaxMessageLength = 1024 * 1024;
	constexpr wchar_t kDispatcherClass[] = L"FlutterZoomHostDispatcher";

	std::wstring AppUserModelId() {
		PWSTR value = nullptr;
		if (FAILED(GetCurrentProcessExplicitAppUserModelID(&value))) return {};
		std::wstring id = value ? value : L"";
		CoTaskMemFree(value);
		return id;
	}

	template <typename T>
	bool ParseNumber(const std::string& text, T& value) {
		auto result = std::from_chars(text.data(),
			text.data() + text.size(), value);
		return result.ec == std::errc() &&
			result.ptr == text.data() + text.size();
	}

	std::vector<std::string> Split(const std::string& line) {
		std::vector<std::string> values;
		size_t start = 0;
		while (true) {
			auto end = line.find('\t', start);
			values.emplace_back(line.substr(start, end - start));
			if (end == std::string::npos) return values;
			start = end + 1;
		}
	}
}

// static
void FlutterZoomSdkPlugin::RegisterWithRegistrar(
	flutter::PluginRegistrarWindows* registrar) {
	auto channel = std::make_unique<flutter::MethodChannel<Value>>(
		registrar->messenger(), "flutter_zoom_sdk",
		&flutter::StandardMethodCodec::GetInstance());
	auto event_channel = std::make_unique<flutter::EventChannel<Value>>(
		registrar->messenger(), "flutter_zoom_sdk_event_stream",
		&flutter::StandardMethodCodec::GetInstance());

	auto plugin = std::make_unique<FlutterZoomSdkPlugin>();
	channel->SetMethodCallHandler(
		[pointer = plugin.get()](const auto& call, auto result) {
			pointer->HandleMethodCall(call, std::move(result));
		});
	event_channel->SetStreamHandler(
		std::make_unique<flutter::StreamHandlerFunctions<>>(
			[pointer = plugin.get()](auto, auto events) {
				pointer->Listen(std::move(events));
				return nullptr;
			},
			[pointer = plugin.get()](auto) {
				pointer->meeting_event_sink_.reset();
				return nullptr;
			}));
	registrar->AddPlugin(std::move(plugin));
}

FlutterZoomSdkPlugin::FlutterZoomSdkPlugin() {
	app_user_model_id_ = AppUserModelId();
	WNDCLASSW wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandleW(nullptr);
	wc.lpszClassName = kDispatcherClass;
	RegisterClassW(&wc);
	dispatcher_ = CreateWindowExW(0, kDispatcherClass, L"", 0,
		0, 0, 0, 0, HWND_MESSAGE, nullptr, wc.hInstance, nullptr);
	if (dispatcher_) {
		SetWindowLongPtrW(dispatcher_, GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(this));
	}
}

FlutterZoomSdkPlugin::~FlutterZoomSdkPlugin() {
	StopHost();
	if (dispatcher_) {
		KillTimer(dispatcher_, kTimeoutTimer);
		DestroyWindow(dispatcher_);
	}
}

LRESULT CALLBACK FlutterZoomSdkPlugin::WindowProc(HWND window,
	UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_TIMER && wparam == kTimeoutTimer) {
		auto* plugin = reinterpret_cast<FlutterZoomSdkPlugin*>(
			GetWindowLongPtrW(window, GWLP_USERDATA));
		if (plugin) plugin->CheckTimeouts();
		return 0;
	}
	if (message == kHostMessage) {
		auto* plugin = reinterpret_cast<FlutterZoomSdkPlugin*>(
			GetWindowLongPtrW(window, GWLP_USERDATA));
		if (plugin) plugin->HandleMessages();
		return 0;
	}
	return DefWindowProcW(window, message, wparam, lparam);
}

std::string FlutterZoomSdkPlugin::GetString(const Map& values,
	const char* key) {
	auto iter = values.find(Value(key));
	if (iter == values.end()) return {};
	auto* text = std::get_if<std::string>(&iter->second);
	return text ? *text : std::string();
}

std::string FlutterZoomSdkPlugin::Hex(const std::string& input) {
	constexpr char alphabet[] = "0123456789ABCDEF";
	std::string result;
	result.reserve(input.size() * 2);
	for (unsigned char ch : input) {
		result.push_back(alphabet[ch >> 4]);
		result.push_back(alphabet[ch & 15]);
	}
	return result;
}

void FlutterZoomSdkPlugin::HandleMethodCall(
	const flutter::MethodCall<Value>& call,
	std::unique_ptr<Result> result) {
	if (call.method_name() == "getPlatformVersion") {
		std::ostringstream version;
		version << "Windows ";
		if (IsWindows10OrGreater()) {
			version << "10+";
		} else if (IsWindows8OrGreater()) {
			version << "8";
		} else if (IsWindows7OrGreater()) {
			version << "7";
		}
		result->Success(Value(version.str()));
		return;
	}

	if (call.method_name() == "init_and_join") {
		const auto* args = call.arguments()
			? std::get_if<Map>(call.arguments()) : nullptr;
		if (!args) {
			result->Success(Value(false));
			return;
		}
		auto init_it = args->find(Value("initOptions"));
		auto meeting_it = args->find(Value("meetingOptions"));
		if (init_it == args->end() || meeting_it == args->end()) {
			result->Success(Value(false));
			return;
		}
		const auto* init = std::get_if<Map>(&init_it->second);
		const auto* meeting = std::get_if<Map>(&meeting_it->second);
		if (!init || !meeting) {
			result->Success(Value(false));
			return;
		}
		const auto jwt = GetString(*init, "jwtToken");
		const auto meeting_id = GetString(*meeting, "meetingId");
		if (jwt.empty() || meeting_id.empty()) {
			result->Success(Value(false));
			return;
		}

		StopHost();
		last_status_.clear();
		if (!StartHost()) {
			result->Success(Value(false));
			return;
		}
		std::string command = "I";
		for (const auto& value : {
			jwt,
			GetString(*init, "domain"),
			GetString(*init, "language"),
			meeting_id,
			GetString(*meeting, "meetingPassword"),
			GetString(*meeting, "displayName"),
			GetString(*meeting, "noAudio"),
			GetString(*meeting, "noVideo")}) {
			command += "\t" + Hex(value);
		}
		SendCommand(command, std::move(result));
		return;
	}

	std::string command;
	if (call.method_name() == "leave_meeting") {
		command = "L";
	} else if (call.method_name() == "show_meeting") {
		command = "S";
	} else if (call.method_name() == "hide_meeting") {
		command = "H";
	} else if (call.method_name() == "show_mini_video") {
		command = "V";
	} else {
		result->NotImplemented();
		return;
	}
	SendCommand(command, std::move(result));
}

void FlutterZoomSdkPlugin::Listen(
	std::unique_ptr<flutter::EventSink<>>&& events) {
	meeting_event_sink_ = std::move(events);
	if (meeting_event_sink_ &&
		last_status_ == "MEETING_STATUS_INMEETING") {
		NotifyStatus(last_status_, last_status_code_);
	}
}

void FlutterZoomSdkPlugin::NotifyStatus(const std::string& status, int code) {
	last_status_ = status;
	last_status_code_ = code;
	if (meeting_event_sink_) {
		flutter::EncodableList data;
		data.emplace_back(status);
		data.emplace_back(code);
		meeting_event_sink_->Success(data);
	}
}

void FlutterZoomSdkPlugin::FailPending() {
	for (auto& entry : pending_) entry.second.result->Success(Value(false));
	pending_.clear();
	if (dispatcher_) KillTimer(dispatcher_, kTimeoutTimer);
}

void FlutterZoomSdkPlugin::CheckTimeouts() {
	auto now = GetTickCount64();
	for (auto it = pending_.begin(); it != pending_.end();) {
		if (it->second.deadline <= now) {
			it->second.result->Success(Value(false));
			it = pending_.erase(it);
		} else {
			++it;
		}
	}
	if (pending_.empty() && dispatcher_) {
		KillTimer(dispatcher_, kTimeoutTimer);
	}
}

void FlutterZoomSdkPlugin::HandleMessages() {
	std::deque<std::string> entries;
	{
		std::lock_guard<std::mutex> lock(messages_mutex_);
		entries.swap(messages_);
	}
	for (const auto& entry : entries) HandleMessage(entry);
}

void FlutterZoomSdkPlugin::HandleMessage(const std::string& message) {
	auto parts = Split(message);
	if (parts.empty()) return;
	if (parts[0] == "R" && parts.size() == 3) {
		uint64_t id = 0;
		if (!ParseNumber(parts[1], id)) return;
		auto iter = pending_.find(id);
		if (iter == pending_.end()) return;
		iter->second.result->Success(Value(parts[2] == "1"));
		pending_.erase(iter);
		if (pending_.empty() && dispatcher_) {
			KillTimer(dispatcher_, kTimeoutTimer);
		}
	} else if (parts[0] == "E" && parts.size() == 3) {
		int code = 0;
		if (ParseNumber(parts[2], code)) NotifyStatus(parts[1], code);
	} else if (parts[0] == "D" && !stopping_) {
		FailPending();
		if (last_status_ != "MEETING_STATUS_ENDED" &&
			last_status_ != "MEETING_STATUS_DISCONNECTING") {
			NotifyStatus("MEETING_STATUS_FAILED", -1);
		}
	}
}

void FlutterZoomSdkPlugin::ReadHostOutput() {
	char buffer[4096];
	DWORD bytes = 0;
	std::string line;
	while (ReadFile(output_, buffer, sizeof(buffer), &bytes, nullptr) && bytes) {
		for (DWORD i = 0; i < bytes; ++i) {
			if (buffer[i] == '\n') {
				{
					std::lock_guard<std::mutex> lock(messages_mutex_);
					messages_.push_back(std::move(line));
				}
				PostMessageW(dispatcher_, kHostMessage, 0, 0);
				line.clear();
			} else if (line.size() < kMaxMessageLength) {
				line.push_back(buffer[i]);
			}
		}
	}
	{
		std::lock_guard<std::mutex> lock(messages_mutex_);
		messages_.emplace_back("D");
	}
	PostMessageW(dispatcher_, kHostMessage, 0, 0);
}

bool FlutterZoomSdkPlugin::StartHost() {
	wchar_t path[32768];
	DWORD length = GetModuleFileNameW(nullptr, path, ARRAYSIZE(path));
	if (!length || length == ARRAYSIZE(path) || !dispatcher_) return false;
	std::wstring directory(path, length);
	directory.erase(directory.find_last_of(L"\\/"));
	std::wstring host_directory = directory + L"\\zoom_host";
	std::wstring executable = host_directory + L"\\ZoomHost.exe";

	SECURITY_ATTRIBUTES attributes = {sizeof(attributes), nullptr, TRUE};
	HANDLE child_input = nullptr;
	HANDLE child_output = nullptr;
	if (!CreatePipe(&child_input, &input_, &attributes, 0)) return false;
	if (!SetHandleInformation(input_, HANDLE_FLAG_INHERIT, 0)) {
		CloseHandle(child_input);
		StopHost();
		return false;
	}
	if (!CreatePipe(&output_, &child_output, &attributes, 0)) {
		CloseHandle(child_input);
		StopHost();
		return false;
	}
	if (!SetHandleInformation(output_, HANDLE_FLAG_INHERIT, 0)) {
		CloseHandle(child_input);
		CloseHandle(child_output);
		StopHost();
		return false;
	}

	STARTUPINFOW startup = {};
	startup.cb = sizeof(startup);
	startup.dwFlags = STARTF_USESTDHANDLES;
	startup.hStdInput = child_input;
	startup.hStdOutput = child_output;
	HANDLE errors = CreateFileW(L"NUL", GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, &attributes, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);
	startup.hStdError = errors != INVALID_HANDLE_VALUE ? errors : child_output;
	PROCESS_INFORMATION process_info = {};
	std::wstring command_line = L"\"" + executable + L"\"";
	if (!app_user_model_id_.empty()) {
		command_line += L" \"" + app_user_model_id_ + L"\"";
	}
	BOOL created = CreateProcessW(executable.c_str(), command_line.data(),
		nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr,
		host_directory.c_str(), &startup, &process_info);
	CloseHandle(child_input);
	CloseHandle(child_output);
	if (errors != INVALID_HANDLE_VALUE) CloseHandle(errors);
	if (!created) {
		StopHost();
		return false;
	}
	process_ = process_info.hProcess;
	CloseHandle(process_info.hThread);
	stopping_ = false;
	reader_ = std::thread([this] { ReadHostOutput(); });
	return true;
}

bool FlutterZoomSdkPlugin::WriteCommand(const std::string& command) {
	if (!input_) return false;
	std::string line = command + "\n";
	size_t offset = 0;
	while (offset < line.size()) {
		DWORD written = 0;
		if (!WriteFile(input_, line.data() + offset,
			static_cast<DWORD>(line.size() - offset), &written, nullptr) ||
			!written) return false;
		offset += written;
	}
	return true;
}

void FlutterZoomSdkPlugin::SendCommand(const std::string& command,
	std::unique_ptr<Result> result) {
	if (!process_ || WaitForSingleObject(process_, 0) != WAIT_TIMEOUT) {
		result->Success(Value(false));
		return;
	}
	auto id = ++next_id_;
	std::string line = command.substr(0, 1) + "\t" +
		std::to_string(id) + command.substr(1);
	if (!WriteCommand(line)) {
		result->Success(Value(false));
		return;
	}
	pending_.emplace(id, PendingCall{std::move(result), GetTickCount64() + 30000});
	SetTimer(dispatcher_, kTimeoutTimer, 1000, nullptr);
}

void FlutterZoomSdkPlugin::StopHost() {
	stopping_ = true;
	if (input_) {
		WriteCommand("Q\t0");
		CloseHandle(input_);
		input_ = nullptr;
	}
	if (process_) {
		if (WaitForSingleObject(process_, kHostShutdownTimeoutMs) == WAIT_TIMEOUT) {
			TerminateProcess(process_, 1);
			WaitForSingleObject(process_, 1000);
		}
	}
	if (reader_.joinable()) {
		CancelSynchronousIo(reader_.native_handle());
		reader_.join();
	}
	if (output_) {
		CloseHandle(output_);
		output_ = nullptr;
	}
	if (process_) {
		CloseHandle(process_);
		process_ = nullptr;
	}
	{
		std::lock_guard<std::mutex> lock(messages_mutex_);
		messages_.clear();
	}
	FailPending();
}

}  // namespace flutter_zoom_sdk
