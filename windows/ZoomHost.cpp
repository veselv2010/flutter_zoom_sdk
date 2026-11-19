#include <windows.h>
#include <shellapi.h>
#include <shobjidl.h>
#include <propkey.h>
#include <propvarutil.h>

#include <zoom_sdk.h>
#include <auth_service_interface.h>
#include <meeting_service_interface.h>
#include <setting_service_interface.h>
#include <meeting_service_components/meeting_configuration_interface.h>
#include <meeting_service_components/meeting_ui_ctrl_interface.h>
#include <meeting_service_components/meeting_audio_interface.h>
#include <meeting_service_components/meeting_video_interface.h>
#include <meeting_service_components/meeting_sharing_interface.h>
#include <meeting_service_components/meeting_participants_ctrl_interface.h>

#include <atomic>
#include <charconv>
#include <system_error>
#include <cstdint>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace zoom = ZOOMSDK;
namespace {
	constexpr UINT kCommandMessage = WM_APP + 64;
	constexpr UINT kJoinMessage = WM_APP + 65;
	constexpr UINT kGroupMessage = WM_APP + 66;
	constexpr UINT kShareMessage = WM_APP + 67;
	constexpr UINT_PTR kGroupTimer = 1;
	constexpr UINT_PTR kShareTimer = 2;
	constexpr UINT kWindowGroupIntervalMs = 1000;
	constexpr UINT kShareRefreshDelayMs = 350;
	constexpr size_t kMaxMessageLength = 1024 * 1024;
	constexpr wchar_t kWindowClass[] = L"FlutterZoomHostCommands";

	void SetWindowAppId(HWND window, const std::wstring& id) {
		if (!window || id.empty()) return;
		IPropertyStore* store = nullptr;
		if (FAILED(SHGetPropertyStoreForWindow(window,
			IID_PPV_ARGS(&store)))) return;
		PROPVARIANT current;
		PropVariantInit(&current);
		bool matched = SUCCEEDED(store->GetValue(PKEY_AppUserModel_ID,
			&current)) && current.vt == VT_LPWSTR && current.pwszVal &&
			id == current.pwszVal;
		PropVariantClear(&current);
		if (!matched) {
			PROPVARIANT value;
			if (SUCCEEDED(InitPropVariantFromString(id.c_str(), &value))) {
				store->SetValue(PKEY_AppUserModel_ID, value);
				PropVariantClear(&value);
			}
		}
		store->Release();
	}

	std::vector<std::string> Split(const std::string& line) {
		std::vector<std::string> parts;
		size_t start = 0;
		while (true) {
			size_t end = line.find('\t', start);
			parts.emplace_back(line.substr(start, end - start));
			if (end == std::string::npos) return parts;
			start = end + 1;
		}
	}

	int HexValue(char ch) {
		if (ch >= '0' && ch <= '9') return ch - '0';
		if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
		if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
		return -1;
	}

	bool DecodeHex(const std::string& input, std::string& output) {
		if (input.size() % 2) return false;
		output.clear();
		output.reserve(input.size() / 2);
		for (size_t i = 0; i < input.size(); i += 2) {
			int high = HexValue(input[i]);
			int low = HexValue(input[i + 1]);
			if (high < 0 || low < 0) return false;
			output.push_back(static_cast<char>((high << 4) | low));
		}
		return true;
	}

	std::wstring Wide(const std::string& utf8) {
		if (utf8.empty()) return {};
		int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
			utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
		if (!length) return {};
		std::wstring result(length, L'\0');
		if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
			utf8.data(), static_cast<int>(utf8.size()),
			result.data(), length) != length) return {};
		return result;
	}

	zoom::SDK_LANGUAGE_ID Language(const std::string& language) {
		if (language.size() >= 2 &&
			(language[0] == 'r' || language[0] == 'R') &&
			(language[1] == 'u' || language[1] == 'U')) {
			return zoom::LANGUAGE_Russian;
		}
		if (language.size() >= 2 &&
			(language[0] == 'e' || language[0] == 'E') &&
			(language[1] == 'n' || language[1] == 'N')) {
			return zoom::LANGUAGE_English;
		}
		return zoom::LANGUAGE_Unknown;
	}
}

class ZoomHost;

class AuthEvents : public zoom::IAuthServiceEvent {
public:
	explicit AuthEvents(ZoomHost& host) : host_(host) {}
	void onAuthenticationReturn(zoom::AuthResult result) override;
	void onLoginReturnWithReason(zoom::LOGINSTATUS,
		zoom::IAccountInfo*, zoom::LoginFailReason) override {}
	void onLogout() override {}
	void onZoomIdentityExpired() override {}
	void onZoomAuthIdentityExpired() override {}
	void onNotificationServiceStatus(zoom::SDKNotificationServiceStatus,
		zoom::SDKNotificationServiceError) override {}
private:
	ZoomHost& host_;
};

class MeetingEvents : public zoom::IMeetingServiceEvent {
public:
	explicit MeetingEvents(ZoomHost& host) : host_(host) {}
	void onMeetingStatusChanged(zoom::MeetingStatus, int code = 0) override;
	void onMeetingStatisticsWarningNotification(
		zoom::StatisticsWarningType) override {}
	void onMeetingParameterNotification(
		const zoom::MeetingParameter*) override {}
	void onSuspendParticipantsActivities() override {}
	void onAICompanionActiveChangeNotice(bool) override {}
	void onMeetingTopicChanged(const zchar_t*) override {}
	void onMeetingFullToWatchLiveStream(const zchar_t*) override {}
	void onUserNetworkStatusChanged(zoom::MeetingComponentType,
		zoom::ConnectionQuality, unsigned int, bool) override {}
	void onAppSignalPanelUpdated(zoom::IMeetingAppSignalHandler*) override {}
private:
	ZoomHost& host_;
};

class ShareEvents : public zoom::IMeetingShareCtrlEvent {
public:
	explicit ShareEvents(ZoomHost& host) : host_(host) {}
	void onSharingStatus(zoom::ZoomSDKSharingSourceInfo info) override;
	void onFailedToStartShare() override {}
	void onShareContentNotification(zoom::ZoomSDKSharingSourceInfo info) override;
	void onMultiShareSwitchToSingleShareNeedConfirm(
		zoom::IShareSwitchMultiToSingleConfirmHandler*) override {}
	void onShareSettingTypeChangedNotification(zoom::ShareSettingType) override {}
	void onSharedVideoEnded() override {}
	void onVideoFileSharePlayError(zoom::ZoomSDKVideoFileSharePlayError) override {}
	void onOptimizingShareForVideoClipStatusChanged(
		zoom::ZoomSDKSharingSourceInfo) override {}
	void onVideoSharingQualityChanged(zoom::ZoomSDKVideoSharingQuality) override {}
private:
	ZoomHost& host_;
};

class ZoomHost {
public:
	ZoomHost(HWND dispatcher, HANDLE input, HANDLE output,
		std::wstring app_id)
		: window_(dispatcher), input_(input), output_(output),
		  app_user_model_id_(std::move(app_id)),
		  auth_events_(*this), meeting_events_(*this), share_events_(*this) {}

	void GroupWindows() {
		if (app_user_model_id_.empty()) return;
		EnumWindows([](HWND hwnd, LPARAM context) -> BOOL {
			DWORD process_id = 0;
			GetWindowThreadProcessId(hwnd, &process_id);
			if (process_id != GetCurrentProcessId() || !IsWindowVisible(hwnd)) {
				return TRUE;
			}
			if (GetWindow(hwnd, GW_OWNER) &&
				!(GetWindowLongPtrW(hwnd, GWL_EXSTYLE) & WS_EX_APPWINDOW)) {
				return TRUE;
			}
			auto* host = reinterpret_cast<ZoomHost*>(context);
			SetWindowAppId(hwnd, host->app_user_model_id_);
			return TRUE;
		}, reinterpret_cast<LPARAM>(this));
	}

	void StopReading() { stopping_ = true; }

	void ReadCommands() {
		char buffer[4096];
		DWORD bytes = 0;
		std::string line;
		while (!stopping_ &&
			ReadFile(input_, buffer, sizeof(buffer), &bytes, nullptr) && bytes) {
			for (DWORD i = 0; i < bytes; ++i) {
				if (buffer[i] == '\n') {
					{
						std::lock_guard<std::mutex> lock(commands_mutex_);
						commands_.push_back(std::move(line));
					}
					PostMessageW(window_, kCommandMessage, 0, 0);
					line.clear();
				} else if (line.size() < kMaxMessageLength) {
					line.push_back(buffer[i]);
				}
			}
		}
		if (!stopping_) PostMessageW(window_, WM_CLOSE, 0, 0);
	}

	void ProcessCommands() {
		std::deque<std::string> commands;
		{
			std::lock_guard<std::mutex> lock(commands_mutex_);
			commands.swap(commands_);
		}
		for (const auto& command : commands) {
			auto parts = Split(command);
			if (parts.size() < 2) continue;
			const std::string& id = parts[1];
			if (parts[0] == "Q") {
				PostMessageW(window_, WM_CLOSE, 0, 0);
				return;
			}
			bool success = false;
			if (parts[0] == "I" && parts.size() == 10) {
				success = InitAndJoin(parts);
			} else if (parts[0] == "L" && parts.size() == 2) {
				success = Leave();
			} else if (parts[0] == "S" && parts.size() == 2) {
				success = Show();
			} else if (parts[0] == "H" && parts.size() == 2) {
				success = Hide();
			} else if (parts[0] == "V" && parts.size() == 2) {
				success = ShowMiniVideo();
			}
			Send("R\t" + id + (success ? "\t1\n" : "\t0\n"));
		}
	}

	void OnAuthentication(zoom::AuthResult result) {
		if (result == zoom::AUTHRET_SUCCESS) {
			PostMessageW(window_, kJoinMessage, 0, 0);
		} else {
			Status("MEETING_STATUS_FAILED", static_cast<int>(result));
		}
	}

	void JoinAfterAuthentication() {
		Join();
	}

	void OnMeetingStatus(zoom::MeetingStatus status, int code) {
		const char* name = nullptr;
		switch (status) {
			case zoom::MEETING_STATUS_INMEETING:
				name = "MEETING_STATUS_INMEETING";
				break;
			case zoom::MEETING_STATUS_DISCONNECTING:
				name = "MEETING_STATUS_DISCONNECTING";
				break;
			case zoom::MEETING_STATUS_FAILED:
				name = "MEETING_STATUS_FAILED";
				break;
			case zoom::MEETING_STATUS_ENDED:
				name = "MEETING_STATUS_ENDED";
				break;
			default:
				return;
		}
		PostMessageW(window_, kGroupMessage,
			status == zoom::MEETING_STATUS_INMEETING, 0);
		Status(name, code);
	}

	void Shutdown() {
		if (meeting_) {
			if (auto* share = meeting_->GetMeetingShareController()) {
				share->SetEvent(nullptr);
			}
			meeting_->SetEvent(nullptr);
			zoom::DestroyMeetingService(meeting_);
			meeting_ = nullptr;
		}
		if (setting_) {
			zoom::DestroySettingService(setting_);
			setting_ = nullptr;
		}
		if (auth_) {
			auth_->SetEvent(nullptr);
			zoom::DestroyAuthService(auth_);
			auth_ = nullptr;
		}
		if (initialized_) {
			zoom::CleanUPSDK();
			initialized_ = false;
		}
		pinned_host_ = 0;
		minimized_ = false;
		KillTimer(window_, kGroupTimer);
		KillTimer(window_, kShareTimer);
	}

	void OnSharing(zoom::SharingStatus status) {
		if (status == zoom::Sharing_Other_Share_Begin ||
			status == zoom::Sharing_Other_Share_End) {
			PostMessageW(window_, kShareMessage, 0, 0);
		}
	}

