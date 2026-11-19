#ifndef FLUTTER_PLUGIN_FLUTTER_ZOOM_SDK_PLUGIN_H_
#define FLUTTER_PLUGIN_FLUTTER_ZOOM_SDK_PLUGIN_H_

// This must be included before many other Windows headers.
#include <windows.h>

#include <flutter/event_channel.h>
#include <flutter/event_sink.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <cstdint>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace flutter_zoom_sdk {

class FlutterZoomSdkPlugin : public flutter::Plugin {
public:
	static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

	FlutterZoomSdkPlugin();
	~FlutterZoomSdkPlugin() override;

	// Disallow copy and assign.
	FlutterZoomSdkPlugin(const FlutterZoomSdkPlugin&) = delete;
	FlutterZoomSdkPlugin& operator=(const FlutterZoomSdkPlugin&) = delete;

private:
	using Result = flutter::MethodResult<flutter::EncodableValue>;
	using Value = flutter::EncodableValue;
	using Map = flutter::EncodableMap;

	// Called when a method is called on this plugin's channel from Dart.
	void HandleMethodCall(const flutter::MethodCall<Value>& call,
		std::unique_ptr<Result> result);
	void Listen(std::unique_ptr<flutter::EventSink<>>&& events);
	void HandleMessages();
	void HandleMessage(const std::string& message);
	void NotifyStatus(const std::string& status, int code);
	void FailPending();
	void CheckTimeouts();

	bool StartHost();
	void StopHost();
	bool WriteCommand(const std::string& command);
	void SendCommand(const std::string& command,
		std::unique_ptr<Result> result);
	void ReadHostOutput();

	static LRESULT CALLBACK WindowProc(HWND window, UINT message,
		WPARAM wparam, LPARAM lparam);
	static std::string GetString(const Map& values, const char* key);
	static std::string Hex(const std::string& input);

	std::wstring app_user_model_id_;
	HWND dispatcher_ = nullptr;
	HANDLE process_ = nullptr;
	HANDLE input_ = nullptr;
	HANDLE output_ = nullptr;
	std::thread reader_;
	std::mutex messages_mutex_;
	std::deque<std::string> messages_;
	std::unique_ptr<flutter::EventSink<>> meeting_event_sink_;
	struct PendingCall {
		std::unique_ptr<Result> result;
		ULONGLONG deadline;
	};
	std::map<uint64_t, PendingCall> pending_;
	uint64_t next_id_ = 0;
	std::string last_status_;
	int last_status_code_ = 0;
	bool stopping_ = false;
};

}  // namespace flutter_zoom_sdk

#endif  // FLUTTER_PLUGIN_FLUTTER_ZOOM_SDK_PLUGIN_H_
