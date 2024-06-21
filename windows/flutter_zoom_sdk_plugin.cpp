#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "flutter_zoom_sdk_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// This header contains the programming interfaces for various Windows controls, such as buttons,
// combo boxes, date and time pickers, and toolbars.
#include <commctrl.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <codecvt>

#pragma comment(lib, "Comctl32.lib")

namespace flutter_zoom_sdk {
	FlutterZoomSdkPlugin* plagin;

	std::string wstring_to_string(const std::wstring& wstr) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
		return convert.to_bytes(wstr);
	}

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
		else if (method_call.method_name().compare("init_and_start") == 0) {
			EncodableMap ZoomInitOptions = get<EncodableMap>(arguments->find(EncodableValue("initOptions"))->second);
			FlutterZoomSdkPlugin::ZoomMeetingOptions = get<EncodableMap>(arguments->find(EncodableValue("meetingOptions"))->second);

			bool res = FlutterZoomSdkPlugin::AuthorizeSDK(ZoomInitOptions);

			result->Success(EncodableValue(res));
		}
        else if (method_call.method_name().compare("init") == 0) {
            EncodableMap ZoomInitOptions = get<EncodableMap>(arguments->find(EncodableValue("initOptions"))->second);

            bool res = FlutterZoomSdkPlugin::AuthorizeSDK(ZoomInitOptions);

            result->Success(EncodableValue(res));
        }
        else if (method_call.method_name().compare("login") == 0) {
            FlutterZoomSdkPlugin::ZoomMeetingOptions = get<EncodableMap>(arguments->find(EncodableValue("meetingOptions"))->second);

            bool res = FlutterZoomSdkPlugin::startMeeting();

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
		else if (method_call.method_name().compare("disable_window_styles") == 0) {
			bool res = FlutterZoomSdkPlugin::disableWindowStyles();

			result->Success(EncodableValue(res));
		}
		else if (method_call.method_name().compare("get_participants") == 0) {
			auto participants_list = FlutterZoomSdkPlugin::getParticipants();
			result->Success(flutter::EncodableValue(participants_list));
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
				authListener = new AuthEvent(ZoomMeetingOptions);
				FlutterZoomSdkPlugin::AuthService->SetEvent(authListener);

				ZOOM_SDK_NAMESPACE::SDKError authCallReturnValue(ZOOM_SDK_NAMESPACE::SDKERR_UNAUTHENTICATION);

				// Auth SDK with AuthContext object
				ZOOM_SDK_NAMESPACE::AuthContext authContext;

				// Provide your JWT to the AuthContext object
				string jwtTokenStr = get<string>(ZoomInitOptions.find(EncodableValue("jwtToken"))->second);
				wstring jwtTokenWstr = wstring(jwtTokenStr.begin(), jwtTokenStr.end());
				authContext.jwt_token = jwtTokenWstr.c_str();

				authCallReturnValue = FlutterZoomSdkPlugin::AuthService->SDKAuth(authContext);

				if (authCallReturnValue == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
					_cputts(L"Init and auth succeeded\n");
					return true;
				}
			}
		}

		return false;
	}

    bool FlutterZoomSdkPlugin::startMeeting() {
        auto ZoomMeetingOptions = FlutterZoomSdkPlugin::ZoomMeetingOptions;

        if (ZoomMeetingOptions.empty()) {
            return false;
        }

        FlutterZoomSdkPlugin::createMeetingService();
        FlutterZoomSdkPlugin::createSettingService();

        // Join meeting for non-login user with StartParam object
        ZOOM_SDK_NAMESPACE::StartParam startMeetingParam = ZOOM_SDK_NAMESPACE::StartParam();

        // Provide meeting credentials for API user using StartParam4WithoutLogin
        ZOOM_SDK_NAMESPACE::StartParam4WithoutLogin startMeetingWithoutLoginParam;

        startMeetingParam.userType = ZOOM_SDK_NAMESPACE::SDK_UT_WITHOUT_LOGIN;

        string meetingNumberStr = get<string>(ZoomMeetingOptions.find(EncodableValue("meetingId"))->second);
        string userZAKStr = get<string>(ZoomMeetingOptions.find(EncodableValue("zoomAccessToken"))->second);
        string userNameStr = get<string>(ZoomMeetingOptions.find(EncodableValue("userId"))->second);
        string noAudioStr = get<string>(ZoomMeetingOptions.find(EncodableValue("noAudio"))->second);
        string noVideoStr = get<string>(ZoomMeetingOptions.find(EncodableValue("noVideo"))->second);

        wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
        wstring userZAKWstr = convert.from_bytes(userZAKStr);
        wstring userNameWstr = convert.from_bytes(userNameStr);

        startMeetingWithoutLoginParam.zoomuserType = ZOOM_SDK_NAMESPACE::ZoomUserType_APIUSER;
        startMeetingWithoutLoginParam.meetingNumber = stoull(meetingNumberStr);
        startMeetingWithoutLoginParam.userZAK = userZAKWstr.c_str();
        startMeetingWithoutLoginParam.userName = userNameWstr.c_str();
		startMeetingWithoutLoginParam.customer_key = NULL;
		startMeetingWithoutLoginParam.vanityID = NULL;
		startMeetingWithoutLoginParam.hDirectShareAppWnd = NULL;
		startMeetingWithoutLoginParam.isDirectShareDesktop = false;
		startMeetingWithoutLoginParam.isAudioOff = (noAudioStr == "true");
		startMeetingWithoutLoginParam.isVideoOff = (noVideoStr == "true");

        startMeetingParam.param.withoutloginStart = startMeetingWithoutLoginParam;

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

        ZOOM_SDK_NAMESPACE::IMeetingConfiguration* meetingConfiguration = FlutterZoomSdkPlugin::MeetingService->GetMeetingConfiguration();

        if (meetingConfiguration)
        {
            meetingConfiguration->HideMeetingInfoOnMeetingUI(true);
        }

        if (FlutterZoomSdkPlugin::MeetingService) {
            ZOOM_SDK_NAMESPACE::SDKError startMeetingReturnValue = FlutterZoomSdkPlugin::MeetingService->Start(startMeetingParam);

            if (startMeetingReturnValue == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
                _cputts(L"Start meeting call succeeded\n");
                return true;
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
		FlutterZoomSdkPlugin::createSettingService();

		// Join meeting for non-login user with JoinParam object
		ZOOM_SDK_NAMESPACE::JoinParam joinMeetingParam = ZOOM_SDK_NAMESPACE::JoinParam();

		// Provide meeting credentials for non-login user using JoinParam4WithoutLogin
		ZOOM_SDK_NAMESPACE::JoinParam4NormalUser joinMeetingForNonLoginUserParam = ZOOM_SDK_NAMESPACE::JoinParam4NormalUser();

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

		joinMeetingParam.param.normaluserJoin = joinMeetingForNonLoginUserParam;

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

		ZOOM_SDK_NAMESPACE::IMeetingConfiguration* meetingConfiguration = FlutterZoomSdkPlugin::MeetingService->GetMeetingConfiguration();

        if (meetingConfiguration)
        {
        	meetingConfiguration->HideMeetingInfoOnMeetingUI(true);
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
					//ShowWindow(firstView, SW_MINIMIZE);

					// Выходим из полноэкранного режима, если он включен (т.к. иначе свернуть с помощью Win+Down не выйдет)
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

	bool FlutterZoomSdkPlugin::disableWindowStyles() {
	  if (FlutterZoomSdkPlugin::MeetingService) {
		ZoomWindowHelper::SetupZoomWindow(FlutterZoomSdkPlugin::MeetingService);
		return true;
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

	flutter::EncodableList FlutterZoomSdkPlugin::getParticipants() {
		flutter::EncodableList participants_list;

		if (FlutterZoomSdkPlugin::MeetingService) {
			auto participants = ZoomParticipantsHelper::GetParticipantsList(FlutterZoomSdkPlugin::MeetingService);

			for (const auto &participant : participants) {
				flutter::EncodableMap participant_map;
				participant_map[flutter::EncodableValue("userName")] = flutter::EncodableValue(wstring_to_string(participant.userName));
            	participant_map[flutter::EncodableValue("customerKey")] = flutter::EncodableValue(wstring_to_string(participant.customerKey));
            	participant_map[flutter::EncodableValue("isHost")] = flutter::EncodableValue(participant.isHost);
            	participant_map[flutter::EncodableValue("isVideoOn")] = flutter::EncodableValue(participant.isVideoOn);
            	participant_map[flutter::EncodableValue("isAudioMuted")] = flutter::EncodableValue(participant.isAudioMuted);
            	participant_map[flutter::EncodableValue("isTalking")] = flutter::EncodableValue(participant.isTalking);
            	participant_map[flutter::EncodableValue("hasCamera")] = flutter::EncodableValue(participant.hasCamera);
				participants_list.push_back(flutter::EncodableValue(participant_map));
			}
		}

		return participants_list;
	}

	LRESULT CALLBACK ZoomWindowHelper::CustomWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
		switch (uMsg) {
			case WM_NCLBUTTONDBLCLK:
				// Prevent double-click on title bar
				return 0;
			case WM_SYSCOMMAND:
				// Prevent system commands like maximize and restore
				if (wParam == SC_MAXIMIZE || wParam == SC_RESTORE) {
					return 0;
				}
				break;
			default:
				break;
		}

		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}

	void ZoomWindowHelper::DisableWindowControls(HWND hWnd) {
		if (hWnd) {
			// Remove minimize, maximize, and close buttons
			LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
			style &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
			style |= WS_CAPTION;
			SetWindowLongPtr(hWnd, GWL_STYLE, style);
			SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

			// Subclass the window procedure to intercept messages
			SetWindowSubclass(hWnd, CustomWindowProc, 0, 0);
		}
	}

	void ZoomWindowHelper::SetWindowSizeAndPosition(HWND hWnd) {
		if (hWnd) {
            // Get the work area dimensions (excluding taskbar)
            RECT workAreaRect;
            SystemParametersInfo(SPI_GETWORKAREA, 0, &workAreaRect, 0);
            int workAreaWidth = workAreaRect.right - workAreaRect.left;
            int workAreaHeight = workAreaRect.bottom - workAreaRect.top;

            // Calculate the desired window dimensions and position
            int windowWidth = workAreaWidth / 2;
            int windowHeight = workAreaHeight;
            int windowX = workAreaWidth / 2;
            int windowY = 0;

			// Set the window size and position
			SetWindowPos(hWnd, NULL, windowX, windowY, windowWidth, windowHeight, SWP_NOZORDER | SWP_FRAMECHANGED);
		}
	}

	void ZoomWindowHelper::SetupZoomWindow(ZOOM_SDK_NAMESPACE::IMeetingService* pMeetingService) {
		if (pMeetingService) {
			ZOOM_SDK_NAMESPACE::IMeetingUIController* pUIController = pMeetingService->GetUIController();

			if (pUIController) {
				HWND firstView = NULL;
				HWND secondView = NULL;

				pUIController->GetMeetingUIWnd(firstView, secondView);

				if (firstView) {
					DisableWindowControls(firstView);
					SetWindowSizeAndPosition(firstView);
				}
			}
		}
	}

	std::vector<ParticipantInfo> ZoomParticipantsHelper::GetParticipantsList(ZOOM_SDK_NAMESPACE::IMeetingService* pMeetingService) {
		std::vector<ParticipantInfo> participants;

		if (pMeetingService) {
			ZOOM_SDK_NAMESPACE::IMeetingParticipantsController* pParticipantsController = pMeetingService->GetMeetingParticipantsController();
			if (pParticipantsController) {
				ZOOM_SDK_NAMESPACE::IList<unsigned int>* lstUserID = pParticipantsController->GetParticipantsList();
				if (lstUserID) {
					for (int i = 0; i < lstUserID->GetCount(); ++i) {
						unsigned int userId = lstUserID->GetItem(i);
						ZOOM_SDK_NAMESPACE::IUserInfo* pUserInfo = pParticipantsController->GetUserByUserID(userId);
						if (pUserInfo) {
							ParticipantInfo info;
							info.userName = pUserInfo->GetUserNameW();
							info.customerKey = pUserInfo->GetCustomerKey();
							info.isHost = pUserInfo->IsHost();
							info.isVideoOn = pUserInfo->IsVideoOn();
							info.isAudioMuted = pUserInfo->IsAudioMuted();
							info.isTalking = pUserInfo->IsTalking();
							info.hasCamera = pUserInfo->HasCamera();
							participants.push_back(info);
						}
					}
				}
			}
		}

		return participants;
	}

	// class AuthEvent
	AuthEvent::AuthEvent(const EncodableMap& zoomMeetingOptions) : zoomMeetingOptions_(zoomMeetingOptions) {}

	AuthEvent::~AuthEvent() {}

	void AuthEvent::onAuthenticationReturn(ZOOM_SDK_NAMESPACE::AuthResult ret) {
		string isStartMeetingStr = get<string>(zoomMeetingOptions_.find(EncodableValue("isStartMeeting"))->second);
		bool isStartMeeting = (isStartMeetingStr == "true");

		if (ret == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
			if (isStartMeeting) {
			plagin->startMeeting();
        } else {
			plagin->joinMeeting();
		}

		_cputts(L"onAuthenticationReturn succeeded\n");
		}
	}

	void AuthEvent::onLoginReturnWithReason(ZOOM_SDK_NAMESPACE::LOGINSTATUS ret, ZOOM_SDK_NAMESPACE::IAccountInfo* pAccountInfo, ZOOM_SDK_NAMESPACE::LoginFailReason reason) {}

	void AuthEvent::onLogout() {}

	void AuthEvent::onZoomIdentityExpired() {}

	void AuthEvent::onZoomAuthIdentityExpired() {}

	void AuthEvent::onNotificationServiceStatus(ZOOM_SDK_NAMESPACE::SDKNotificationServiceStatus status) {}

	void AuthEvent::onNotificationServiceStatus(ZOOM_SDK_NAMESPACE::SDKNotificationServiceStatus status, ZOOM_SDK_NAMESPACE::SDKNotificationServiceError error) {}


	// class MeetingServiceEvent
	MeetingServiceEvent::MeetingServiceEvent() {}

	MeetingServiceEvent::~MeetingServiceEvent() {}

	void MeetingServiceEvent::onMeetingStatusChanged(ZOOM_SDK_NAMESPACE::MeetingStatus status,
													 int iResult)
	{
		if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_IDLE) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_IDLE"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_CONNECTING) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_CONNECTING"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_WAITINGFORHOST) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_WAITINGFORHOST"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_INMEETING) {
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
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_RECONNECTING) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_RECONNECTING"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_FAILED) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_FAILED"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_ENDED) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_ENDED"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_UNKNOW) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_UNKNOW"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_LOCKED) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_LOCKED"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_UNLOCKED) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_UNLOCKED"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_IN_WAITING_ROOM) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_IN_WAITING_ROOM"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_WEBINAR_PROMOTE) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_WEBINAR_PROMOTE"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_WEBINAR_DEPROMOTE) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_WEBINAR_DEPROMOTE"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_JOIN_BREAKOUT_ROOM) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_JOIN_BREAKOUT_ROOM"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
		else if (status == ZOOM_SDK_NAMESPACE::MEETING_STATUS_LEAVE_BREAKOUT_ROOM) {
			EncodableList results;
			results.push_back(EncodableValue("MEETING_STATUS_LEAVE_BREAKOUT_ROOM"));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
	}

	void MeetingServiceEvent::onMeetingStatisticsWarningNotification(ZOOM_SDK_NAMESPACE::StatisticsWarningType type) {}

	void MeetingServiceEvent::onMeetingParameterNotification(const ZOOM_SDK_NAMESPACE::MeetingParameter* meeting_param) {}

	void MeetingServiceEvent::onSuspendParticipantsActivities(){}

	void MeetingServiceEvent::onAICompanionActiveChangeNotice(bool bActive) {}

}  // namespace flutter_zoom_sdk
