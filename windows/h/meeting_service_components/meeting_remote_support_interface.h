/*!
* \file meeting_remote_support_interface.h
* \brief Meeting Service Remote Support Interface
* Valid for both ZOOM style and user custom interface mode.
*/
#ifndef _MEETING_REMOTE_SUPPORT_INTERFACE_H_
#define _MEETING_REMOTE_SUPPORT_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
/*! \enum RemoteSupportType
    \brief Information category for remote support.
    Here are more detailed structural descriptions.
*/
enum RemoteSupportType
{
	Remote_Support_None,///<For initialization.
	Remote_Support_Desktop,///<Remote support for desktop.	
	Remote_Support_Application,///<Remote support for application.	
};

/*! \enum RemoteSupportStatus
	\brief Information category for authority and status changes during remote support.
	Here are more detailed structural descriptions.
*/
enum RemoteSupportStatus
{
	RemoteSupport_RequestDeny,///Request was denied by other.
	RemoteSupport_RequestGrant,///Request was granted by other.
	RemoteSupport_Stop_By_SupportedUser,///User stops remote support.
	RemoteSupport_Stop_By_Supporter,///Supporter stops remote support.
};


/// \brief Process after the user receives the requirement from the host to give the remote support privilege.
class IRemoteSupportRequestHandler
{
public:
	virtual ~IRemoteSupportRequestHandler() {};

	/// \brief Get the user ID who requested privilege.
	/// \return If the function succeeds, the return value is the user ID. Otherwise, this returns 0.
	virtual unsigned int GetRequesterId() = 0;

	/// \brief Get the user name who requested privileges.
	/// \return If the function succeeds, the return value is the user name.
	virtual const zchar_t* GetRequesterName() = 0;

	/// \brief Get the remote support type.
	/// \return If the function succeeds, the return value is the RemoteSupportType.
	virtual RemoteSupportType GetRemoteSupportType() = 0;
	
	/// \brief Set the specified application.
	/// \param view Specify the window handle of the application to be remote support. If the view can't be shared, the return value is the SDKERR_INVALID_PARAMETER error code.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Set the application view only when RemoteSupportType is Remote_Support_Application.
	virtual SDKError SetRemoteSupportApplicationView(void* view) = 0;

	/// \brief Allows the user to remote support and finally self-destroy.
	virtual SDKError Grant() = 0;

	/// \brief Denies the user to remote support and finally self-destroy.
	virtual SDKError Deny() = 0;
};

/// \brief Meeting remote support callback event.
///
class IMeetingRemoteSupportEvent
{
public:
	/// \brief Callback event of notification of the remote support status. 
	/// \param status The value of remote support status. For more details, see \link RemoteSupportStatus \endlink enum.
	/// \param userId The remote support user ID.
	virtual void onRemoteSupportStatus(RemoteSupportStatus status, unsigned int userId) = 0;

	/// \brief These IRemoteSupportRequestHandler have expired.
	virtual void onRemoteSupportRequestExpired(IList<IRemoteSupportRequestHandler*>* requestHandleList) = 0;

	/// \brief Callback event when a host requests to support you remotely.
	/// \param handler A pointer to the IRemoteSupportRequestHandler. For more details, see \link IRemoteSupportRequestHandler \endlink.
	virtual void onRemoteSupportRequestReceived(IRemoteSupportRequestHandler* handler) = 0;

	/// \brief Callback event for update the user's screen list when the request be granted.
	/// \param userId The remote support user ID.
	/// \param monitorList The monitor list ID to be supported.
	virtual void onRemoteSupportScreenListUpdated(unsigned int userId, IList<int>* monitorList) = 0;

	/// \brief Callback event when remote support requests are forbidden for guest users. 
	/// \param userId The remote support user ID.
	virtual void onUserRemoteSupportForbidden(unsigned int userId) = 0;
};

/// \brief Meeting remote support interface.
///
class IMeetingRemoteSupportController
{
public:
	virtual ~IMeetingRemoteSupportController() {}

	/// \brief Set meeting remote support callback event handler.
	/// \param pEvent A pointer to the IMeetingRemoteSupportEvent that receives the remote support event. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks When the remote support or the authority changes, the user receives a callback event.
	virtual SDKError SetEvent(IMeetingRemoteSupportEvent* pEvent) = 0;
	
	/// \brief Query Whether the meeting supports remote support.
	/// \return true if supported, false if not supported.
	virtual bool IsSupportRemoteSupport() = 0;

	/// \brief Whether if the current user can request remote support.
	/// \return true if the user can request remote support, false if they can't.
	virtual bool CanRequestRemoteSupport() = 0;

	/// \brief Get the list of users'IDs that can be supported remotely.
	/// \return If the function succeeds, the return value is a pointer to the IList object.
	virtual IList<unsigned int>* GetRemoteSupportUserList() = 0;

	/// \brief Send the request of remote support to the specified user.
	/// \param userId Specify the user ID that you want to send the request.
	/// \param supportType Specify category for remote support   .
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError RequestRemoteSupport(unsigned int userId, RemoteSupportType supportType) = 0;

	/// \brief Stop the remote support to the specified user.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks It will stop the share currently in progress.
	virtual SDKError StopRemoteSupport() = 0;

	/// \brief Whether if the current user is able to select remote support screen.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	virtual SDKError CanSelectRemoteSupportScreen() = 0;

	/// \brief Select the monitor of remote support to the specified user.
	/// \param screenIndex Specify the screen index that you want to control.	
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SelectRemoteSupportScreen(int screenIndex) = 0;
};
END_ZOOM_SDK_NAMESPACE
#endif