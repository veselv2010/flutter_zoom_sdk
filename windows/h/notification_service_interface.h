/*!
* \file notification_service_interface.h
* \brief Notification Service Interface
* Valid for both ZOOM style and user custom interface mode.
*/
#ifndef _NOTIFICATION_SERVICE_INTERFACE_H_
#define _NOTIFICATION_SERVICE_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/*! \struct MeetingDeviceInfo
	\brief information of meeting device info.
	Here are more detailed structural descriptions..
*/
struct InMeetingDeviceInfo
{
	int nIndex;///<deprecated this param
	const zchar_t* deviceName;///<device name
	const zchar_t* meetingTopic;///<meeting topic
	const zchar_t* meetingId;///<meeting id
	UINT64 meetingNumber;///<meeting number
	bool isSupportCompanionMode;///<True indicates this meeting supports joining in companion mode
	InMeetingDeviceInfo()
	{
		nIndex = -1;
		deviceName = nullptr;
		meetingTopic = nullptr;
		meetingId = nullptr;
		meetingNumber = 0;
		isSupportCompanionMode = false;
	}
};

/*! \enum TransferMeetingMode
	\brief Type of transfer meeting mode.
	Here are more detailed structural descriptions.
*/
enum TransferMeetingMode
{
	TransferMeetingMode_None,///<For initialization.
	TransferMeetingMode_Transfer,///<Try to transfer meeting to current device.
	TransferMeetingMode_Companion,///<Try to join meeting with companion mode.If the meeting is successfully joined, both video and audio will be unavailable.
};

/// \brief Process after the user joins meeting in companion mode or transfer meeting.
class ITransferMeetingHandler
{
public:
	virtual ~ITransferMeetingHandler() {};

	/// \brief Determine if transfer meeting or join meeting in companion mode success.
	/// \return True indicates transfer meeting or join meeting in companion mode success.
	virtual bool IsTransferMeetingSuccess() = 0;

	/// \brief Get the mode of transfer meeting.
	/// \return The mode of the transfer meeting. For more details, see \link TransferMeetingMode \endlink enum.
	virtual TransferMeetingMode GetTransferMeetingMode() = 0;

	/// \brief Try to leave companion mode and rejoin the meeting in normal mode.	
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError LeaveCompanionToJoinMeeting() = 0;
};

/// \brief Notification service helper callback event.
///
class INotificationServiceEvent
{
public:
	/// \brief Callback event that the device for users to attend the meeting has changed.	
	/// \param pDeviceList A list of meeting info with active meetings.
	/// \If the user USES IAuthService::RegisterNotificationService interface to connect to the service,they can't get webinar meeting list in this callback.
	virtual void onMeetingDeviceListChanged(IList<InMeetingDeviceInfo>* pDeviceList) = 0;

	/// \brief Transfer meeting status changed callback.
	/// \param bSuccess The result of transfer meeting.
	/// \deprecated This interface is marked as deprecated.
	virtual void onTransferMeetingStatus(bool bSuccess) = 0;

	/// \brief Transfer meeting status changed or join companion mode meeting callback.
	/// \param handler A pointer to the ITransferMeetingHandler. For more details, see \link ITransferMeetingHandler \endlink.
	/// \param bSuccess The result of transfer meeting.
	virtual void onTransferMeetingResult(ITransferMeetingHandler* handler) = 0;
};

/// \brief Notification service control interface.
class IPresenceHelper;
class INotificationServiceHelper 
{
public:
	virtual ~INotificationServiceHelper () {}

	/// \brief Get the Presence helper.
	/// \return If the function succeeds, the return value is a pointer to IPresenceHelper. Otherwise the function fails and returns nullptr.
	virtual IPresenceHelper* GetPresenceHelper() = 0;

	/// Set the controller event of notification service.
	virtual SDKError SetEvent(INotificationServiceEvent* pEvent) = 0;

	/// brief Determine whether the transfer meeting feature is enabled.
	virtual bool IsTransferMeetingEnabled() = 0;

	/// brief Determine whether the presence feature is enabled.
	virtual bool IsPresenceFeatureEnabled() = 0;

	/// \brief Try to transfer meeting to current device.
	/// \param nIndex Specifies the index of meeting list.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \deprecated This interface is marked as deprecated.
	virtual SDKError TransferMeeting(int nIndex) = 0;

	/// \brief Try to transfer meeting to current device or join meeting with companion mode.
	/// \param TransferMeetingMode Specifies the transfer meeting mode.
	/// If the mode is TransferMeetingMode_Companion, you will join meeting as a companion,Otherwise will transfer meeting to current device.
	/// \param meetingId Specifies the meeting list's meeting ID.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError TransferMeeting(TransferMeetingMode mode,const zchar_t* meetingId) = 0;

};
END_ZOOM_SDK_NAMESPACE
#endif