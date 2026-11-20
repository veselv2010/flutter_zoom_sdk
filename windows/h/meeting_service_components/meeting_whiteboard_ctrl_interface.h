/**
 * @file meeting_whiteboard_ctrl_interface.h
 * @brief Meeting Service White Board Interface.
 */
#ifndef _MEETING_WHITEBOARD_CTRL_INTERFACE_H_
#define _MEETING_WHITEBOARD_CTRL_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/**
 * @brief Enumeration of whiteboard who can share setting type.
 */
typedef enum 
{    
	/** Only the host can share a whiteboard. */
	SDKWhiteboardShareOption_HostShare,   
	/** Anyone can share a whiteboard, but only one can share at a time, and only the host can take another's sharing role. */
	SDKWhiteboardShareOption_HostGrabShare, 
	/** Anyone can share a whiteboard, but only one can share at a time, and anyone can take another's sharing role. */
	SDKWhiteboardShareOption_AllGrabShare      
}SDKWhiteboardShareOption;

/** 
 * @brief Enumeration of whiteboard who can initiate new whiteboard setting type.
 */
typedef enum 
{
	/** Only the host can initiate a new whiteboard. */
	SDKWhiteboardCreateOption_HostOnly,   
	/** Users under the same account as the meeting owner can initiate a new whiteboard. */
	SDKWhiteboardCreateOption_AccountUsers,   
	/** All participants can initiate a new whiteboard. */
	SDKWhiteboardCreateOption_All               
} SDKWhiteboardCreateOption;

/**
 * @brief Enumeration of whiteboard status.
 */
typedef enum 
{
	/**  User stared sharing their whiteboard. */
	SDKWhiteboardStatus_Started, 
	/**  User stopped sharing their whiteboard. */
	SDKWhiteboardStatus_Stopped, 
}SDKWhiteboardStatus;

/**
 * @class IMeetingWhiteboardCtrlEvent
 * @brief Whiteboard controller callback event.
 */
class IMeetingWhiteboardCtrlEvent
{
public:
	virtual ~IMeetingWhiteboardCtrlEvent() {}
	
	/**
	 * @brief Whiteboard status changed callback. Use this function to inform the user that the whiteboard has been started or stopped, and all users in the meeting can get the event.
	 * @param status Specify current whiteboard status.
	 */
	virtual void onWhiteboardStatusChanged(SDKWhiteboardStatus status) = 0;
	
	/**
	 * @brief Callback event of whiteboard setting type changed.
	 * @param shareOption Who can share their whiteboard.
	 * @param createOption Who can create a new whiteboard.
	 * @param bEnable Whether enable the participants create whiteboard without host in the meeting.
	 */
	virtual void onWhiteboardSettingsChanged(SDKWhiteboardShareOption shareOption, SDKWhiteboardCreateOption createOption, bool bEnable) = 0;
};

/**
 * @class IMeetingWhiteboardController
 * @brief Whiteboard controller interface.
 */
class IMeetingWhiteboardController
{
public:
	/**
	 * @brief Configure the meeting whiteboard controller callback event handler.
	 * @param pEvent An object pointer to the IMeetingWhiteboardControllerEvent that receives the meeting whiteboard callback event.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note The SDK use pEvent to transmit the callback event to the user's application. If the function is not called or fails, the user's application is unable to retrieve the callback event.
	 */
	virtual SDKError SetEvent(IMeetingWhiteboardCtrlEvent* pEvent) = 0;
	
	/**
	 * @brief Sets the whiteboard web view and dashboard web view owner window.
	 * @param [in] hOwner Specify the owner window. If the hOwner is nullptr, the owner window will be desktop window. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for user custom interface mode. Call this function before calling ShowDashboardView. 
	 * Call this function when receiving the meeting status MEETING_STATUS_INMEETING. Otherwise, the dashboard web view owner window will be desktop window. 
	 */
	virtual SDKError SetOwnerWindow(HWND hOwner) = 0;
	
	/**
	 * @brief Determines whether the current meeting supports the whiteboard or not.
	 * @return true indicates to support.
	 */
	virtual bool IsSupportWhiteBoard() = 0;
	
	/**
	 * @brief Determines whether the current meeting can start sharing the whiteboard. 
	 * @param [out] reason The reason that no one can start sharing the whiteboard.
	 * @return true indicates you can start sharing the whiteboard.
	 */
	virtual bool CanStartShareWhiteboard(CannotShareReasonType& reason) = 0;

	/**
	 * @brief Determines whether the current user can create a new whiteboard.
	 * @return true if the current user can create a new whiteboard. Otherwise, this function returns false.
	 */
	virtual bool CanStartShareNewWhiteboard() = 0;

	/**
	 * @brief Starts sharing a new whiteboard.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for custom user interface mode.
	 */
	virtual SDKError StartNewWhiteboardShare() = 0;

	/**
	 * @brief Determines whether the current user can stop sharing a new whiteboard.
	 * @return true if the current user can stop sharing the new whiteboard. Otherwise, this function returns false.
	 */
	virtual bool CanStopWhiteboardShare() = 0;

	/**
	 * @brief When self sharing whiteboard, stop self; when self not sharing, stop all others.
	 * @return If the function succeeds, it returns SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for user custom interface mode.
	 */
	virtual SDKError StopWhiteboardShare() = 0;

	/**
	 * @brief Shows the dashboard web view window.
	 * @param x: The horizontal coordinate value. 
	 * @param y: The vertical coordinate value. 
	 * @note If x == -1 and y = -1, the window position will be in the center of the screen where the owner window is located.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for custom user interface mode.
	 */
	virtual SDKError ShowDashboardView(int x = -1, int y = -1) = 0;
	
	/**
	 * @brief Sets the dashboard web view window size.
	 * @param height Specify the height of the window. 
	 * @param width Specify the width of the window. 
	 * @note If height == -1 and width == -1, the window size will be default.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for custom user interface mode.
	 */
	virtual SDKError SetDashboardViewSize(int width = -1, int  height = -1) = 0;
	
	/**
	 * @brief Sets the whiteboard web view window position.
	 * @param x: The horizontal coordinate value. 
	 * @param y: The vertical coordinate value.
	 * @note If x == -1 and y = -1, the window position will be in the center of the screen where the owner window is located.
	 * @note If you don't call this function, the window position will be in the center of the screen where the owner window is located.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for custom user interface mode.
	 */
	virtual SDKError SetWhiteboardViewPos(int x = -1, int y = -1) = 0;
	
	/**
	 * @brief Sets the whiteboard web view window size.
	 * @param height Specify the height of the window. 
	 * @param width Specify the width of the window. 
	 * @note If height == -1 and width == -1, the window size will be default.
	 * @note If you don't call this function, the window size will be default.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for custom user interface mode.
	 */
	virtual SDKError SetWhiteboardViewSize(int width = -1, int height = -1) = 0;
	
	/**
	 * @brief Sets the option for who can share a whiteboard.
	 * @param [in] option New setting for who can share a whiteboard.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetWhiteboardShareOption(SDKWhiteboardShareOption option) = 0;
	
	/**
	 * @brief Gets the option for who can share a whiteboard.
	 * @param [out] option Setting option for who can share a whiteboard.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError GetWhiteboardShareOption(SDKWhiteboardShareOption& option) = 0;
	
	/**
	 * @brief Sets the option for who can initiate a new whiteboard.
	 * @param [in] option Setting option for who can initiate a new whiteboard.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetWhiteboardCreateOption(SDKWhiteboardCreateOption option) = 0;
	
	/**
	 * @brief Gets the option for who can initiate a new whiteboard.
	 * @param [out] option Setting option for who can initiate a new whiteboard.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError GetWhiteboardCreateOption(SDKWhiteboardCreateOption& option) = 0;
	
	/**
	 * @brief Enables the participants to create a new whiteboard without the host in the meeting. 
	 * @param bEnable true indicates to enable. false not.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @deprecated This interface is no longer applicable.
	 */
	virtual SDKError EnableParticipantsCreateWithoutHost(bool bEnable) = 0;
	
	/**
	 * @brief Determines whether enable the participants create a new whiteboard without the host in the meeting.
	 * @return true indicates that they have these permission.
	 * @deprecated This interface is no longer applicable.
	 */
	virtual bool IsParticipantsCreateWithoutHostEnabled() = 0;
	
	/**
	 * @brief Query if other user is sharing whiteboard.
	 * @return true if other user is sharing. Otherwise, false.
	 */
	virtual bool IsOtherSharingWhiteboard() = 0;
	
	/**
	 * @brief Query if the current user is sharing whiteboard successfully.
	 * @return true if the current user is sharing successfully. Otherwise, false.
	 */
	virtual bool IsSharingWhiteboardOut() = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif