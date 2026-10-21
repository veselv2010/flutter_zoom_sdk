/**
 * @file meeting_ui_ctrl_interface.h
 * @brief UI Controller of Meeting Service Interface
 * Valid only for ZOOM style user interface mode.
 */
#ifndef _MEETING_UI_CTRL_INTERFACE_H_
#define _MEETING_UI_CTRL_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
/**
 * @brief Enumeration of the displayed type of user videos in the meeting.
 * Here are more detailed structural descriptions.
 */
enum SDKMeetingUIType
{
	/** For initialization. */
	SDK_Meeting_UI_None,
	/** Video wall mode. */
	SDK_Meeting_UI_VideoWall_Mode,
	/** Active user mode. */
	SDK_Meeting_UI_ActiveRender_Mode,
};

/**
 * @brief Video wall page information.
 * Here are more detailed structural descriptions.
 */
typedef struct tagVideoWallPageInfoParam
{
	/** The page in video wall mode for the moment. */
	int nCurrentPage;
	/** The total number of pages in video wall mode. */
	int nTotalPages;

	tagVideoWallPageInfoParam()
	{
		nCurrentPage = 0;
		nTotalPages = 0;
	}
}VideoWallPageInfoParam;

/** 
 * @brief The configuration of the parameters to display the dialog. 
 * Here are more detailed structural descriptions.
 */
typedef struct tagShowChatDlgParam
{
	/** Parent window handle. */
	HWND hParent;
	/** Chat dialog position. */
	RECT rect;
	/** Chat dialog handle. */
	HWND hChatWnd;
	tagShowChatDlgParam()
	{
		hParent = nullptr;
		hChatWnd = nullptr;
 		rect.top = 0;
 		rect.bottom = 0;
 		rect.left = 0;
 		rect.right = 0;
	}
}ShowChatDlgParam;

/**
 * @brief Enumeration of float video user interface type.
 * Here are more detailed structural descriptions.
 */
enum SDKFloatVideoType
{
	/** Type of list. */
	FLOATVIDEO_List,
	/** Small.  */
	FLOATVIDEO_Small,
	/** Large. */
	FLOATVIDEO_Large,
	/** Minimized. */
	FLOATVIDEO_Minimize,
};

/**
 * @brief Enumeration of minimize user interface mode.
 * Here are more detailed structural descriptions.
 */
enum SDKMinimizeUIMode
{
	/** For initialization. */
	MinimizeUIMode_NONE,
	/** Minimized mode for sharing. */
	MinimizeUIMode_SHARE,
	/** Minimized mode for video. */
	MinimizeUIMode_VIDEO,
	/** Minimized mode for speaking. */
	MinimizeUIMode_ACTIVESPEAKER,
};

/**
 * @brief Split screen mode information.
 * Here are more detailed structural descriptions.
 */
typedef struct tagSplitScreenInfo
{
	/** Support display the video in a row. */
	bool bSupportSplitScreen;
	/** In the process of displaying the video in the row. */
	bool bInSplitScreenMode;
	tagSplitScreenInfo()
	{
		bSupportSplitScreen = false;
		bInSplitScreenMode = false;
	}
}SplitScreenInfo;

/**
 * @brief Enumeration of action user suggested to take after getting the callback event "IMeetingUIControllerEvent::onAudioBtnClicked()".
 * Here are more detailed structural descriptions.
 */
enum AudioCallbackActionInfo
{
	/** For initialization. */
	ACTION_NONE = 0,
	/** Choose audio device because no audio device is connected yet. */
	ACTION_CHOOSE_AUDIO_DEVICE_NOAUDIODEVICECONNECTTED,
	/** Choose audio device because there is an error in the connected computer audio device. */
	ACTION_CHOOSE_AUDIO_DEVICE_COMPUTERAUDIODEVICEERROR,
	/** Choose audio device because there is an error in the connected phone call device. */
	ACTION_CHOOSE_AUDIO_DEVICE_PHONECALLDEVICEERROR,
	/** Need to join voip. */
	ACTION_NEED_JOIN_VOIP,
	/** Mute or unmute some user's audio according to the "AudioBtnClickedCallbackInfo::userid_MuteUnmute" */
	ACTION_MUTE_UNMUTE_AUDIO,
	/** Show audio setting window. */
	ACTION_SHOW_AUDIO_SETTING_WINDOW,
};

/**
 * @brief The suggested action information for user to handle after getting the callback event "IMeetingUIControllerEvent::onAudioBtnClicked()"
 * Here are more detailed structural descriptions.
 */
typedef struct tagAudioBtnClickedCallbackInfo
{
	/** The id of the user that should be muted or unmuted. When no mute or unmute operation is required, the value is 0 */
	unsigned int userid_MuteUnmute;
	/** The suggested action for user to take. */
	AudioCallbackActionInfo audio_clicked_action;
	tagAudioBtnClickedCallbackInfo()
	{
		userid_MuteUnmute = 0;
		audio_clicked_action = ACTION_NONE;
	}

}AudioBtnClickedCallbackInfo;
/**
 * @class IMeetingUIControllerEvent
 * @brief Callback Event of Meeting UI Controller.
 */
class IMeetingUIControllerEvent
{
public:
	virtual ~IMeetingUIControllerEvent() {}
	
	/**
	 * @brief Callback event to click the INVITE button.
	 * @param [out] bHandled true indicates to show the user's own custom dialog interface. Default value: false.
	 * @note If the value of bHandled is not set to true, the default interface will pop up. 
	 */
	virtual void onInviteBtnClicked(bool& bHandled) = 0;
	
	/**
	 * @brief Callback event for clicking START SHARE button.
	 * @note The user won't receive this callback event unless he sets to redirect the process of clicking the SHARE button. 
	 * For more details, see \link IMeetingUIElemConfiguration::RedirectClickShareBTNEvent() \endlink.
	 */
	virtual void onStartShareBtnClicked() = 0;
	
	/**
	 * @brief Callback event of clicking the END MEETING button.
	 * @note The user won't receive this callback event unless he sets to redirect the process of clicking the END MEETING button. 
	 * For more details, see \link IMeetingUIElemConfiguration::RedirectClickEndMeetingBTNEvent() \endlink.
	 */
	virtual void onEndMeetingBtnClicked() = 0;
	
	/**
	 * @brief Callback event of clicking PRTICIPANT LIST button.
	 * @note The user won't receive this callback event unless he sets to redirect the process of clicking the PARTICIPANT LIST button. 
	 * For more details, see \link IMeetingUIElemConfiguration::RedirectClickParticipantListBTNEvent() \endlink.
	 */
	virtual void onParticipantListBtnClicked() = 0;
	
	/**
	 * @brief Callback event of clicking CUSTOME LIVE STREAM menu.
	 * @note The user won't receive this callback event unless he sets to redirect the process of clicking the CUSTOME LIVE STREAM menu. 
	 * For more details, see \link IMeetingUIElemConfiguration::RedirectClickCustomLiveStreamMenuEvent() \endlink.
	 */
	virtual void onCustomLiveStreamMenuClicked() = 0;
	
	/**
	 * @brief Notification occurs only when the SDK fails to display the default ZOOM INVITE dialog.
	 */
	virtual void onZoomInviteDialogFailed() = 0;
	
	/**
	 * @brief Callback event of clicking CC menu.
	 * @note The user won't receive this callback event unless he redirects the process of clicking the CUSTOME LIVE STREAM menu. For more details, see \link IMeetingUIElemConfiguration::RedirectClickCCBTNEvent() \endlink .
	 */
	virtual void onCCBTNClicked() = 0;
	
	/**
	 * @brief Callback event for clicking Audio button in the meeting.
	 * @note The user won't receive this callback event unless he sets to redirect the process of clicking the Audio button in the meeting. 
	 * For more details, see \link IMeetingUIElemConfiguration::RedirectClickAudioBTNEvent() \endlink .
	 */
	virtual void onAudioBtnClicked(AudioBtnClickedCallbackInfo info) = 0;
	
	/**
	 * @brief Callback event for clicking Audio Menu button in the meeting.
	 * @note The user won't receive this callback event unless he sets to redirect the process of clicking the Audio Menu button in the meeting. 
	 * For more details, see \link IMeetingUIElemConfiguration::RedirectClickAudioMenuBTNEvent() \endlink .
	 */
	virtual void onAudioMenuBtnClicked() = 0;
	
	/**
	 * @brief Callback event for clicking Breakout Room button in the meeting.
	 * @note The user won't receive this callback event unless he sets to redirect the process of clicking the Breakout Room button in the meeting. 
	 * For more details, see \link IMeetingUIElemConfiguration::RedirectClickBreakoutRoomButtonEvent() \endlink .
	 */
	virtual void onBreakoutRoomBtnClicked() = 0;
	
};
/**
 * @class IMeetingUIController
 * @brief Meeting UI Controller Interface.
 */
class IMeetingUIController
{
public:
	/**
	 * @brief Sets meeting UI controller callback event handler. 
	 * @param pEvent A pointer to the IMeetingUIControllerEvent that receives the meeting user interface event.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetEvent(IMeetingUIControllerEvent* pEvent) = 0;
	
	/**
	 * @brief Show the chat dialog during the meeting.
	 * @param param Specifies the way to show the chat dialog.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ShowChatDlg(ShowChatDlgParam& param) = 0;
	
	/**
	 * @brief Hide the chat dialog during the meeting.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError HideChatDlg() = 0;
	
	/**
	 * @brief Enter full screen display mode.
	 * @param firstView true indicates to enable the full screen mode for the first view.
	 * @param secondView true indicates to enable the full screen mode for the second view if it is in the dual view mode.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EnterFullScreen(bool firstView, bool secondView) = 0;
	
	/**
	 * @brief Exit the full screen display mode.
	 * @param firstView true indicates to exit the full screen mode for the first view.
	 * @param secondView true indicates to exit the full screen mode for the second view if it is in the dual view mode.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ExitFullScreen(bool firstView, bool secondView) = 0;
	
	/**
	 * @brief Active the principal window of meeting and place it on top.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError BackToMeeting() = 0;
	
	/**
	 * @brief Switch to video wall mode. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SwitchToVideoWall() = 0;
	
	/**
	 * @brief Switch to the mode of showing the current speaker.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SwitchToActiveSpeaker() = 0;
	
	/**
	 * @brief Move the floating video window.
	 * @param left Sets the left margin edge for the floating video window. Please use the coordinate of the screen.
	 * @param top Sets the top margin edge for the floating video window. Please use the coordinate of the screen.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError MoveFloatVideoWnd(int left, int top) = 0;
	
	/**
	 * @brief Enables or disable to display the floating sharing toolbar.
	 * @param bShow true indicates to display the floating toolbar.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note This function works only in the share mode. 
	 */
	virtual SDKError ShowSharingToolbar(bool bShow) = 0;
	
	/**
	 * @brief Switch to current speaker mode on the floating window. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SwitchFloatVideoToActiveSpkMod() = 0;
	
	/**
	 * @brief Adjust the display mode of floating window. 
	 * @param type Specify the type of the floating video.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ChangeFloatToActiveSpkVideoSize(SDKFloatVideoType type) = 0;
	
	/**
	 * @brief Switch to gallery view mode on the floating window. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SwitchFloatVideoToGalleryMod() = 0;
	
	/**
	 * @brief Display/hide the window which is used to display the list of the participants. 
	 * @param bShow true indicates to display the list of the participants.
	 * @param [out] hParticipantsListWnd This function will return the window handle if the bShow value is set to true and API calls successfully. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ShowParticipantsListWnd(bool bShow, HWND& hParticipantsListWnd) = 0;
	
	/**
	 * @brief Display/hide the toolbar at the bottom of the meeting window. 
	 * @param bShow true indicates to display the toolbar.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note This function does not work if the user sets to hide the toolbar via IMeetingUIElemConfiguration::SetBottomFloatToolbarWndVisibility().
	 */
	virtual SDKError ShowBottomFloatToolbarWnd(bool bShow) = 0;
	
	/**
	 * @brief Gets the window handle of the meeting user interface.
	 * @param [out] hFirstView If the function succeeds, the parameter will save the window handle of the meeting user interface displayed by the first view.
	 * @param [out] hSecondView If the function succeeds, the parameter will save the window handle of the meeting user interface displayed by the second view.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError GetMeetingUIWnd(HWND& hFirstView, HWND& hSecondView) = 0;
	
	/**
	 * @brief Display the dialog to choose the audio to join the meeting.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ShowJoinAudioDlg() = 0;
	
	/**
	 * @brief Hide the dialog to choose the audio to join the meeting.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError HideJoinAudioDlg() = 0;
	
	/**
	 * @brief Gets the information in video wall mode.
	 * @param [out] videoWallPageInfoParam If the function succeeds, the parameter will save the current page index and the number of the pages.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError GetWallViewPageInfo(VideoWallPageInfoParam& videoWallPageInfoParam) = 0;
	
	/**
	 * @brief Show the video users on previous page or next page in video wall mode.
	 * @param bPageUp true indicates to show the video users on previous page, false next page.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note The function does not work if the window shows the first or last page. The return value is SDKERR_SUCCESS in this case.
	 */
	virtual SDKError ShowPreOrNextPageVideo(bool bPageUp) = 0;
	
