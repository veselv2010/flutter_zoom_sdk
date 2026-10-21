#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "flutter_zoom_sdk_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <codecvt>

#include <algorithm>
#include <cctype>
#include <meeting_service_components/meeting_audio_interface.h>
#include <meeting_service_components/meeting_video_interface.h>
#include <meeting_service_components/meeting_participants_ctrl_interface.h>

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
		plagin = this;
		FlutterZoomSdkPlugin::AuthService = nullptr;
		FlutterZoomSdkPlugin::MeetingService = nullptr;
		FlutterZoomSdkPlugin::SettingService = nullptr;
	}

    FlutterZoomSdkPlugin::~FlutterZoomSdkPlugin() {
        cleanupSDK();
        ZoomMeetingOptions.clear();
        plagin = nullptr;
    }

    void FlutterZoomSdkPlugin::cleanupSDK() {
        if (MeetingService) {
            MeetingService->SetEvent(nullptr);
            ZOOM_SDK_NAMESPACE::DestroyMeetingService(MeetingService);
            MeetingService = nullptr;
        }
        if (SettingService) {
            ZOOM_SDK_NAMESPACE::DestroySettingService(SettingService);
            SettingService = nullptr;
        }
        if (AuthService) {
            AuthService->SetEvent(nullptr);
            ZOOM_SDK_NAMESPACE::DestroyAuthService(AuthService);
            AuthService = nullptr;
        }

        meetingListener.reset();
        authListener.reset();
        pinnedHostId = 0;
        if (sdkInitialized) {
            ZOOM_SDK_NAMESPACE::CleanUPSDK();
            sdkInitialized = false;
        }
    }

    void FlutterZoomSdkPlugin::MeetingStreamListen(
            unique_ptr<flutter::EventSink<>>&& events) {
        meeting_event_sink_ = std::move(events);
        if (!sdkInitialized) {
            return;
        }
        createMeetingService();

        if (MeetingService && meeting_event_sink_ &&
            MeetingService->GetMeetingStatus() ==
            ZOOM_SDK_NAMESPACE::MEETING_STATUS_INMEETING) {
            EncodableList results;
            results.push_back(EncodableValue("MEETING_STATUS_INMEETING"));
            results.push_back(EncodableValue(0));
            meeting_event_sink_->Success(results);
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

        cleanupSDK();

        // Initialize SDK with InitParam object
		ZOOM_SDK_NAMESPACE::InitParam initParam;
		ZOOM_SDK_NAMESPACE::SDKError initReturnVal(ZOOM_SDK_NAMESPACE::SDKERR_UNINITIALIZE);

		// Set web domain
		string domainStr = get<string>(ZoomInitOptions.find(EncodableValue("domain"))->second);
		wstring domainWstr = wstring(domainStr.begin(), domainStr.end());
		initParam.strWebDomain = domainWstr.c_str();

        // Set language
        auto itLang = ZoomInitOptions.find(EncodableValue("language"));
        if (itLang != ZoomInitOptions.end()) {
            string langStr = get<string>(itLang->second);
            initParam.emLanguageID = LanguageIdFromString(langStr);
        }

		initReturnVal = ZOOM_SDK_NAMESPACE::InitSDK(initParam);

		// Check if InitSDK call succeeded
        if (initReturnVal == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS) {
            sdkInitialized = true;
			// Create IAuthService object to perform Auth actions
			ZOOM_SDK_NAMESPACE::IAuthService* authService;
			ZOOM_SDK_NAMESPACE::SDKError authServiceInitReturnVal = ZOOM_SDK_NAMESPACE::CreateAuthService(&authService);

			if (authServiceInitReturnVal == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS)
			{
				FlutterZoomSdkPlugin::AuthService = authService;

				// Create IAuthServiceEvent object to listen for Auth events from SDK
                // Call SetEvent to assign your IAuthServiceEvent listener
                authListener = std::make_unique<AuthEvent>();
                FlutterZoomSdkPlugin::AuthService->SetEvent(authListener.get());

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

        cleanupSDK();
		return false;
	}

	void FlutterZoomSdkPlugin::joinMeeting() {
		auto ZoomMeetingOptions = FlutterZoomSdkPlugin::ZoomMeetingOptions;

		if (ZoomMeetingOptions.empty()) {
			return;
		}

        FlutterZoomSdkPlugin::createMeetingService();
        if (!MeetingService) {
            return;
        }
        FlutterZoomSdkPlugin::createSettingService();

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
            meetingConfiguration->EnableInviteButtonOnMeetingUI(false);
            meetingConfiguration->EnableInviteLinkOnMeetingUI(false);
            meetingConfiguration->HideRecordItemOnMeetingUI(true);
            meetingConfiguration->HideUpgradeFreeMeetingButton(true);
        }

		ZOOM_SDK_NAMESPACE::SDKError joinMeetingCallReturnValue(ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN);

		joinMeetingCallReturnValue = FlutterZoomSdkPlugin::MeetingService->Join(joinMeetingParam);

        if (joinMeetingCallReturnValue == ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS) {
            _cputts(L"Join meeting call succeeded\n");
        } else {
            OutputDebugStringW(
                    (L"Zoom join failed: " +
                     std::to_wstring(static_cast<int>(joinMeetingCallReturnValue)) +
                     L"\n").c_str()
            );
        }
	}

    void FlutterZoomSdkPlugin::createMeetingService() {
        if (!sdkInitialized) {
            return;
        }
        if (!MeetingService) {
            ZOOM_SDK_NAMESPACE::SDKError result =
                    ZOOM_SDK_NAMESPACE::CreateMeetingService(&MeetingService);
            if (result != ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS) {
                return;
            }
            _cputts(L"Created Meeting Service\n");
        }
        if (!meetingListener) {
            meetingListener = std::make_unique<MeetingServiceEvent>();
            MeetingService->SetEvent(meetingListener.get());
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
                    unpinHostVideo();
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
        if (MeetingService) {
            ZOOM_SDK_NAMESPACE::IMeetingUIController* UIController = MeetingService->GetUIController();

            if (UIController) {
                HWND firstView = NULL;
                HWND secondView = NULL;

                UIController->GetMeetingUIWnd(firstView, secondView);

                if (firstView) {
                    UIController->ExitFullScreen(true, true);
                    pinHostVideo();

                    return PostMessage(firstView, WM_SYSCOMMAND, SC_MINIMIZE, 0) != 0;
                }
            }
        }

        return false;
    }

    void FlutterZoomSdkPlugin::pinHostVideo() {
        if (!MeetingService || pinnedHostId != 0) {
            return;
        }
        auto* participants = MeetingService->GetMeetingParticipantsController();
        auto* video = MeetingService->GetMeetingVideoController();
        if (!participants || !video) {
            return;
        }
        auto* users = participants->GetParticipantsList();
        if (!users) {
            return;
        }
        for (int i = 0; i < users->GetCount(); ++i) {
            unsigned int id = users->GetItem(i);
            auto* user = participants->GetUserByUserID(id);
            if (user && user->IsHost() && !user->IsMySelf()) {
                auto* pinned = video->GetPinnedUserListFromFirstView();
                if (pinned) {
                    for (int j = 0; j < pinned->GetCount(); ++j) {
                        if (pinned->GetItem(j) == id) {
                            return;
                        }
                    }
                }
                if (video->PinVideoToFirstView(id) ==
                    ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS) {
                    pinnedHostId = id;
                }
                return;
            }
        }
    }

    void FlutterZoomSdkPlugin::unpinHostVideo() {
        if (pinnedHostId == 0) {
            return;
        }
        if (MeetingService) {
            auto* video = MeetingService->GetMeetingVideoController();
            if (video) {
                video->UnPinVideoFromFirstView(pinnedHostId);
            }
        }
        pinnedHostId = 0;
    }

	// class AuthEvent
	AuthEvent::AuthEvent() {}

	AuthEvent::~AuthEvent() {}

	void AuthEvent::onAuthenticationReturn(ZOOM_SDK_NAMESPACE::AuthResult ret) {
		if (ret == ZOOM_SDK_NAMESPACE::AUTHRET_SUCCESS) {
			if (plagin) {
				plagin->joinMeeting();
			}
		} else {
			OutputDebugStringW(
					(L"Zoom authentication failed: " +
					 std::to_wstring(static_cast<int>(ret)) + L"\n").c_str()
			);
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

	void MeetingServiceEvent::onMeetingStatusChanged(
			ZOOM_SDK_NAMESPACE::MeetingStatus status, int iResult) {
		const char* state = nullptr;
		switch (status) {
			case ZOOM_SDK_NAMESPACE::MEETING_STATUS_INMEETING:
				state = "MEETING_STATUS_INMEETING";
				break;
			case ZOOM_SDK_NAMESPACE::MEETING_STATUS_DISCONNECTING:
				state = "MEETING_STATUS_DISCONNECTING";
				break;
			case ZOOM_SDK_NAMESPACE::MEETING_STATUS_FAILED:
				state = "MEETING_STATUS_FAILED";
				break;
			case ZOOM_SDK_NAMESPACE::MEETING_STATUS_ENDED:
				state = "MEETING_STATUS_ENDED";
				break;
			default:
				return;
		}

		if (plagin && plagin->meeting_event_sink_) {
			EncodableList results;
			results.push_back(EncodableValue(state));
			results.push_back(EncodableValue(iResult));
			plagin->meeting_event_sink_->Success(results);
		}
	}

	void MeetingServiceEvent::onMeetingStatisticsWarningNotification(ZOOM_SDK_NAMESPACE::StatisticsWarningType type) {}

	void MeetingServiceEvent::onMeetingParameterNotification(const ZOOM_SDK_NAMESPACE::MeetingParameter* meeting_param) {}

	void MeetingServiceEvent::onSuspendParticipantsActivities(){}

	void MeetingServiceEvent::onAICompanionActiveChangeNotice(bool bActive){}

	void MeetingServiceEvent::onMeetingTopicChanged(const zchar_t *sTopic){}

	void MeetingServiceEvent::onMeetingFullToWatchLiveStream(const zchar_t* sLiveStreamUrl){}

	void MeetingServiceEvent::onUserNetworkStatusChanged(
			ZOOM_SDK_NAMESPACE::MeetingComponentType type,
			ZOOM_SDK_NAMESPACE::ConnectionQuality level,
			unsigned int userId,
			bool uplink
	) {}

	void MeetingServiceEvent::onAppSignalPanelUpdated(
			ZOOM_SDK_NAMESPACE::IMeetingAppSignalHandler* pHandler
	) {}

    ZOOM_SDK_NAMESPACE::SDK_LANGUAGE_ID LanguageIdFromString(const string &lang_in) {
        if (lang_in.empty()) return ZOOM_SDK_NAMESPACE::LANGUAGE_Unknown;

        string s = lang_in;
        transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c) { return tolower(c); });

        if (s.rfind("en", 0) == 0) {
            return ZOOM_SDK_NAMESPACE::LANGUAGE_English;
        }

        if (s.rfind("ru", 0) == 0) {
            return ZOOM_SDK_NAMESPACE::LANGUAGE_Russian;
        }

        return ZOOM_SDK_NAMESPACE::LANGUAGE_Unknown;
    }

}  // namespace flutter_zoom_sdk