	void ScheduleShareRefresh() {
		if (minimized_) {
			SetTimer(window_, kShareTimer, kShareRefreshDelayMs, nullptr);
		}
	}

	void RefreshMinimizedWindow() {
		KillTimer(window_, kShareTimer);
		if (!minimized_) return;
		zoom::IMeetingUIController* ui = nullptr;
		HWND window = nullptr;
		if (!GetMeetingWindow(ui, window)) return;
		ShowWindow(window, SW_SHOWNOACTIVATE);
		PostMessageW(window, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}

private:
	void Send(const std::string& text) {
		std::lock_guard<std::mutex> lock(output_mutex_);
		size_t offset = 0;
		while (offset < text.size()) {
			DWORD written = 0;
			if (!WriteFile(output_, text.data() + offset,
				static_cast<DWORD>(text.size() - offset), &written, nullptr) ||
				!written) return;
			offset += written;
		}
	}

	void Status(const char* status, int code) {
		Send(std::string("E\t") + status + "\t" +
			std::to_string(code) + "\n");
	}

	bool InitAndJoin(const std::vector<std::string>& parts) {
		std::vector<std::string> values(8);
		for (size_t i = 0; i < values.size(); ++i) {
			if (!DecodeHex(parts[i + 2], values[i])) return false;
		}
		if (values[0].empty() || values[3].empty()) return false;

		Shutdown();
		jwt_ = Wide(values[0]);
		domain_ = Wide(values[1]);
		meeting_id_ = values[3];
		password_ = Wide(values[4]);
		name_ = Wide(values[5]);
		no_audio_ = values[6] == "true";
		no_video_ = values[7] == "true";
		if (domain_.empty()) domain_ = L"zoom.us";

		zoom::InitParam options;
		options.strWebDomain = domain_.c_str();
		options.emLanguageID = Language(values[2]);
		if (zoom::InitSDK(options) != zoom::SDKERR_SUCCESS) return false;
		initialized_ = true;
		if (zoom::CreateAuthService(&auth_) != zoom::SDKERR_SUCCESS || !auth_) {
			Shutdown();
			return false;
		}
		auth_->SetEvent(&auth_events_);
		zoom::AuthContext context;
		context.jwt_token = jwt_.c_str();
		if (auth_->SDKAuth(context) != zoom::SDKERR_SUCCESS) {
			Shutdown();
			return false;
		}
		return true;
	}

	void Join() {
		if (!initialized_ || meeting_id_.empty()) return;
		if (zoom::CreateMeetingService(&meeting_) != zoom::SDKERR_SUCCESS ||
			!meeting_) {
			Status("MEETING_STATUS_FAILED", -1);
			return;
		}
		if (meeting_->SetEvent(&meeting_events_) != zoom::SDKERR_SUCCESS) {
			Status("MEETING_STATUS_FAILED", -1);
			return;
		}
		if (auto* share = meeting_->GetMeetingShareController()) {
			share->SetEvent(&share_events_);
		}
		if (zoom::CreateSettingService(&setting_) == zoom::SDKERR_SUCCESS &&
			setting_) {
			if (auto* audio = setting_->GetAudioSettings()) {
				audio->EnableAutoJoinAudio(true);
			}
			if (auto* share = setting_->GetShareSettings()) {
				share->EnableAutoFitToWindowWhenViewSharing(false);
			}
		}
		if (auto* config = meeting_->GetMeetingConfiguration()) {
			config->HideMeetingInfoOnMeetingUI(true);
			config->EnableInviteButtonOnMeetingUI(false);
			config->EnableInviteLinkOnMeetingUI(false);
			config->HideRecordItemOnMeetingUI(true);
			config->HideUpgradeFreeMeetingButton(true);
		}

		zoom::JoinParam param;
		zoom::JoinParam4WithoutLogin guest{};
		const char* begin = meeting_id_.data();
		const char* end = begin + meeting_id_.size();
		auto parsed = std::from_chars(begin, end, guest.meetingNumber);
		if (parsed.ec != std::errc() || parsed.ptr != end) {
			Status("MEETING_STATUS_FAILED", -1);
			return;
		}
		guest.userName = name_.c_str();
		guest.psw = password_.c_str();
		guest.isAudioOff = no_audio_;
		guest.isVideoOff = no_video_;
		param.userType = zoom::SDK_UT_WITHOUT_LOGIN;
		param.param.withoutloginuserJoin = guest;
		auto error = meeting_->Join(param);
		if (error != zoom::SDKERR_SUCCESS) {
			Status("MEETING_STATUS_FAILED", static_cast<int>(error));
		}
	}

	bool Leave() {
		return meeting_ &&
			meeting_->Leave(zoom::LEAVE_MEETING) == zoom::SDKERR_SUCCESS;
	}

	bool GetMeetingWindow(zoom::IMeetingUIController*& ui, HWND& window) {
		if (!meeting_) return false;
		ui = meeting_->GetUIController();
		if (!ui) return false;
		HWND second = nullptr;
		return ui->GetMeetingUIWnd(window, second) == zoom::SDKERR_SUCCESS &&
			window != nullptr;
	}

	bool Show() {
		zoom::IMeetingUIController* ui = nullptr;
		HWND window = nullptr;
		if (!GetMeetingWindow(ui, window)) return false;
		minimized_ = false;
		KillTimer(window_, kShareTimer);
		UnpinHost();
		ShowWindow(window, SW_SHOWMAXIMIZED);
		return ui->BackToMeeting() == zoom::SDKERR_SUCCESS;
	}

	bool Hide() {
		zoom::IMeetingUIController* ui = nullptr;
		HWND window = nullptr;
		if (!GetMeetingWindow(ui, window)) return false;
		PinHost();
		ui->ExitFullScreen(true, true);
		minimized_ = PostMessageW(window, WM_SYSCOMMAND, SC_MINIMIZE, 0) != 0;
		return minimized_;
	}

	bool ShowMiniVideo() {
		if (!meeting_) return false;
		auto* ui = meeting_->GetUIController();
		if (!ui) return false;
		zoom::SDKMinimizeUIMode mode;
		if (!ui->IsMinimizeModeOfFirstScreenMeetingUIWnd(mode)) return false;
		PinHost();
		if (mode == zoom::MinimizeUIMode_VIDEO) return true;
		if (ui->SwitchMinimizeUIMode4FirstScreenMeetingUIWnd(
			zoom::MinimizeUIMode_VIDEO) != zoom::SDKERR_SUCCESS) return false;
		return ui->IsMinimizeModeOfFirstScreenMeetingUIWnd(mode) &&
			mode == zoom::MinimizeUIMode_VIDEO;
	}

	void PinHost() {
		if (!meeting_ || pinned_host_) return;
		auto* participants = meeting_->GetMeetingParticipantsController();
		auto* video = meeting_->GetMeetingVideoController();
		if (!participants || !video) return;
		auto* users = participants->GetParticipantsList();
		if (!users) return;
		for (int index = 0; index < users->GetCount(); ++index) {
			auto id = users->GetItem(index);
			auto* user = participants->GetUserByUserID(id);
			if (!user || !user->IsHost() || user->IsMySelf()) continue;
			auto* pinned = video->GetPinnedUserListFromFirstView();
			if (pinned) {
				for (int j = 0; j < pinned->GetCount(); ++j) {
					if (pinned->GetItem(j) == id) return;
				}
			}
			if (video->PinVideoToFirstView(id) == zoom::SDKERR_SUCCESS) {
				pinned_host_ = id;
			}
			return;
		}
	}

	void UnpinHost() {
		if (!pinned_host_) return;
		if (meeting_) {
			if (auto* video = meeting_->GetMeetingVideoController()) {
				video->UnPinVideoFromFirstView(pinned_host_);
			}
		}
		pinned_host_ = 0;
	}

	HWND window_;
	HANDLE input_;
	HANDLE output_;
	std::wstring app_user_model_id_;
	std::mutex commands_mutex_;
	std::mutex output_mutex_;
	std::deque<std::string> commands_;
	bool initialized_ = false;
	zoom::IAuthService* auth_ = nullptr;
	zoom::IMeetingService* meeting_ = nullptr;
	zoom::ISettingService* setting_ = nullptr;
	AuthEvents auth_events_;
	MeetingEvents meeting_events_;
	ShareEvents share_events_;
	std::wstring jwt_;
	std::wstring domain_;
	std::wstring name_;
	std::wstring password_;
	std::string meeting_id_;
	bool no_audio_ = false;
	bool no_video_ = false;
	bool minimized_ = false;
	std::atomic<bool> stopping_{false};
	unsigned int pinned_host_ = 0;
};

void AuthEvents::onAuthenticationReturn(zoom::AuthResult result) {
	host_.OnAuthentication(result);
}

void MeetingEvents::onMeetingStatusChanged(zoom::MeetingStatus status, int code) {
	host_.OnMeetingStatus(status, code);
}

void ShareEvents::onSharingStatus(zoom::ZoomSDKSharingSourceInfo info) {
	host_.OnSharing(info.status);
}

void ShareEvents::onShareContentNotification(zoom::ZoomSDKSharingSourceInfo info) {
	host_.OnSharing(info.status);
}

LRESULT CALLBACK HostWindowProc(HWND window, UINT message,
	WPARAM wparam, LPARAM lparam) {
	auto* host = reinterpret_cast<ZoomHost*>(
		GetWindowLongPtrW(window, GWLP_USERDATA));
	if (message == kCommandMessage && host) {
		host->ProcessCommands();
		return 0;
	}
	if (message == kJoinMessage && host) {
		host->JoinAfterAuthentication();
		return 0;
	}
	if (message == kGroupMessage && host) {
		if (wparam) SetTimer(window, kGroupTimer, kWindowGroupIntervalMs, nullptr);
		else KillTimer(window, kGroupTimer);
		host->GroupWindows();
		return 0;
	}
	if (message == WM_TIMER && wparam == kGroupTimer && host) {
		host->GroupWindows();
		return 0;
	}
	if (message == kShareMessage && host) {
		host->ScheduleShareRefresh();
		return 0;
	}
	if (message == WM_TIMER && wparam == kShareTimer && host) {
		host->RefreshMinimizedWindow();
		return 0;
	}
	if (message == WM_CLOSE) {
		DestroyWindow(window);
		return 0;
	}
	if (message == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(window, message, wparam, lparam);
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int) {
	int argument_count = 0;
	LPWSTR* arguments = CommandLineToArgvW(GetCommandLineW(), &argument_count);
	std::wstring app_id;
	if (arguments && argument_count > 1) {
		app_id = arguments[1];
		SetCurrentProcessExplicitAppUserModelID(app_id.c_str());
	}
	if (arguments) LocalFree(arguments);

	HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!input || input == INVALID_HANDLE_VALUE ||
		!output || output == INVALID_HANDLE_VALUE) return 1;

	SetHandleInformation(input, HANDLE_FLAG_INHERIT, 0);
	SetHandleInformation(output, HANDLE_FLAG_INHERIT, 0);

	WNDCLASSW wc = {};
	wc.lpfnWndProc = HostWindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = kWindowClass;
	if (!RegisterClassW(&wc)) return 1;
	HWND window = CreateWindowExW(0, kWindowClass, L"", 0,
		0, 0, 0, 0, HWND_MESSAGE, nullptr, instance, nullptr);
	if (!window) return 1;

	HRESULT com_result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	ZoomHost host(window, input, output, std::move(app_id));
	SetWindowLongPtrW(window, GWLP_USERDATA,
		reinterpret_cast<LONG_PTR>(&host));
	std::thread reader([&host] { host.ReadCommands(); });
	MSG message = {};
	while (GetMessageW(&message, nullptr, 0, 0) > 0) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
	host.Shutdown();
	host.StopReading();
	CancelSynchronousIo(reader.native_handle());
	reader.join();
	if (SUCCEEDED(com_result)) CoUninitialize();
	return 0;
}