	/**
	 * @brief Sets the visibility of the green frame when sharing the desktop.
	 * @param bShow true indicates to display the frame. false hide.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ShowSharingFrameWindows(bool bShow) = 0;
	
	/**
	 * @brief Determines the minimize state of the first view.
	 * @param [out] mode If the function succeeds, the parameter will save the display mode.
	 * @return true if the minimize state. Otherwise, false.
	 */
	virtual bool IsMinimizeModeOfFirstScreenMeetingUIWnd(SDKMinimizeUIMode& mode) = 0;
	
	/**
	 * @brief Change the display mode of the minimized meeting window for the first view.
	 * @param mode Specifies the minimized mode.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SwitchMinimizeUIMode4FirstScreenMeetingUIWnd(SDKMinimizeUIMode mode) = 0;
	
	/**
	 * @brief Gets the information whether the current view supports split screen mode or not. If supports, check it if it is already in the split screen mode.
	 * @param [out] info If the function succeeds, the parameter will save the configuration of split screen mode.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError GetCurrentSplitScreenModeInfo(SplitScreenInfo& info) = 0;
	
	/**
	 * @brief Switch to the split screen mode or cancel.
	 * @param bSplit true indicates to switch to the split screen mode. false cancel.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * \note true does not work if it is in the split screen mode. false does not work if it is not the split screen mode.
	 */
	virtual SDKError SwitchSplitScreenMode(bool bSplit) = 0;
	
	/**
	 * @brief when someone else shares, and meeting window is not full screen. you can call the api to switch video & share display postion. 
	 * @param bToDisplayShare true indicates to display share, otherwise video.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SwapToShowShareViewOrVideo(bool bToDisplayShare) = 0;
	
	/**
	 * @brief Determines if the meeting is displaying the sharing screen now.
	 * @param [out] bIsShare true indicates is showing sharing screen, false means is showing video.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError IsDisplayingShareViewOrVideo(bool& bIsShare) = 0;
	
	/**
	 * @brief Determines if the user can swap to show sharing screen or video now.
	 * @param [out] bCan true indicates Can, otherwise not.
	 * @return SDKERR_SUCCESS means success, otherwise not Otherwise, this function returns an error.
	 */
	virtual SDKError CanSwapToShowShareViewOrVideo(bool& bCan) = 0;
	
	/**
	 * @brief Sets the cloud recording manage url in the recording setting page. 
	 * @param crmURL Specify the cloud recording manage url in the recording setting page.
	 * @return SDKERR_SUCCESS means success, otherwise not Otherwise, this function returns an error.
	 */
	virtual SDKError SetCustomizedCloudRecordingMgrUrl(const zchar_t* crmURL) = 0;
	
	/**
	 * @brief Sets the invitation domain. 
	 * @param invitation_domain Specify the invitation domain.
	 * @return SDKERR_SUCCESS means success, otherwise not Otherwise, this function returns an error.
	 */
	virtual SDKError SetCustomizedInvitationDomain(const zchar_t* invitation_domain) = 0;
	
	/**
	 * @brief Allowing the developer to customize the URL of create/edit the polling. 
	 * @param URL customized URL.
	 * @param bCreate When bCreate is true, it changes the URL of creating a polling. Otherwise, it changes the URL of editing a polling..
	 * @return SDKERR_SUCCESS means success, otherwise not Otherwise, this function returns an error.
	 */
	virtual SDKError SetCustomizedPollingUrl(const zchar_t* URL, bool bCreate) = 0;
	
	/**
	 * @brief Sets the feedback url in the whiteboard page. 
	 * @param feedbackURL Specify the feedback url in the the whiteboard page.
	 * @return SDKERR_SUCCESS means success, otherwise not Otherwise, this function returns an error.
	 */
	virtual SDKError SetCloudWhiteboardFeedbackUrl(const zchar_t* feedbackURL) = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif
