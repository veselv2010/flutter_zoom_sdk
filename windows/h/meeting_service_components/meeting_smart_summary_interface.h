#ifndef _MEETING_SMART_SUMMARY_INTERFACE_H_
#define _MEETING_SMART_SUMMARY_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE


/*! \class ISmartSummaryStartRequestHandler
	\brief Interface to handle start smart summary request
*/
class ISmartSummaryPrivilegeHandler
{
public:
	virtual ~ISmartSummaryPrivilegeHandler() {};

	/// \brief Agree the start smart summary request.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Accept() = 0;

	/// \brief Decline the start smart summary request.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Decline() = 0;

	/// \brief Ignore the start smart summary request.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Ignore() = 0;
};
/// \brief Meeting smart summary callback event.
///
class IMeetingSmartSummaryControllerEvent
{
public:
	/// \brief Callback event when smart summary status changes.
	/// \param isStarted true means smart summary is started, false means not.
	virtual void onSmartSummaryStatusChange(bool isStarted) = 0;

	/// \brief Callback event when a user requests host to start smart summary.
	/// \param senderId The user who requests host to start smart summary.
	/// \param handler The handler to handle the smart summary start request.
	virtual void onSmartSummaryPrivilegeRequested(unsigned int senderId, ISmartSummaryPrivilegeHandler* handler) = 0;

	/// \brief Callback event when the host handle the smart summary request.
	/// \param timeout True means the host not handle the request until timeout.
	/// \param decline True means the host decline the request, false means the host agree the request.
	virtual void onSmartSummaryStartReqResponse(bool timeout, bool decline) = 0;

	/// \brief Callback event when the user need to go to web to enable the smart summary feature.
	/// \remarks Valid for both ZOOM style and user custom interface mode. The callback will only be triggered for Business account. And the smart summary feature will be enabled for the future meeting, but not enable the current meeting if the user enables the setting in web.
	virtual void onEnableSmartSummary() = 0;

	virtual ~IMeetingSmartSummaryControllerEvent() {}

};

/// \brief Meeting smart summary controller interface.
///
class IMeetingSmartSummaryController
{
public:
	virtual ~IMeetingSmartSummaryController() {}

	/// \brief Set the smart summary callback event handler.
	/// \param event A pointer to the IMeetingSmartSummaryControllerEvent that receives the smart summary event. 
	virtual void SetEvent(IMeetingSmartSummaryControllerEvent* event) = 0;

	/// \brief Determine if current meeting support smart summary feature.
	/// \return True means the current meeting support smart summary feature, False means not supported.
	virtual bool IsSmartSummarySupported() = 0;

	/// \brief Determine if smart summary feature is enabled in the meeting.
	/// \return True means smart summary feature is enabled.
	virtual bool IsSmartSummaryEnabled() = 0;

	/// \brief Whether the current user can enable the smart summary feature for the account.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError CanEnableSmartSummaryFeature() = 0;

	/// \brief Enable the smart summary feature for the account.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError EnableSmartSummaryFeature() = 0;

	/// \brief Whether the current user is able to start smart summary.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError CanStartSmartSummary() = 0;

	/// \brief Start smart summary.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError StartSmartSummary() = 0;

	/// \brief Stop smart summary.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError StopSmartSummary() = 0;

	/// \brief Query whether smart summary is started.
	/// \return True means smart summary is started, false means not.
	virtual bool IsSmartSummaryStarted() = 0;

	/// \brief Whether the current user can request the admin to enable the smart summary feature for the account.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError CanRequestEnableSmartSummaryFeature() = 0;

	/// \brief Request the admin to enable the smart summary feature for the account.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError RequestEnableSmartSummaryFeature() = 0;

	/// \brief Whether the current user can request host to start the smart summary for the current meeting.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError CanRequestStartSmartSummary() = 0;

	/// \brief Request the host to start the smart summary for the current meeting.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError RequestStartSmartSummary() = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif