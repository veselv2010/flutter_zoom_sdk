/*!
* \file meeting_docs_interface.h
*/
#ifndef _MEETING_DOCS_INTERFACE_H_
#define _MEETING_DOCS_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/*! \enum SDKDocsStatus
	\brief Docs share status.
*/
typedef enum
{
	SDKDocsStatus_None,
	SDKDocsStatus_Start,		///<User starts sharing docs.
	SDKDocsStatus_Stop			///<User stops sharing docs.
}SDKDocsStatus;

/*! \enum SDKDocsShareOption
	\brief Docs share option
*/
typedef enum
{
	SDKDocsShareOption_None,				///<A wrong option, such as the meeting not supporting docs.
	SDKDocsShareOption_HostShare,			///<Only host can share docs.
	SDKDocsShareOption_HostGrabShare,		///<Anyone can share docs, but only one doc can be shared at a time, and only host can take over another's sharing.
	SDKDocsShareOption_AllGrabShare			///<Anyone can share docs, but only one doc can be shared at a time, and anyone can take over another's sharing.
}SDKDocsShareOption;

/*! \enum SDKDocsCreateOption
	\brief Docs create option
*/
typedef enum
{
	SDKDocsCreateOption_None,				///<A wrong option, maybe the meeting does not support docs.
	SDKDocsCreateOption_HostOnly,			///<Only the host can initiate new docs.
	SDKDocsCreateOption_AccountUsers,		///<Users under the same account can initiate new docs.
	SDKDocsCreateOption_All					///<All participants can initiate new docs.
}SDKDocsCreateOption;

/// \brief Docs controller callback event.
class IMeetingDocsCtrlEvent
{
public:
	virtual ~IMeetingDocsCtrlEvent() {}

	/// \brief Docs status changed callback. Use this function to inform the user docs has been started or stopped, and all users in the meeting can get the event.
	/// \param status Specify current docs status. For more details, see \link SDKDocsStatus \endlink enum.
	virtual void onDocsStatusChanged(SDKDocsStatus status) = 0;

	/// \brief Callback event of docs share setting type changed.
	/// \param createOption Docs who can create new docs type. For more details, see \link SDKDocsCreateOption \endlink enum.
	/// \param option Docs who can share type. For more details, see \link SDKDocsShareOption \endlink enum.
	virtual void onDocsPermissionChanged(SDKDocsCreateOption createOption, SDKDocsShareOption option) = 0;
	
};

/// \brief Meeting Docs helper interface.
class IMeetingDocsController
{
public:
	virtual ~IMeetingDocsController() {}

	/// \brief Configure the meeting docs controller callback event handler.
	/// \param pEvent An object pointer to the IMeetingDocsCtrlEvent that receives the meeting docs callback event. For more details, see \link IMeetingDocsCtrlEvent \endlink.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks The SDK use pEvent to transmit the callback event to the user's application. If the function is not called or fails, the user's application is unable to retrieve the callback event.
	virtual SDKError SetEvent(IMeetingDocsCtrlEvent* pEvent) = 0;

	/// \brief Determine whether the current meeting supports the docs or not.
	/// \return True indicates to support.
	virtual bool IsSupportDocs() = 0;

	/// \brief Determine whether the current user can start sharinge the doc or not.
	/// \param [out] reason The reason why no one can start sharing the doc. See \link CannotShareReasonType \endlink enum.
	/// \return True indicates you can start sharing the doc.
	virtual bool CanStartShareDocs(CannotShareReasonType& reason) = 0;

	/// \brief Determine whether the current user can set docs options or not.
	/// \return True means the current user can set docs options, false means the user can' not set docs options.
	virtual bool CanSetDocsOption() = 0;

	/// \brief Set the setting option for who can share docs.
	/// \param [in] The setting for who can share docs, see \link SDKDocsShareOption \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetDocsShareOption(SDKDocsShareOption option) = 0;

	/// \brief Get the setting option for who can share docs.
	/// \param [out] option Setting option for who can share docs, see \link SDKDocsShareOption \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError GetDocsShareOption(SDKDocsShareOption& option) = 0;

	/// \brief Set the option for who can initiate a new doc.
	/// \param [in] option Setting option for who can initiate a new doc, see \link SDKDocsCreateOption \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetDocsCreateOption(SDKDocsCreateOption option) = 0;

	/// \brief Get the option for who can initiate a new doc.
	/// \param [out] option Setting option for who can initiate a new doc, see \link SDKDocsCreateOption \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError GetDocsCreateOption(SDKDocsCreateOption& option) = 0;

	/// \brief Query if other user is sharing docs.
	/// \return True means sharing.
	virtual bool IsOtherSharingDocs() = 0;

	/// \brief Query if the current user is sharing docs successfully.
	/// \return True means sharing successfully.
	virtual bool IsSharingDocsOut() = 0;

	/// \brief Set the docs web view and dashboard web view owner window.
	/// \param [in] hOwner Specify the owner window. 
	/// \Call this function before calling ShowDocsDashboardView. Call this function when receiving the meeting status MEETING_STATUS_INMEETING. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid only for user custom interface mode.
	virtual SDKError  SetOwnerWindow(HWND hOwner) = 0;

	/// \brief Show the docs dashboard web view window.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid only for custom user interface mode.
	virtual SDKError ShowDocsDashboard() = 0;

	/// \brief Set the docs web view window position.
	/// \param x Specifies the X-axis coordinate of the top-left corner of the docs web view window in the parent window.
	/// \param y Specifies the Y-axis coordinate of the top-left corner of the docs web view window in the parent window.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid only for custom user interface mode.
	virtual SDKError SetDocsViewPos(int x, int y) = 0;

	/// \brief Set the docs web view window size.
	/// \param height Specify the height of the docs web view window. 
	/// \param width Specify the width of the docs web view window. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks Valid only for custom user interface mode.
	virtual SDKError SetDocsViewSize(int width, int height) = 0;

	/// \brief Update the position and size of the docs web view windowr window when its parent window moves or changes size.
	/// \remark This interface should be invoked when the OnSize and OnMove messages of the parent window recieved. And call this function after calling SetDocsViewPos and SetDocsViewSize. Valid for user custom interface mode only.
	virtual void UpdateDocsView() = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif