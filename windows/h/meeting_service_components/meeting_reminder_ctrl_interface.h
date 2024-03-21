/*!
* \file meeting_reminder_ctrl_interface.h
* \brief Meeting Service Phone Interface.
* Valid for both ZOOM style and user custom interface mode.
*/
#ifndef _MEETING_REMINDER_CTRL_INTERFACE_H_
#define _MEETING_REMINDER_CTRL_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/*! \enum MeetingReminderType
	\brief The type for disclaimer dialog.
	Here are more detailed structural descriptions.
*/
enum MeetingReminderType
{
	TYPE_LOGIN_REQUIRED,///Reminder type of login.
	TYPE_START_OR_JOIN_MEETING,///<Reminder type of start or join meeting.
	TYPE_RECORD_REMINDER,///<Reminder type of record reminder.
	TYPE_RECORD_DISCLAIMER,///<Reminder type of record disclaimer
	TYPE_LIVE_STREAM_DISCLAIMER,///<Reminder type of live stream disclaimer.
	TYPE_ARCHIVE_DISCLAIMER,///<Reminder type of archive disclaimer.
	TYPE_WEBINAR_AS_PANELIST_JOIN,///<Reminder type of join webinar as panelist.
	TYPE_TERMS_OF_SERVICE, ///Reminder type of Terms of service or privacy statement changed.
	TYPE_SMART_SUMMARY_DISCLAIMER, ///<Reminder type of smart summary disclaimer.
	TYPE_SMART_SUMMARY_ENABLE_REQUEST_REMINDER, ///<Reminder type of smart summary enable request.
	TYPE_QUERY_DISCLAIMER, ///<Reminder type of query disclaimer.
	TYPE_QUERY_ENABLE_REQUEST_REMINDER, ///<Reminder type of query enable request.
	TYPE_ENABLE_SMART_SUMMARY_REMINDER,///<Reminder type of enable smart summary.
	TYPE_WEBINAR_ATTENDEE_PROMOTE_REMINDER,///<Reminder type of webinar promote attendee.
	TYPE_JOIN_PRIVATE_MODE_MEETING_REMINDER,///<Reminder type of joining a meeting with private mode.
	TYPE_SMART_RECORDING_ENABLE_REQUEST_REMINDER,///<Reminder type to enable smart recording request.
	TYPE_ENABLE_SMART_RECORDING_REMINDER,///<Reminder type to enable smart recording.
};

/*! \enum ActionType
	\brief The type of the action which user should take.
	Here are more detailed structural descriptions.
*/
enum ActionType
{
	ACTION_TYPE_NONE,///<Need no more action.
	ACTION_TYPE_NEED_SIGN_IN,///<Need to sign in.
	ACTION_TYPE_NEED_SWITCH_ACCOUNT,///<Need to switch account.
};

/// \brief the interface of reminder dialog content.
class IMeetingReminderContent
{
public:
	virtual ~IMeetingReminderContent() {};
	/// \brief Get the type of the reminder.
	virtual MeetingReminderType  GetType() = 0;
	/// \brief Get the title of the reminder dialog content.
	virtual const zchar_t* GetTitle() = 0;
	/// \brief Get the detail content of the reminder dialog content.
	virtual const zchar_t* GetContent() = 0;
	/// \brief Determine whether block the user join or stay in the meeting
	/// \return True indicates block the user join or stay in the meeting. Otherwise False.
	virtual bool  IsBlocking() = 0;
	/// \brief Get the type of the action which user should take after receiving this reminder content.
	virtual ActionType GetActionType() = 0;
};

/// \brief the interface to handle the reminder dialog.
class IMeetingReminderHandler
{
public:
	virtual ~IMeetingReminderHandler() {};
	/// \brief Ignore the reminder.
	virtual void  Ignore() = 0;
	/// \brief Accept the reminder.
	virtual void  Accept() = 0;
	/// \brief Decline the reminder.
	virtual void  Decline() = 0;
};

/*! \enum FeatureEnableOption
	\brief The option for meeting feature.
*/
enum FeatureEnableOption
{
	EnableOption_None,///do not enable.
	EnableOption_Once,///enable for this meeting.
	EnableOption_Always,///enable for this and all future meetings on this account.
};

/// \brief if the current user can control web setting, they will get this handler when an attendee requests to start the smart recording feature or start feature by itself.
class IMeetingEnableReminderHandler
{
public:
	virtual ~IMeetingEnableReminderHandler() {};

	/// \brief Set the option indicating which meetings smart recording will be enabled for.
	/// \param option Specify the option. 	
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetEnableOption(FeatureEnableOption option) = 0;

	/// \brief Start the smart recording feature.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Start() = 0;

	/// \brief Decline the reminder.
	/// \param bDeclineAll TRUE means decline all reminders,and participants cannot send requests again until the host changes the setting. FALSE means that the host declines only this specific request, not all requests.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Decline(bool bDeclineAll) = 0;

	/// \brief Ignore the reminder.
	virtual SDKError Ignore() = 0;
};

/// \brief Callback event to enable showing the reminder dialog.
class IMeetingReminderEvent
{
public:
	virtual ~IMeetingReminderEvent() {}

	/// \brief Callback event of the reminder dialog show.
	/// \param content The detail content in the reminder dialog. For more details, see \link IMeetingReminderContent \endlink enum.
	/// \param handle A pointer to the IMeetingReminderHandler. For more details, see \link IMeetingReminderHandler \endlink.
	virtual void onReminderNotify(IMeetingReminderContent* content, IMeetingReminderHandler* handle) = 0;

	/// \brief Callback event of the enable reminder dialog show.
	/// \param content The detail content in the reminder dialog. For more details, see \link IMeetingEnableReminderHandler \endlink enum.
	/// \param handle A pointer to the IMeetingReminderHandler. For more details, see \link IMeetingEnableReminderHandler \endlink.
	virtual void onEnableReminderNotify(IMeetingReminderContent* content, IMeetingEnableReminderHandler* handle) = 0;
};
/// \brief Meeting reminder dialog interface.
///
class IMeetingReminderController
{
public:
	/// \brief Set meeting reminder controller callback event handler
	/// \param pEvent A pointer to the IMeetingReminderEvent that receives reminder callback event. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetEvent(IMeetingReminderEvent* pEvent) = 0;
};
END_ZOOM_SDK_NAMESPACE
#endif