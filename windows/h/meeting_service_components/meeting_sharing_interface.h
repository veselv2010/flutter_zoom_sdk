/**
 * @file meeting_sharing_interface.h
 * @brief Meeting Service Sharing Interface 
 */
#ifndef _MEETING_SHARING_INTERFACE_H_
#define _MEETING_SHARING_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
/**
 * @brief Enumeration of share setting type.
 * Here are more detailed structural descriptions..
 */
enum ShareSettingType
{
	/** Only host can share, the same as "lock share" */
	ShareSettingType_LOCK_SHARE,
	/** Anyone can share, but one sharing only at one moment, and only host can grab other's sharing */
	ShareSettingType_HOST_GRAB, 
	/** Anyone can share, but one sharing only at one moment, and anyone can grab other's sharing */
	ShareSettingType_ANYONE_GRAB,  
	/** Anyone can share, Multi-share can exist at the same time */
	ShareSettingType_MULTI_SHARE,  
	
};

/**
 * @brief Enumeration of audio share mode.
 * Here are more detailed structural descriptions.
 */
enum AudioShareMode
{
	/** Mono mode. */
	AudioShareMode_Mono,		
	/** Stereo mode */
	AudioShareMode_Stereo		
};

/** 
 * @brief Visible shared source information.
 * Here are more detailed structural descriptions..
 */
typedef struct tagZoomSDKSharingSourceInfo
{
	/** User ID. */
	unsigned int userid;			
	/** Share source ID. */
	unsigned int shareSourceID;		
	/** The values of sharing status. */
	SharingStatus status;			
	/** Display or not on the primary view. Available only for Zoom UI mode. */
	bool isShowingInFirstView;		
	/** Display or not on the secondary view. Available only for Zoom UI mode. */
	bool isShowingInSecondView;		
	/** Enable or disable the remote control. */
	bool isCanBeRemoteControl;		
	/** Enable or disable the optimizing video. */
	bool bEnableOptimizingVideoSharing;    

	/** Type of sharing. */
	/** @note In early share status callbacks, for example, Sharing_Self_Send_Begin or Sharing_Other_Share_Begin,
	 *        this value can be SHARE_TYPE_UNKNOWN temporarily. The finalized content type is available after
	 *        onShareContentNotification is received.
	 */
	ShareType contentType;			
	/** Handle of sharing application or white-board. It is invalid unless the value of the eShareType is SHARE_TYPE_AS or SHARE_TYPE_WB. */
	HWND hwndSharedApp;				
	/** The ID of screen to be shared. It is invalid unless the value of the eShareType is SHARE_TYPE_DS. */
	const zchar_t* monitorID;		

	tagZoomSDKSharingSourceInfo()
	{
		userid = 0;
		shareSourceID = 0;
		contentType = SHARE_TYPE_UNKNOWN;
		status = Sharing_Self_Send_Begin;
		isShowingInFirstView = false;
		isShowingInSecondView = false;
		isCanBeRemoteControl = false;
		bEnableOptimizingVideoSharing = false;
		hwndSharedApp = nullptr;
		monitorID = nullptr;
	}
}ZoomSDKSharingSourceInfo;

/**
 * @brief Enumeration of additional type of current sharing sent to others.
 * Here are more detailed structural descriptions.
 */
enum AdvanceShareOption
{
	/** Type of sharing a selected area of desktop. */
	AdvanceShareOption_ShareFrame,
	/** Type of sharing only the computer audio. */
	AdvanceShareOption_PureComputerAudio,
	/** Type of sharing the camera. */
	AdvanceShareOption_ShareCamera,
};

enum MultiShareOption
{
	/** Multi-participants can share simultaneously. */
	Enable_Multi_Share = 0, 
	/** Only host can share at a time. */
	Enable_Only_HOST_Start_Share, 
	/** One participant can share at a time, during sharing only host can start a new sharing and the previous sharing will be replaced. */
	Enable_Only_HOST_Grab_Share, 
	/** One participant can share at a time, during sharing everyone can start a new sharing and the previous sharing will be replaced. */
	Enable_All_Grab_Share, 
};

enum ZoomSDKVideoFileSharePlayError
{
	ZoomSDKVideoFileSharePlayError_None,
	/** Not supported. */
	ZoomSDKVideoFileSharePlayError_Not_Supported,
	/** The resolution is too high to play. */
	ZoomSDKVideoFileSharePlayError_Resolution_Too_High,
	/** Failed to open. */
	ZoomSDKVideoFileSharePlayError_Open_Fail,
	/** Failed to play. */
	ZoomSDKVideoFileSharePlayError_Play_Fail,
	/** Failed to seek. */
	ZoomSDKVideoFileSharePlayError_Seek_Fail
};

#if defined(WIN32)
/**
 * @brief Enumeration of video sharing quality options.
 * Used to control resolution and frame rate when "Optimize for video sharing" is enabled.
 */
enum ZoomSDKVideoSharingQuality
{
	/**
	 * Not set or invalid value.
	 * @note Cannot be used as valid input for SetVideoSharingQuality and CanSetVideoSharingQuality.
	 */
	ZoomSDKVideoSharingQuality_None,

	/** 720p at 30fps quality option. */
	ZoomSDKVideoSharingQuality_720p_30fps,

	/** 720p at 60fps quality option. Requires the device to support high frame rate. */
	ZoomSDKVideoSharingQuality_720p_60fps,

	/** 1080p at 30fps quality option. Requires the meeting to support Full HD. */
	ZoomSDKVideoSharingQuality_1080p_30fps,

	/** 1080p at 60fps quality option. Requires Full HD and device 1080p60 capability. */
	ZoomSDKVideoSharingQuality_1080p_60fps,
};
#endif

/**
 * @class IShareSwitchMultiToSingleConfirmHandler
 * @brief Reminder handler of switching from multi-share to single share.
 */
class IShareSwitchMultiToSingleConfirmHandler
{
public:
	/**
	 * @brief Cancels switching from multi-share to single share. All sharing will remain.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError Cancel() = 0;
	
	/**
	 * @brief Switches from multi-share to single share. All sharing will be stopped.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError Confirm() = 0;

	virtual ~IShareSwitchMultiToSingleConfirmHandler() {};
};

/** 
 * @brief Annotation arrow information.
 * Here are more detailed structural descriptions.
 */
struct ZoomSDKAnnotationArrowInfo
{
	/** x-coordinate, the top-left corner is the coordinate origin.*/
	int posX;
	/** y-coordinate, the top-left corner is the coordinate origin.*/
	int posY;
	/** arrow title. */
	const zchar_t* pTitle;
	/** arrow color. */
	unsigned int arrowColor;
	/** arrow title color. */
	unsigned int titleColor;
	/** transparency. */
	unsigned short alpha;

	ZoomSDKAnnotationArrowInfo()
	{
		posX = 0;
		posY = 0;
		pTitle = nullptr;
		arrowColor = 0;
		titleColor = 0;
		alpha = 0;
	}
};

enum ZoomSDKRemoteControlMouseEventType
{
	/** Mouse movement event. */
	ZoomSDKRemoteControlMouseEventType_Move,
	/** Left mouse button press event. */
	ZoomSDKRemoteControlMouseEventType_Left_Down,
	/** Left mouse button release event. */
	ZoomSDKRemoteControlMouseEventType_Left_Up
};

/**
 * @brief mouse information.
 * Here are more detailed structural descriptions.
 */
struct ZoomSDKRemoteControlMouseInfo
{
	/** mouse event type. */
	ZoomSDKRemoteControlMouseEventType eventType;
	/** x-coordinate, the top-left corner is the coordinate origin.*/
	int x;
	/** y-coordinate, the top-left corner is the coordinate origin.*/
	int y;
};

/**
 * @class IMeetingShareCtrlEvent
 * @brief Callback event of meeting share controller.
 */
class IMeetingShareCtrlEvent
{
public:
	virtual ~IMeetingShareCtrlEvent() {}
	
	/**
	 * @brief Callback event of the changed sharing status. 
	 * @param shareInfo Sharing information.
	 * @note The userId changes according to the status value. When the status value is the Sharing_Self_Send_Begin or Sharing_Self_Send_End, the userId is the user own ID. Otherwise, the value of userId is the sharer ID.
	 * @note When status is Sharing_Self_Send_Begin or Sharing_Other_Share_Begin, shareInfo.contentType can be
	 *       SHARE_TYPE_UNKNOWN temporarily. Do not treat UNKNOWN at this stage as the finalized share content type.
	 *       Wait for onShareContentNotification to get the finalized content type.
	 */
	virtual void onSharingStatus(ZoomSDKSharingSourceInfo shareInfo) = 0;
	
	/**
	 * @brief Callback event of failure to start sharing. 
	 */
	virtual void onFailedToStartShare() = 0;
	
	/**
	 * @brief Callback event of changed sharing information.
	 * @param shareInfo Sharing information.
	 */
	virtual void onShareContentNotification(ZoomSDKSharingSourceInfo shareInfo) = 0;
	
	/**
	 * @brief Callback event of switching multi-participants share to one participant share.
	 * @param handler_ An object pointer used by user to complete all the related operations.
	 */
	virtual void onMultiShareSwitchToSingleShareNeedConfirm(IShareSwitchMultiToSingleConfirmHandler* handler_) = 0;
	
	/**
	 * @brief Callback event of sharing setting type changed.
	 * @param type Sharing setting type.
	 */
	virtual void onShareSettingTypeChangedNotification(ShareSettingType type) = 0;
	
	/**
	 * @brief Callback event of the shared video's playback has completed.
	 */
	virtual void onSharedVideoEnded() = 0;
	
	/**
	 * @brief Callback event of the video file playback error.
	 * @param error The error type.
	 */
	virtual void onVideoFileSharePlayError(ZoomSDKVideoFileSharePlayError error) = 0;
	
	/**
	 * @brief Callback event of the changed optimizing video status.
	 * @param shareInfo Sharing information.
	 */
	virtual void onOptimizingShareForVideoClipStatusChanged(ZoomSDKSharingSourceInfo shareInfo) = 0;

#if defined(WIN32)
	/**
	 * @brief Callback when the video sharing quality has changed.
	 * @param quality The new quality in effect.
	 * @note Triggered only when the effective quality actually changes.
	 */
	virtual void onVideoSharingQualityChanged(ZoomSDKVideoSharingQuality quality) = 0;
#endif

	/**
	 * @brief Callback event when annotation arrow information changes.
	 * @param arrowInfo arrow information.
	 */
	virtual void onAnnotationArrowInfoChanged(const ZoomSDKAnnotationArrowInfo& arrowInfo) {}

	/**
	* @brief Callback event when remote control mouse information changes.
	* @param mouseInfo mouse information.
	*/
	virtual void onRemoteControlMouseInfoChanged(const ZoomSDKRemoteControlMouseInfo& mouseInfo) {}
};

/**
 * @class IMeetingShareController
 * @brief Meeting share controller interface.
 */
class IMeetingShareController
{
public:
	/**
	 * @brief Sets meeting share controller callback event handler.
	 * @param pEvent A pointer to the IMeetingShareCtrlEvent that receives sharing event. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetEvent(IMeetingShareCtrlEvent* pEvent) = 0;
	
	/**
	 * @brief Shares the specified application.
	 * @param hwndSharedApp The window handle of the application to be shared. If the hwndSharedApp can't be shared, the return value is the SDKERR_INVALID_PARAMETER error code. If the hwndSharedApp is nullptr, the primary monitor will be shared. 
	*   \if WINDOWS
     *   The window handle to share.
     *   \endif
     *   \if LINUX
     *   The device name. Format:
     *   `hostname:display_number-screen_number(x, y, width, height)-app_id`
     *   e.g. `:0-0(0,0,1920,1080)-34563456`
     *   - display_name: `hostname:display_number`
     *   - screen_number: virtual X11 device (default 0)
     *   - device_frame: `(x, y, width, height)`
     *   - app_id: `app_id`
     *   \endif
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError StartAppShare(HWND hwndSharedApp) = 0;
	
	/**
	 * @brief Shares the specified monitor.
	 * @param monitorID The monitor ID to be shared. You may get the value via EnumDisplayMonitors System API. If the monitorID is nullptr, the primary monitor will be shared. For more details, see szDevice in MONITORINFOEX structure.
	 *   \if WINDOWS
     *   The screen name to share.
     *   \endif
     *   \if LINUX
     *   The device name. Format:
     *   `hostname:display_number-screen_number(x, y, width, height)-winid`
     *   e.g. `:0-0(0,0,1920,1080)-34563456`
     *   - display_name: `hostname:display_number`
     *   - screen_number: virtual X11 device (default 0)
     *   - device_frame: `(x, y, width, height)`
     *   - window_id: `winid`
     *   \endif
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError StartMonitorShare(const zchar_t* monitorID) = 0;
#if defined(WIN32)
	/**
	 * @brief Determines whether the window handle can be shared. If the hwndSharedApp is nullptr, the return value is false.
	 * @param hwndSharedApp The window handle to validate.
	 * @return true if the window handle can be shared. Otherwise, false.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual bool IsShareAppValid(HWND hwndSharedApp) = 0;
	/**
	 * @brief A dialog box pops up that enable the user to choose the application or window to share.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for ZOOM style mode. 
	 */
	virtual SDKError ShowSharingAppSelectWnd() = 0;
	
	/**
	 * @brief Starts sharing with mobile device. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError StartAirPlayShare() = 0;
	
	/**
	 * @brief Starts sharing camera.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for ZOOM style only.	
	 */
	virtual SDKError StartShareCamera() = 0;
	
	/**
	 * @brief Block the window when sharing in full screen.
	 * Once the function is called, you need to redraw the window to take effect.
	 * @param bBlock true indicates to block the window when sharing in full screen.
	 * @param hWnd Specify the window to be blocked.
	 * @param bChangeWindowStyle If it is false, please call this function either after the StartMonitorShare is called or when you get the callback event of the onSharingStatus with Sharing_Self_Send_Begin. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for ZOOM style user interface mode.
	 * It is not suggested to use this function for it will change the property of the window and leads to some unknown errors.
	 * It won't work until the IMeetingShareController::StartMonitorShare() is called if the bChangeWindowStyle is set to false. 
	 * If you want to use the specified window during the share, you need to redraw the window.
	 * Set the bBlock to false before ending the share and call the function for the specified window to resume the property of the window.
	 */
	virtual SDKError BlockWindowFromScreenshare(bool bBlock, HWND hWnd, bool bChangeWindowStyle = true) = 0;
	
	/**
	 * @brief Switch to auto-adjust mode from sharing window by the function when watching the share on the specified view.
	 * @param type Specify the view you want to set, either primary or secondary.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for ZOOM style user interface mode.
	 */
	virtual SDKError SwitchToFitWindowModeWhenViewShare(SDKViewType type) = 0;
	
	/**
	 * @brief Switch the window size by the function when watching the share on the specified view.
	 * @param shareSourceID Specify the share scource ID that you want to switch zoom ratio.
	 * @param shareViewZoomRatio Specify the size you want to set.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SwitchZoomRatioWhenViewShare(unsigned int shareSourceID, SDKShareViewZoomRatio shareViewZoomRatio) = 0;
	
	/**
	 * @brief Enables follow presenter's pointer by the function when watching the share on the specified view.
	 * @param shareSourceID Specify the sharing source ID that you want to follow the presenter's pointer. 
	 * @param bEnable true indicates to enable. false not.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EnableFollowPresenterPointerWhenViewShare(unsigned int shareSourceID, bool bEnable) = 0;
	
	/**
	 * @brief Determines if the follow presenter's pointer can be enabled when watching the share on the specified view.
	 * @param shareSourceID Specify the share scource ID that you want to follow his pointer. 
	 * @param [out] bCan true indicates that the pointer can be enabled. false indicates that it can't.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise it fails.
	 */
	virtual SDKError CanEnableFollowPresenterPointerWhenViewShare(unsigned int shareSourceID, bool& bCan) = 0;
	
	/**
	 * @brief Starts sharing with White board.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * You need to draw your own annotation bar for custom mode when you get the onShareContentNotification with SHARE_TYPE_WB.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError StartWhiteBoardShare() = 0;
	
	/**
	 * @brief Starts sharing frame.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError StartShareFrame() = 0;
	
	/**
	 * @brief Starts sharing only the computer audio.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.	
	 */
	virtual SDKError StartSharePureComputerAudio() = 0;
	/**
	 * @brief Starts sharing camera.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for user custom interface mode only.	
	 */
	virtual SDKError StartShareCamera(const zchar_t* deviceID, HWND hWnd) = 0;
	
	/**
	 * @brief Display the dialog of sharing configuration.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for ZOOM style user interface mode.
	 */
	virtual SDKError ShowShareOptionDialog() = 0;
#endif

	/**
	 * @brief Determines if the specified ADVANCE SHARE OPTION is supported. 
	 * @param option_ The ADVANCE SHARE OPTION to be determined.
	 * @return If it is supported, the return value is SDKERR_SUCCESS. Otherwise not.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError IsSupportAdvanceShareOption(AdvanceShareOption option_) = 0;
	
	/**
	 * @brief Stops the current sharing.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError StopShare() = 0;
	
	/**
	 * @brief Pause the current sharing.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError PauseCurrentSharing() = 0;
	
	/**
	 * @brief Resume the current sharing.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError ResumeCurrentSharing() = 0;
	
	/**
	 * @brief Gets the ID of users who are sharing
	 * @return If the function succeeds, the return value is a list of user ID of all users who are sharing. If the function fails, the return value is nullptr.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual IList<unsigned int>* GetViewableSharingUserList() = 0;
	
	/**
	 * @brief Gets the sharing source information list from the specified sharer.
	 * @param userID The ID of the user who is sharing.
	 * @return If the function succeeds, it returns the viewable sharing information list.
	 * @note Valid for both ZOOM style and user custom interface mode. 
	 * For custom interface mode, this interface is only valid after subscribing the sharing content from the specified user by calling ICustomizedShareRender::SetUserID(unsigned int userid) successfully.
	 */
	virtual IList<ZoomSDKSharingSourceInfo>* GetSharingSourceInfoList(unsigned int userID) = 0;
	
	/**
	 * @brief Determines whether the current meeting can start sharing. 
	 * @param [out] reason The reason that no one can start sharing.
	 * @return true indicates you can start sharing.
	 */
	virtual bool CanStartShare(CannotShareReasonType& reason) = 0;
	
	/**
	 * @brief Determines if it is able to share desktop in the current meeting.
	 * @return true if it is able to share desktop in the current meeting. Otherwise, false.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual bool IsDesktopSharingEnabled() = 0;
	
	/**
	 * @brief Determines if the sharing is locked. 
	 * @param bLocked true indicates that the sharing is locked. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError IsShareLocked(bool& bLocked) = 0;
	
	/**
	 * @brief Determines if the specified share type supports sharing with compute sound or not.
	 * @param type The type of sharing content.
	 * @return true indicates that this is supported.
	 */
	virtual bool IsSupportShareWithComputerSound(ShareType type) = 0;
	
	/**
	 * @brief Determines if the current share supports sharing with compute sound or not.
	 * @return true indicates that is supported.
	 */
	virtual bool IsCurrentSharingSupportShareWithComputerSound() = 0;
	
	/**
	 * @brief Determines if the current meeting enabled sharing with compute sound or not before sharing. 
	 * @return true indicates that this is enabled.
	 */
	virtual bool IsEnableShareComputerSoundOn() = 0;
	
	/**
	 * @brief Enables or disable the computer audio before sharing.
	 * @param bEnable true indicates to enable. false not.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note It will be applied when starting share.
	 */
	virtual SDKError EnableShareComputerSound(bool bEnable) = 0;
	
	/**
	 * @brief Determines if the current sharing content enabled sharing with compute sound or not. 
	 * @return true indicates that this is enabled.
	 */
	virtual bool IsEnableShareComputerSoundOnWhenSharing() = 0;
	
	/**
	 * @brief Sets to enable or disable the computer audio when sharing.
	 * @param bEnable true indicates to enable. false not.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError EnableShareComputerSoundWhenSharing(bool bEnable) = 0;
	
	/**
	 * @brief Sets the audio share mode before or during sharing. 
	 * @param mode The mode for audio share.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError SetAudioShareMode(AudioShareMode mode) = 0;
	
	/**
	 * @brief Gets the audio share mode. 
	 * @param mode The mode for audio share, see \link AudioShareMode \endlink enum.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError GetAudioShareMode(AudioShareMode& mode) = 0;
	
	/**
	 * @brief Determines if the current meeting supports sharing with optimize video or not.
	 * @return true indicates this is supported.
	 */
	virtual bool IsSupportEnableOptimizeForFullScreenVideoClip() = 0;
	
	/**
	 * @brief Determines if the current meeting enabled sharing with optimize video or not.
	 * @return true indicates this is enabled.
	 */
	virtual bool IsEnableOptimizeForFullScreenVideoClipOn() = 0;
	
	/**
	 * @brief Enables or disable the video optimization before sharing. 
	 * @param bEnable true indicates to enable. false not.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note It will be applied when starting share.
	 */
	virtual SDKError EnableOptimizeForFullScreenVideoClip(bool bEnable) = 0;
	
	/**
	 * @brief Determines if the current sharing content enabled sharing with optimize video or not.
	 * @return true indicates this is enabled.
	 */
	virtual bool IsEnableOptimizeForFullScreenVideoClipOnWhenSharing() = 0;
	
	/**
	 * @brief Enables or disable the video optimization when sharing. 
	 * @param bEnable true indicates to enable. false not.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError EnableOptimizeForFullScreenVideoClipWhenSharing(bool bEnable) = 0;
	
	/**
	 * @brief Sets the options for multi-participants share.
	 * @param [in] shareOption New options for sharing.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetMultiShareSettingOptions(MultiShareOption shareOption) = 0;
	
	/**
	 * @brief Gets the options for multi-participants share.
	 * @param [out] shareOption Options for sharing.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError GetMultiShareSettingOptions(MultiShareOption& shareOption) = 0;

#if defined(WIN32)
	/**
	 * @brief Checks if the specified quality can be set for the given share type.
	 * @param quality The video sharing quality to check. Cannot be ZoomSDKVideoSharingQuality_None.
	 * @param shareType The type of sharing content. See \link ShareType \endlink enum.
	 * @return true if supported, otherwise false.
	 * @note Returns false unless either EnableOptimizeForFullScreenVideoClip(true)
	 *       has been called, or optimize-for-video-sharing is currently on during an active share.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual bool CanSetVideoSharingQuality(ZoomSDKVideoSharingQuality quality, ShareType shareType) = 0;

	/**
	 * @brief Sets the video sharing quality.
	 * @param quality The video sharing quality to set. Cannot be ZoomSDKVideoSharingQuality_None.
	 * @return SDKERR_SUCCESS if the function succeeds, otherwise an error code.
	 * @note Can be called before or during sharing.
	 *       - Before sharing: device capability is validated and the quality is cached.
	 *       - During sharing: device and share-type constraints are validated and applied immediately.
	 * @note The actual quality may be automatically downgraded. When it changes,
	 *       onVideoSharingQualityChanged is triggered.
	 * @note Returns SDKERR_WRONG_USAGE unless either EnableOptimizeForFullScreenVideoClip(true)
	 *       has been called, or optimize-for-video-sharing is currently on during an active share.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual SDKError SetVideoSharingQuality(ZoomSDKVideoSharingQuality quality) = 0;

	/**
	 * @brief Gets the current video sharing quality.
	 * @return The current quality. Returns ZoomSDKVideoSharingQuality_None if not set,
	 *         or if optimize-for-video-sharing is off in both the pre-share flag and
	 *         the current in-sharing effective state.
	 * @note Before sharing returns the cached value; during sharing returns the quality actually in effect.
	 * @note Valid for both ZOOM style and user custom interface mode.
	 */
	virtual ZoomSDKVideoSharingQuality GetVideoSharingQuality() = 0;
#endif

	/**
	 * @brief Determines whether can switch to next camera, when share camera.
	 * @param [Out] bCan, if bCan is true it means you can switch, else can not.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError CanSwitchToShareNextCamera(bool& bCan) = 0;
	
	/**
	 * @brief switch to next camera, when you are sharing the camera.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SwitchToShareNextCamera() = 0;
	
	/**
	 * @brief Determines whether the user can share video files.
	 * @return true indicates that the user can share video files. Otherwise, false.
	 */
	virtual bool CanShareVideoFile() = 0;

#if defined(WIN32)
	/**
	 * @brief Determines whether the user can share to the breakout room.
	 * @return true indicates that the user can share to the breakout room. Otherwise, this function returns an error.
	 * @note Valid for user custom interface mode only.	
	 */
	virtual SDKError CanEnableShareToBO(bool& bCan) = 0;
	
	/**
	 * @brief Sets to enable sharing to the breakout room. 
	 * @param bEnable true indicates to enable. false indicates that sharing to the breakout room is not enabled.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for user custom interface mode only.	
	 */
	virtual SDKError EnableShareToBO(bool bEnable) = 0;
	
	/**
	 * @brief Determines if sharing to the breakout room is enabled. 
	 * @param bEnabled true indicates that the sharing is locked. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid for user custom interface mode only.	
	 */
	virtual SDKError IsShareToBOEnabled(bool& bEnabled) = 0;
	
	/**
	 * @brief Share the video file.
	 * @param filePath Specify the video file path. Only supports mov, mp4, or avi format.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error and returns error.
	 */
	virtual SDKError StartVideoFileShare(const zchar_t* filePath) = 0;

	/**
	 * @brief Determines whether the legal notice for white board is available
	 * @return true indicates the legal notice for white board is available. Otherwise, false.
	 */
	virtual bool IsWhiteboardLegalNoticeAvailable() = 0;
	
	/**
	 * @brief Gets the white board legal notices prompt.
	 */
	virtual const zchar_t* getWhiteboardLegalNoticesPrompt() = 0;
	
	/**
	 * @brief Gets the white board legal notices explained.
	 */
	virtual const zchar_t* getWhiteboardLegalNoticesExplained() = 0;
#endif
};
END_ZOOM_SDK_NAMESPACE
#endif
