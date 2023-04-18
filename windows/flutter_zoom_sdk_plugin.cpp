#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "flutter_zoom_sdk_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <codecvt>

namespace flutter_zoom_sdk {
	FlutterZoomSdkPlugin* plagin;

	// static
	void FlutterZoomSdkPlugin::RegisterWithRegistrar(
		flutter::PluginRegistrarWindows* registrar) {
		auto channel =
			std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
				registrar->messenger(), "flutter_zoom_sdk",
				&flutter::StandardMethodCodec::GetInstance());
		auto eventChannel =
			std::make_unique<flutter::EventChannel<flutter::EncodableValue>>(
				registrar->messenger(), "flutter_zoom_sdk_event_stream",
				&flutter::StandardMethodCodec::GetInstance());

		auto plugin = std::make_unique<FlutterZoomSdkPlugin>();

		channel->SetMethodCallHandler(
			[plugin_pointer = plugin.get()](const auto& call, auto result) {
			plugin_pointer->HandleMethodCall(call, std::move(result));
		});

		eventChannel->SetStreamHandler(
			std::make_unique<flutter::StreamHandlerFunctions<>>(
				[plugin_pointer = plugin.get()](auto arguments, auto events) {
			plugin_pointer->FlutterZoomSdkPlugin::MeetingStreamListen(std::move(events));
			return nullptr;
		},
				[plugin_pointer = plugin.get()](auto arguments) {
			plugin_pointer->FlutterZoomSdkPlugin::meeting_event_sink_ = nullptr;
			return nullptr;
		}));

		registrar->AddPlugin(std::move(plugin));
	}

	FlutterZoomSdkPlugin::FlutterZoomSdkPlugin() {
		setlocale(LC_ALL, "");

		plagin = this;
		FlutterZoomSdkPlugin::AuthService = nullptr;
		FlutterZoomSdkPlugin::MeetingService = nullptr;
		FlutterZoomSdkPlugin::SettingService = nullptr;
	}

	FlutterZoomSdkPlugin::~FlutterZoomSdkPlugin() {
		FlutterZoomSdkPlugin::AuthService = nullptr;
		FlutterZoomSdkPlugin::MeetingService = nullptr;
		FlutterZoomSdkPlugin::SettingService = nullptr;
		FlutterZoomSdkPlugin::ZoomMeetingOptions.clear();

		ZOOM_SDK_NAMESPACE::CleanUPSDK();
	}

	void FlutterZoomSdkPlugin::MeetingStreamListen(unique_ptr<flutter::EventSink<>>&& events) {
		FlutterZoomSdkPlugin::meeting_event_sink_ = std::move(events);
		FlutterZoomSdkPlugin::createMeetingService();

		if (FlutterZoomSdkPlugin::MeetingService != NULL) {
			MeetingServiceEvent* meetingListener = new MeetingServiceEvent();
			FlutterZoomSdkPlugin::MeetingService->SetEvent(meetingListener);
		}
	}

	void FlutterZoomSdkPlugin::HandleMethodCall(
		const flutter::MethodCall<EncodableValue>& method_call,
		unique_ptr<flutter::MethodResult<EncodableValue>> result) {
		auto arguments = get_if<EncodableMap>(method_call.arguments());

		if (method_call.method_name().compare("getPlatformVersion") == 0) {
			ostringstream version_stream;
			version_stream << "Windows ";
			if (IsWindows10OrGreater()) {
				version_stream << "10+";
			}
			else if (IsWindows8OrGreater()) {
				version_stream << "8";
			}
			else if (IsWindows7OrGreater()) {
				version_stream << "7";
			}
			result->Success(EncodableValue(version_stream.str()));
		}
		else if (method_call.method_name().compare("init_and_join") == 0) {
			EncodableMap ZoomInitOptions = get<EncodableMap>(arguments->find(EncodableValue("initOptions"))->second);
			FlutterZoomSdkPlugin::ZoomMeetingOptions = get<EncodableMap>(arguments->find(EncodableValue("meetingOptions"))->second);

			bool res = FlutterZoomSdkPlugin::AuthorizeSDK(ZoomInitOptions);

			result->Success(EncodableValue(res));
		}
		else if (method_call.method_name().compare("leave_meeting") == 0) {
			bool res = FlutterZoomSdkPlugin::leaveMeeting();

			result->Success(EncodableValue(res));
		}
		else if (method_call.method_name().compare("show_meeting") == 0) {
			bool res = FlutterZoomSdkPlugin::showMeeting();

			result->Success(EncodableValue(res));
		}
		else if (method_call.method_name().compare("hide_meeting") == 0) {
			bool res = FlutterZoomSdkPlugin::hideMeeting();

			result->Success(EncodableValue(res));
		}
		else {
			result->NotImplemented();
		}
	}

	bool FlutterZoomSdkPlugin::AuthorizeSDK(EncodableMap ZoomInitOptions) {
		if (ZoomInitOptions.empty()) {
			return false;
		}

		// Initialize SDK with InitParam object
		ZOOM_SDK_NAMESPACE::InitParam initParam;
		ZOOM_SDK_NAMESPACE::SDKError initReturnVal(ZOOM_SDK_NAMESPACE::SDKERR_UNINITIALIZE);

		// Set web domain
		string domainStr = get<string>(ZoomInitOptions.find(EncodableValue("domain"))->second);
		wstring domainWstr = wstring(domainStr.begin(), domainStr.end());
		initParam.strWebDomain = domainWstr.c_str();
		initReturnVal = ZOOM_SDK_NAMESPACE::InitSDK(initParam);

		// Check if InitSDK call succeeded
		if (initReturnVal == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
			// Create IAuthService object to perform Auth actions
			ZOOM_SDK_NAMESPACE::IAuthService* authService;
			ZOOM_SDK_NAMESPACE::SDKError authServiceInitReturnVal = ZOOM_SDK_NAMESPACE::CreateAuthService(&authService);

			if (authServiceInitReturnVal == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS)
			{
				FlutterZoomSdkPlugin::AuthService = authService;

				// Create IAuthServiceEvent object to listen for Auth events from SDK
				AuthEvent* authListener;

				// Call SetEvent to assign your IAuthServiceEvent listener
				authListener = new AuthEvent();
				FlutterZoomSdkPlugin::AuthService->SetEvent(authListener);

				// Auth SDK with AuthParam
				ZOOM_SDK_NAMESPACE::AuthParam authParam;
				ZOOM_SDK_NAMESPACE::SDKError authCallReturnValue(ZOOM_SDK_NAMESPACE::SDKERR_UNAUTHENTICATION);

				// Provide your keys to the AuthParam object
				string appKeyStr = get<string>(ZoomInitOptions.find(EncodableValue("appKey"))->second);
				string appSecretStr = get<string>(ZoomInitOptions.find(EncodableValue("appSecret"))->second);

				wstring appKeyWstr = wstring(appKeyStr.begin(), appKeyStr.end());
				wstring appSecretWstr = wstring(appSecretStr.begin(), appSecretStr.end());

				authParam.appKey = appKeyWstr.c_str();
				authParam.appSecret = appSecretWstr.c_str();

				authCallReturnValue = FlutterZoomSdkPlugin::AuthService->SDKAuth(authParam);

				if (authCallReturnValue == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
					_cputts(L"Init and auth succeeded\n");
					return true;
				}
			}
		}

		return false;
	}

	void FlutterZoomSdkPlugin::joinMeeting() {
		auto ZoomMeetingOptions = FlutterZoomSdkPlugin::ZoomMeetingOptions;

		if (ZoomMeetingOptions.empty()) {
			return;
		}

		FlutterZoomSdkPlugin::createMeetingService();

		// Join meeting for non-login user with JoinParam object
		ZOOM_SDK_NAMESPACE::JoinParam joinMeetingParam = ZOOM_SDK_NAMESPACE::JoinParam();

		// Provide meeting credentials for non-login user using JoinParam4WithoutLogin
		ZOOM_SDK_NAMESPACE::JoinParam4WithoutLogin joinMeetingForNonLoginUserParam = ZOOM_SDK_NAMESPACE::JoinParam4WithoutLogin();

		joinMeetingParam.userType = ZOOM_SDK_NAMESPACE::SDK_UT_WITHOUT_LOGIN;

		string meetingNumberStr = get<string>(ZoomMeetingOptions.find(EncodableValue("meetingId"))->second);
		string meetingPasswordStr = get<string>(ZoomMeetingOptions.find(EncodableValue("meetingPassword"))->second);
		string userNameStr = get<string>(ZoomMeetingOptions.find(EncodableValue("displayName"))->second);
		string noAudioStr = get<string>(ZoomMeetingOptions.find(EncodableValue("noAudio"))->second);
		string noVideoStr = get<string>(ZoomMeetingOptions.find(EncodableValue("noVideo"))->second);

		wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
		wstring meetingPasswordWstr = convert.from_bytes(meetingPasswordStr);
		wstring userNameWstr = convert.from_bytes(userNameStr);

		joinMeetingForNonLoginUserParam.meetingNumber = stoull(meetingNumberStr);
		joinMeetingForNonLoginUserParam.psw = meetingPasswordWstr.c_str();
		joinMeetingForNonLoginUserParam.userName = userNameWstr.c_str();
		joinMeetingForNonLoginUserParam.isAudioOff = (noAudioStr == "true");
		joinMeetingForNonLoginUserParam.isVideoOff = (noVideoStr == "true");

		joinMeetingParam.param.withoutloginuserJoin = joinMeetingForNonLoginUserParam;

		FlutterZoomSdkPlugin::createSettingService();

		if (FlutterZoomSdkPlugin::SettingService != NULL)
		{
			ZOOM_SDK_NAMESPACE::IAudioSettingContext* pAudioContext = FlutterZoomSdkPlugin::SettingService->GetAudioSettings();

			if (pAudioContext)
			{
				pAudioContext->EnableAutoJoinAudio(true);
			}

			ZOOM_SDK_NAMESPACE::IShareSettingContext* pShareContext = FlutterZoomSdkPlugin::SettingService->GetShareSettings();

			if (pShareContext)
			{
				pShareContext->EnableAutoFitToWindowWhenViewSharing(false);
			}
		}

		ZOOM_SDK_NAMESPACE::SDKError joinMeetingCallReturnValue(ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN);

		joinMeetingCallReturnValue = FlutterZoomSdkPlugin::MeetingService->Join(joinMeetingParam);

		if (joinMeetingCallReturnValue == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
			// Join meeting call succeeded
			_cputts(L"Join meeting call succeeded\n");
		}
	}

	void FlutterZoomSdkPlugin::createMeetingService() {
		if (!FlutterZoomSdkPlugin::MeetingService) {
			ZOOM_SDK_NAMESPACE::SDKError meetingServiceInitReturnVal = ZOOM_SDK_NAMESPACE::CreateMeetingService(&(FlutterZoomSdkPlugin::MeetingService));

			if (meetingServiceInitReturnVal == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
				_cputts(L"Created Meeting Service\n");
			}
		}
	}

	void FlutterZoomSdkPlugin::createSettingService() {
		if (FlutterZoomSdkPlugin::SettingService == NULL) {
			ZOOM_SDK_NAMESPACE::SDKError settingServiceInitReturnVal = ZOOM_SDK_NAMESPACE::CreateSettingService(&(FlutterZoomSdkPlugin::SettingService));

			if (settingServiceInitReturnVal == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
				_cputts(L"Created Setting Service\n");
			}
		}
	}

	bool FlutterZoomSdkPlugin::leaveMeeting() {
		if (FlutterZoomSdkPlugin::MeetingService) {
			ZOOM_SDK_NAMESPACE::SDKError leaveMeetingReturnValue = FlutterZoomSdkPlugin::MeetingService->Leave(ZOOM_SDK_NAMESPACE::LEAVE_MEETING);

			if (leaveMeetingReturnValue == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
				_cputts(L"Left the meeting\n");
				return true;
			}
		}

		return false;
	}

	bool FlutterZoomSdkPlugin::showMeeting() {
		if (FlutterZoomSdkPlugin::MeetingService) {
			ZOOM_SDK_NAMESPACE::IMeetingUIController* UIController = FlutterZoomSdkPlugin::MeetingService->GetUIController();

			if (UIController) {
				HWND firstView = NULL;
				HWND secondView = NULL;

				UIController->GetMeetingUIWnd(firstView, secondView);

				if (firstView) {
					ShowWindow(firstView, SW_SHOWMAXIMIZED);
					UIController->BackToMeeting();

					_cputts(L"Show meeting screen\n");

					return true;
				}
			}
		}

		return false;
	}

	bool FlutterZoomSdkPlugin::hideMeeting() {
		if (FlutterZoomSdkPlugin::MeetingService) {
			ZOOM_SDK_NAMESPACE::IMeetingUIController* UIController = FlutterZoomSdkPlugin::MeetingService->GetUIController();


			if (UIController) {
				HWND firstView = NULL;
				HWND secondView = NULL;

				UIController->GetMeetingUIWnd(firstView, secondView);

				if (firstView) {
				    // Активируем окно конференции Zoom
					ShowWindow(firstView, SW_SHOW);
					//ShowWindow(firstView, SW_MINIMIZE);

					// Выходит из полноэкранного режима, если он включен (т.к. иначе свернуть с помощью Win+Down не выйдет)
					UIController->ExitFullScreen(true, true);

					// ShowWindow не позволяет отобразить мини окно Zoom при сворачивании окна, поэтому
                    // сворачиваем окно конференции при помощи клавиш Win + Down.
					FlutterZoomSdkPlugin::pressWinAndDownKeys();

					_cputts(L"Hide meeting screen\n");

					return true;
				}

			}
		}

		return false;
	}

	void FlutterZoomSdkPlugin::pressWinAndDownKeys() {
		INPUT inputs[4] = {};
		ZeroMemory(inputs, sizeof(inputs));

		inputs[0].type = INPUT_KEYBOARD;
		inputs[0].ki.wVk = VK_LWIN;

		inputs[1].type = INPUT_KEYBOARD;
		inputs[1].ki.wVk = VK_DOWN;

		inputs[2].type = INPUT_KEYBOARD;
		inputs[2].ki.wVk = VK_DOWN;
		inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

		inputs[3].type = INPUT_KEYBOARD;
		inputs[3].ki.wVk = VK_LWIN;
		inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

		UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		if (uSent != ARRAYSIZE(inputs))
		{
			_cputts(L"Press win and down key failed\n");
		}
	}

	// class AuthEvent 
	AuthEvent::AuthEvent() {}

	AuthEvent::~AuthEvent() {}

	void AuthEvent::onAuthenticationReturn(ZOOM_SDK_NAMESPACE::AuthResult ret) {
		if (ret == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
			plagin->joinMeeting();
		}
	}

	void AuthEvent::onLoginReturnWithReason(ZOOM_SDK_NAMESPACE::LOGINSTATUS ret, ZOOM_SDK_NAMESPACE::IAccountInfo* pAccountInfo, ZOOM_SDK_NAMESPACE::LoginFailReason reason) {}

	void AuthEvent::onLogout() {}

	void AuthEvent::onZoomIdentityExpired() {}

	void AuthEvent::onZoomAuthIdentityExpired() {}

	void AuthEvent::onNotificationServiceStatus(ZOOM_SDK_NAMESPACE::SDKNotificationServiceStatus status) {}


	// class MeetingServiceEvent 
	MeetingServiceEvent::MeetingServiceEvent() {}

	MeetingServiceEvent::~MeetingServiceEvent() {}

	void MeetingServiceEvent::onMeetingStatusChanged(ZOOM_SDK_NAMESPACE::MeetingStatus status, int iResult) {
		if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_INMEETING) {
			EncodableList results;

			results.push_back(EncodableValue("MEETING_STATUS_INMEETING"));
			results.push_back(EncodableValue(iResult));

			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_DISCONNECTING) {
			EncodableList results;

			results.push_back(EncodableValue("MEETING_STATUS_DISCONNECTING"));
			results.push_back(EncodableValue(iResult));

			plagin->meeting_event_sink_->Success(results);
		}
	}

	void MeetingServiceEvent::onMeetingStatisticsWarningNotification(ZOOM_SDK_NAMESPACE::StatisticsWarningType type) {}

	void MeetingServiceEvent::onMeetingParameterNotification(const ZOOM_SDK_NAMESPACE::MeetingParameter* meeting_param) {}

	void MeetingServiceEvent::onSuspendParticipantsActivities(){}

}  // namespace flutter_zoom_sdk
