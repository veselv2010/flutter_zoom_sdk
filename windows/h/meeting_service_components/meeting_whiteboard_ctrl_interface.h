/**
 * @file meeting_whiteboard_ctrl_interface.h
 * @brief Meeting Service White Board Interface.
 */
#ifndef _MEETING_WHITEBOARD_CTRL_INTERFACE_H_
#define _MEETING_WHITEBOARD_CTRL_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/**
 * @brief Whiteboard who can share setting type.
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
 * @brief Whiteboard who can initiate new whiteboard setting type.
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
 * @brief Status of whiteboard.
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
	 * @param status Specify current whiteboard status. For more details, see \link SDKWhiteboardStatus \endlink enum.
	 */
	virtual void onWhiteboardStatusChanged(SDKWhiteboardStatus status) = 0;
	
	/**
	 * @brief Callback event of whiteboard setting type changed.
	 * @param shareOption Who can share their whiteboard. For more details, see \link SDKWhiteboardShareOption \endlink enum.
	 * @param createOption Who can create a new whiteboard. For more details, see \link SDKWhiteboardCreateOption \endlink enum.
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
	 * @param pEvent An object pointer to the IMeetingWhiteboardControllerEvent that receives the meeting whiteboard callback event. For more details, see \link IMeetingWhiteboardControllerEvent \endlink.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 * @note The SDK use pEvent to transmit the callback event to the user's application. If the function is not called or fails, the user's application is unable to retrieve the callback event.
	 */
	virtual SDKError SetEvent(IMeetingWhiteboardCtrlEvent* pEvent) = 0;
	
	/**
	 * @brief Set the whiteboard web view and dashboard web view owner window.
	 * @param [in] hOwner Specify the owner window. If the hOwner is nullptr, the owner window will be desktop window. 
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for user custom interface mode. Call this function before calling ShowDashboardView. 
	 * Call this function when receiving the meeting status MEETING_STATUS_INMEETING. Otherwise, the dashboard web view owner window will be desktop window. 
	 */
	virtual SDKError SetOwnerWindow(HWND hOwner) = 0;
	
	/**
	 * @brief Determine whether the current meeting supports the whiteboard or not.
	 * @return True indicates to support.
	 */
	virtual bool IsSupportWhiteBoard() = 0;
	
	/**
	 * @brief Determine whether the current meeting can start sharing the whiteboard. 
	 * @param [out] reason The reason that no one can start sharing the whiteboard. See \link CannotShareReasonType \endlink enum.
	 * @return True indicates you can start sharing the whiteboard.
	 */
	virtual bool CanStartShareWhiteboard(CannotShareReasonType& reason) = 0;
	
	/**
	 * @brief Show the dashboard web view window.
	 * @param x: The horizontal coordinate value. 
	 * @param y: The vertical coordinate value. 
	 * @note If x == -1 and y = -1, the window position will be in the center of the screen where the owner window is located.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for custom user interface mode.
	 */
	virtual SDKError ShowDashboardView(int x = -1, int y = -1) = 0;
	
	/**
	 * @brief Set the dashboard web view window size.
	 * @param height Specify the height of the window. 
	 * @param width Specify the width of the window. 
	 * @note If height == -1 and width == -1, the window size will be default.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for custom user interface mode.
	 */
	virtual SDKError SetDashboardViewSize(int width = -1, int  height = -1) = 0;
	
	/**
	 * @brief Set the whiteboard web view window position.
	 * @param x: The horizontal coordinate value. 
	 * @param y: The vertical coordinate value.
	 * @note If x == -1 and y = -1, the window position will be in the center of the screen where the owner window is located.
	 * @note If you don't call this function, the window position will be in the center of the screen where the owner window is located.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for custom user interface mode.
	 */
	virtual SDKError SetWhiteboardViewPos(int x = -1, int y = -1) = 0;
	
	/**
	 * @brief Set the whiteboard web view window size.
	 * @param height Specify the height of the window. 
	 * @param width Specify the width of the window. 
	 * @note If height == -1 and width == -1, the window size will be default.
	 * @note If you don't call this function, the window size will be default.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 * @note Valid only for custom user interface mode.
	 */
	virtual SDKError SetWhiteboardViewSize(int width = -1, int height = -1) = 0;
	
	/**
	 * @brief Set the option for who can share a whiteboard.
	 * @param [in] option New setting for who can share a whiteboard, see \link SDKWhiteboardShareOption \endlink enum.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 */
	virtual SDKError SetWhiteboardShareOption(SDKWhiteboardShareOption option) = 0;
	
	/**
	 * @brief Get the option for who can share a whiteboard.
	 * @param [out] option Setting option for who can share a whiteboard, see \link SDKWhiteboardShareOption \endlink enum.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 */
	virtual SDKError GetWhiteboardShareOption(SDKWhiteboardShareOption& option) = 0;
	
	/**
	 * @brief Set the option for who can initiate a new whiteboard.
	 * @param [in] option Setting option for who can initiate a new whiteboard, see \link SDKWhiteboardCreateOption \endlink enum.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 */
	virtual SDKError SetWhiteboardCreateOption(SDKWhiteboardCreateOption option) = 0;
	
	/**
	 * @brief Get the option for who can initiate a new whiteboard.
	 * @param [out] option Setting option for who can initiate a new whiteboard, see \link SDKWhiteboardCreateOption \endlink enum.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 */
	virtual SDKError GetWhiteboardCreateOption(SDKWhiteboardCreateOption& option) = 0;
	
	/**
	 * @brief Enable the participants to create a new whiteboard without the host in the meeting. 
	 * @param bEnable TRUE indicates to enable. FALSE not.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	 */
	virtual SDKError EnableParticipantsCreateWithoutHost(bool bEnable) = 0;
	
	/**
	 * @brief Determine whether enable the participants create a new whiteboard without the host in the meeting.
	 * @return True indicates that they have these permission.
	 */
	virtual bool IsParticipantsCreateWithoutHostEnabled() = 0;
	
	/**
	 * @brief Query if other user is sharing whiteboard.
	 * @return True means other user is sharing. False means not.
	 */
	virtual bool IsOtherSharingWhiteboard() = 0;
	
	/**
	 * @brief Query if the current user is sharing whiteboard successfully.
	 * @return True means the current user is sharing successfully. False means not.
	 */
	virtual bool IsSharingWhiteboardOut() = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif