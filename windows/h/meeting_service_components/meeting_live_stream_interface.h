/*!
* \file meeting_live_stream_interface.h
* \brief Meeting Service Live Streaming Interface.
*
*/
#ifndef _MEETING_LIVE_STREAM_INTERFACE_H_
#define _MEETING_LIVE_STREAM_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/*! \enum LiveStreamStatus
	\brief Status of live stream.
	Here are more detailed structural descriptions.
*/
enum LiveStreamStatus
{
	LiveStreamStatus_None,///<Only for initialization.
	LiveStreamStatus_InProgress,///<In progress.
	LiveStreamStatus_Connecting,///<Be connecting.
	LiveStreamStatus_Start_Failed_Timeout,///<Connect timeout.
	LiveStreamStatus_Start_Failed,///<Failed to start live streaming. 
	LiveStreamStatus_Ended,///<Live stream ends.
};

/*! \struct RawLiveStreamInfo
	\brief information of raw live stream info.
	Here are more detailed structural descriptions..
*/
struct RawLiveStreamInfo
{
	unsigned int userId;///<User ID.
	const zchar_t* broadcastUrl;///<Broadcast URL
	const zchar_t* broadcastName;///<Broadcast Name
	RawLiveStreamInfo()
	{
		userId = 0;
		broadcastUrl = nullptr;
		broadcastName = nullptr;
	}
};

/// \brief Process after the host receives the requirement from the user to give the raw live stream privilege.
class IRequestRawLiveStreamPrivilegeHandler
{
public:
	virtual ~IRequestRawLiveStreamPrivilegeHandler() {};
	/// \brief Get the request ID.
	/// \return If the function succeeds, the return value is the request ID.
	virtual const zchar_t* GetRequestId() = 0;

	/// \brief Get the user ID who requested privilege.
	/// \return If the function succeeds, the return value is the user ID. Otherwise, this returns 0.
	virtual unsigned int GetRequesterId() = 0;

	/// \brief Get the user name who requested privileges.
	/// \return If the function succeeds, the return value is the user name.
	virtual const zchar_t* GetRequesterName() = 0;

	/// \brief Get the broadcast URL.
	/// \return If the function succeeds, the return value is the broadcast URL.
	virtual const zchar_t* GetBroadcastUrl() = 0;

	/// \brief Get the broadcast name.
	/// \return If the function succeeds, the return value is the broadcast name.
	virtual const zchar_t* GetBroadcastName() = 0;

	/// \brief Allows the user to start raw live stream and finally self-destroy.
	virtual SDKError GrantRawLiveStreamPrivilege() = 0;

	/// \brief Denies the user permission to start raw live stream and finally self-destroy.
	virtual SDKError DenyRawLiveStreamPrivilege() = 0;
};

/// \brief Live stream meeting controller callback event.
///
class IMeetingLiveStreamCtrlEvent
{
public:
	/// \brief Callback event when live stream status changes.
	/// \param status Live stream status. For more details, see \link LiveStreamStatus \endlink enum.
	virtual void onLiveStreamStatusChange(LiveStreamStatus status) = 0;

	/// \brief Callback event when the current user's raw live streaming privilege changes.
	/// \param bHasPrivilege Specify whether or not the user has privileg.
	virtual void onRawLiveStreamPrivilegeChanged(bool bHasPrivilege) = 0;

	/// \brief Callback event when the current user's request has time out.
	virtual void onRawLiveStreamPrivilegeRequestTimeout() = 0;

	/// \brief Callback event when another user's raw live streaming privilege has changed.
	/// \param userid The ID of the user whose privilege has changed.
	/// \param bHasPrivilege Specify whether the user has privilege or not.
	virtual void onUserRawLiveStreamPrivilegeChanged(unsigned int userid, bool bHasPrivilege) = 0;

	/// \brief Callback event when a user requests raw live streaming privilege.
	/// \param handler A pointer to the IRequestRawLiveStreamPrivilegeHandler. For more details, see \link IRequestRawLiveStreamPrivilegeHandler \endlink.
	virtual void onRawLiveStreamPrivilegeRequested(IRequestRawLiveStreamPrivilegeHandler* handler) = 0;

	/// \brief Callback event when users start/stop raw live streaming.
	/// \param liveStreamList A list of users with an active raw live stream.
	virtual void onUserRawLiveStreamingStatusChanged(IList<RawLiveStreamInfo>* liveStreamList) = 0;

	/// \brief Callback event when the live stream reminder enable status changes.
	/// \param enable True means the live stream reminder is enabled.
	virtual void onLiveStreamReminderStatusChanged(bool enable) = 0;

	/// \brief Callback event when the live stream reminder enable status change fails.
	virtual void onLiveStreamReminderStatusChangeFailed() = 0;

	/// \brief Callback event when the meeting or webinar user has nearly reached the meeting capacity, like 80% or 100% for the meeting or webinar capacity.
	/// The host can start live stream to let unjoined user watch live stream.
	/// \param percent Proportion of joined users to the total capacity.
	virtual void onUserThresholdReachedForLiveStream(int percent) = 0;
};

/// \brief Live stream of current meeting.
///
class IMeetingLiveStreamItem
{
public:
	/// \brief Get URL of the live stream meeting.
	/// \return If the function succeeds, the return value is the URL of the live stream meeting.
	virtual const zchar_t* GetLiveStreamURL() = 0;

	/// \brief Get the descriptions of live stream.
	/// \return If the function succeeds, the return value is the description of live stream.
	virtual const zchar_t* GetLiveStreamURLDescription() = 0;

	/// \brief Get the viewer URL of the live stream meeting.
	/// \return If the function succeeds, the return value is the viewer URL of the live stream meeting.
	virtual const zchar_t* GetLiveStreamViewerURL() = 0;
	virtual ~IMeetingLiveStreamItem() {};
};

/// \brief Live stream meeting controller interface.
///
class IMeetingLiveStreamController
{
public:
	/// \brief Set live stream meeting callback event handler
	/// \param pEvent A pointer to the IMeetingLiveStreamCtrlEvent that receives live stream event. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetEvent(IMeetingLiveStreamCtrlEvent* pEvent) = 0;

	/// \brief Determine if it is able to start live streaming.
	/// \return If it is enabled to start the live streaming, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError CanStartLiveStream() = 0;

	/// \brief Start live streaming.
	/// \param item_ A pointer to the IMeetingLiveStreamItem created via IMeetingLiveStreamController::GetSupportLiveStreamURL() API. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError StartLiveStream(IMeetingLiveStreamItem* item_) = 0;

	/// \brief Start live streaming.
	/// \param streamingURL The URL of live streaming.
	/// \param streamingKey The key of live streaming. 
	/// \param broadcastURL The broadcast URL of live-stream.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Get the parameters from the third party of live stream service
	virtual SDKError StartLiveStreamWithSteamingURL(const zchar_t* streamingURL, const zchar_t* streamingKey, const zchar_t* broadcastURL) = 0;

	/// \brief Stop live streaming.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError StopLiveStream() = 0;

	/// \brief Get the list of URL and associated information used by live streaming in the current meeting. 
	/// \return If the function succeeds, the return value is the meeting information to be live streamed.
	///Otherwise failed, the return value is nullptr. For more details, see \link IMeetingLiveStreamItem \endlink.
	/// \deprecated This interface is marked as deprecated, and is replaced by GetSupportLiveStreamItems().
	virtual IList<IMeetingLiveStreamItem* >* GetSupportLiveStreamURL() = 0;

	/// \brief Get the list of live stream information items in the current meeting.
	/// \return If the function succeeds, the return value is the live stream item list.
	///Otherwise the function fails and the return value is nullptr. For more details, see \link IMeetingLiveStreamItem \endlink.
	virtual IList<IMeetingLiveStreamItem* >* GetSupportLiveStreamItems() = 0;

	/// \brief Get the current live stream object. 
	/// \return If the function succeeds, the return value is the current live stream object.
	///Otherwise the function fails and the return value is nullptr. For more details, see \link IMeetingLiveStreamItem \endlink.
	virtual IMeetingLiveStreamItem* GetCurrentLiveStreamItem() = 0;

	/// \brief Get live stream status of current meeting.
	/// \return If the function succeeds, the return value is the live stream status of current meeting.  
	///Otherwise failed. For more details, see \link LiveStreamStatus \endlink enum.
	virtual LiveStreamStatus GetCurrentLiveStreamStatus() = 0;

	/// \brief Query Whether the meeting supports raw live streams.
	/// \return true if supported, false if not supported.
	virtual bool IsRawLiveStreamSupported() = 0;

	/// \brief Whether if the current user is able to start raw live streaming.
	/// \return If the current user is able to start raw live streaming, the return value is SDKErr_Success.
	///Otherwise it fails,and returns nullptr. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError CanStartRawLiveStream() = 0;

	/// \brief Send a request to enable the SDK to start a raw live stream.
	/// \param broadcastURL The broadcast URL of the live-stream.
	/// \param broadcastName The broadcast name of the live-stream.
	/// \return If the function succeeds, the return value is SDKErr_Success and the SDK will send the request.
	///Otherwise it fails and the request will not be sent. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError RequestRawLiveStreaming(const zchar_t* broadcastURL, const zchar_t* broadcastName) = 0;

	/// \brief Start raw live streaming.
	/// \param broadcastURL The broadcast URL of the live-stream.
	/// \param broadcastName The broadcast name of the live-stream.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError StartRawLiveStreaming(const zchar_t* broadcastURL, const zchar_t* broadcastName) = 0;

	/// \brief Stop raw live streaming.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError StopRawLiveStream() = 0;

	/// \brief Remove the raw live stream privilege.
	/// \param userID Specify the ID of the user whose privilege will be removed.
	/// \return If the function succeeds, the return value is SDKErr_Success. Otherwise it fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError RemoveRawLiveStreamPrivilege(unsigned int userid) = 0;

	/// \brief Get a list of current active raw live streams.
	/// \return If the function succeeds, the return value is a pointer to the IList object. For more details, see \link IList \endlink.
	virtual IList<RawLiveStreamInfo>* GetRawLiveStreamingInfoList() = 0;

	/// \brief Get the list of users'IDs who have raw live stream privileges.
	/// \return If the function succeeds, the return value is a pointer to the IList object.
	virtual IList<unsigned int>* GetRawLiveStreamPrivilegeUserList() = 0;

	/// \brief Check if the live stream reminder is enabled.
	///When the live stream reminder is enabled, the new join user is notified that the meeting is at capacity but that they can
	///watch the meeting live stream with the callback IMeetingServiceEvent::onMeetingFullToWatchLiveStream when the meeting user has reached the meeting capability.
	/// \return True means the live stream reminder is enabled.
	virtual bool IsLiveStreamReminderEnabled() = 0;

	/// \brief Check if the current user can enable/disable the live stream reminder.
	/// \return True means the current user can enable/disable the live stream reminder.
	virtual bool CanEnableLiveStreamReminder() = 0;

	/// \brief Enable or disable the live stream reminder.
	/// \param enable True means enable the live stream reminder. False means disable the live stream reminder.
	/// \return If the function succeeds, the return value is SDKErr_Success. Otherwise it fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError EnableLiveStreamReminder(bool enable) = 0;
};
END_ZOOM_SDK_NAMESPACE
#endif