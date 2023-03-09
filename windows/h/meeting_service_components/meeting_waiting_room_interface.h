/*!
* \file meeting_waiting_room_interface.h
* \brief Meeting Service Waiting Room Interface.
* Valid only for Zoom style user interface mode.
*/
#ifndef _MEETING_WaitingRoom_INTERFACE_H_
#define _MEETING_WaitingRoom_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/*! \enum WaitingRoomLayoutType
	\brief WaitingRoom LayoutType.
	Here are more detailed structural descriptions.
*/
enum WaitingRoomLayoutType
{
	WaitingRoomLayoutType_Default,
	WaitingRoomLayoutType_Logo,
	WaitingRoomLayoutType_Video
};

/*! \enum CustomWaitingRoomDataStatus
	\brief Download Status of CustomWaitingRoomData.
	Here are more detailed structural descriptions.
*/
enum CustomWaitingRoomDataStatus
{
	CustomWaitingRoomDataStatus_Init,
	CustomWaitingRoomDataStatus_Downloading,
	CustomWaitingRoomDataStatus_Download_OK,
	CustomWaitingRoomDataStatus_Download_Failed
};

/*! \struct WaitingRoomCustomizeData
	\brief The WaitingRoom Customize Data Info.
	Here are more detailed structural descriptions..
*/
typedef struct CustomWaitingRoomData_s
{
	CustomWaitingRoomData_s()
		:type(WaitingRoomLayoutType_Default)
		, status(CustomWaitingRoomDataStatus_Init)
	{
		title = NULL;
		description = NULL;
		logo_path = NULL;
		video_path = NULL;
	}
	const wchar_t* title;
	const wchar_t* description;
	const wchar_t* logo_path;
	const wchar_t* video_path;
	WaitingRoomLayoutType type;
	CustomWaitingRoomDataStatus status;
}CustomWaitingRoomData;

/// \brief handler for download waitingRoom Customize Data if download fail.
class IWaitingRoomDataDownloadHandler
{
public:
	virtual ~IWaitingRoomDataDownloadHandler() {}

	/// \brief Retry to Download the WaitingRoom CustomizeData information in the waiting room.
	/// \return True indicates to Retry success.
	virtual bool Retry() = 0;

	/// \brief Ignore to GDownloadet the WaitingRoom CustomizeData information in the waiting room.
	virtual void Ignore() = 0;
};

/// \brief Meeting Waiting Room Callback Event.
///
class IMeetingWaitingRoomEvent
{
public:
	virtual ~IMeetingWaitingRoomEvent() {}

	/// \brief Callback event of notification that user joins the waiting room.
	/// \param userID The ID of user who joins the waiting room. 
	virtual void onWatingRoomUserJoin(unsigned int userID) = 0;

	/// \brief Callback event of notification that user leaves the waiting room.
	/// \param userID The ID of user who leaves the waiting room.
	virtual void onWatingRoomUserLeft(unsigned int userID) = 0;

	/// \brief During the waiting room, this callback event will be triggered when host change audio status.	
	/// \param bAudioCanTurnOn TRUE means audio can be turned on. Otherwise not.
	virtual void onWaitingRoomPresetAudioStatusChanged(bool bAudioCanTurnOn) = 0;

	/// \brief During the waiting room, this callback event will be triggered when host change video status.	
	/// \param bVideoCanTurnOn TRUE means video can be turned on. Otherwise not.
	virtual void onWaitingRoomPresetVideoStatusChanged( bool bVideoCanTurnOn) = 0;

	/// \brief During the waiting room, this callback event will be triggered when RequestCustomWaitingRoomData called.	
	/// \param The WaitingRoom Customize Data Info, handler for download waitingRoom Customize Data if download fail.
	virtual void onCustomWaitingRoomDataUpdated(CustomWaitingRoomData& bData, IWaitingRoomDataDownloadHandler* bHandler) = 0;
};
/// \brief Meeting waiting room controller interface.
///
class IMeetingWaitingRoomController
{
public:
	/// \brief Set meeting waiting room callback event handler.
	/// \param pEvent A pointer to the IMeetingWaitingRoomEvent that receives the waiting room event. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetEvent(IMeetingWaitingRoomEvent* pEvent) = 0;

	/// \brief Determine whether the current meeting supports the waiting room or not.
	/// \return True indicates to support.
	virtual bool IsSupportWaitingRoom() = 0;

	/// \brief Determine if the attendee is enabled to enter the waiting room when joining the meeting.
	/// \return True indicates to enable to enter.
	virtual bool IsWaitingRoomOnEntryFlagOn() = 0;

	/// \brief Set to enable the attendee to enter the waiting room when joining the meeting.
	/// \param bEnable True indicates to enable to enter. False not.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError EnableWaitingRoomOnEntry(bool bEnable) = 0;

	/// \brief Get the list of attendees who are in the waiting room.
	/// \return If the function succeeds, the return value is the list of attendees.
	///Otherwise failed, the return is NULL. 
	virtual IList<unsigned int >* GetWaitingRoomLst() = 0;

	/// \brief Get the attendee information in the waiting room via user ID.
	/// \param userid Specifies the user ID.
	/// \return If the function succeeds, the return value is a pointer to IUserInfo. 
	///Otherwise failed, the return is NULL. For more details, see \link IUserInfo \endlink.
	virtual IUserInfo* GetWaitingRoomUserInfoByID(unsigned int userid) = 0;

	/// \brief Permit the specified user to join the meeting.
	/// \param userid Specifies the user ID.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed, the return is NULL. For more details, see \link SDKError \endlink enum.
	virtual SDKError AdmitToMeeting(unsigned int userid) = 0;

	/// \brief Permit all of the users currently in the waiting room to join the meeting.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed, the return is NULL. For more details, see \link SDKError \endlink enum.
	virtual SDKError AdmitAllToMeeting() = 0;

	/// \brief Enable the specified user to enter the waiting room.
	/// \param userid Specifies the user ID.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError PutInWaitingRoom(unsigned int userid) = 0;

	/// \brief Determine if the attendee is enabled to turn on audio when joining the meeting.
	/// \return True indicates to enable to turn on.
	virtual bool IsAudioEnabledInWaitingRoom() = 0;

	/// \brief Determine if the attendee is enabled to turn on video when joining the meeting.
	/// \return True indicates to enable to turn on.
	virtual bool IsVideoEnabledInWaitingRoom() = 0;


	/// \brief Get the WaitingRoom CustomizeData information in the waiting room.
	/// \return If the function succeeds, the return value is SDKErr_Success. See \link onCustomWaitingRoomDataUpdated \endlink to access the result data.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError RequestCustomWaitingRoomData() = 0;
};
END_ZOOM_SDK_NAMESPACE
#endif