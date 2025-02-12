/*!
* \file customized_share_render.h
* \brief ZOOM Custom Video Share Render Interface.
* 
*/
#ifndef _ZOOM_CUSTOMIZED_SHARE_RENDER_H_
#define _ZOOM_CUSTOMIZED_SHARE_RENDER_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
/*! \enum CustomizedViewShareMode
    \brief View mode of shared render.
    Here are more detailed structural descriptions.
*/ 
enum CustomizedViewShareMode 
{
	CSM_FULLFILL,///<View the shared content in full screen mode.
	CSM_LETTER_BOX,///<View the shared content in original size.
};

/// \brief Share render callback event.
///
class ICustomizedShareRenderEvent
{
public:
	/// \brief Callback event the moment received the shared content.
	virtual void onSharingContentStartReceiving() = 0;

	/// \brief Callback event of changed sender or the sharing closes when receiving the shared content.
	/// \param nShareSourceID The new sharer's share source ID.
	virtual void onSharingSourceNotification(unsigned int nShareSourceID) = 0;

	/// \brief Callback event when the App receives the window messages from the sharer. Here are the list of the window messages.
	///WM_MOUSEMOVE
	///WM_MOUSEENTER
	///WM_MOUSELEAVE
	///WM_LBUTTONDOWN
	///WM_LBUTTONUP
	///WM_RBUTTONUP
	///WM_LBUTTONDBLCLK
	///WM_KEYDOWN
	virtual void onWindowMsgNotification(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};

/// \brief Share action base interface.
///
class IShareActionBase
{
public:

	/// \brief Show the shared content received.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Show() = 0;

	/// \brief Hide the shared content received.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Hide() = 0;

	/// \brief Get the window handle of showing sharing content.
	/// \return If the function succeeds, the return value is the window handle.
	///Otherwise failed, the return value is nullptr.
	virtual HWND GetOwnerWnd() = 0;

	virtual ~IShareActionBase() {}
};

/// \brief Share render interface.
///
class ICustomizedShareRender : public IShareActionBase
{
public:
	/// \brief Set the share render callback event handler.
	/// \param pEvent A pointer to the ICustomizedShareRenderEvent that receives the share render event. 
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetEvent(ICustomizedShareRenderEvent* pEvent) = 0;

	/// \brief View the sharing content from the specified share source ID.
	/// \param nShareSourceID Specify the share source ID.
	/// \remarks Call the IMeetingShareController::GetViewableShareSourceList to get the list of users who are sending the sharing. 
	///Handle the IMeetingShareCtrlEvent::onSharingStatus callback event to know who starts/stops the sharing.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetShareSourceID(unsigned int nShareSourceID) = 0;

	/// \brief Get the share source ID of the user who's sending the sharing.
	/// \return If the function succeeds, the return value is the share source ID. 
	///Otherwise failed, the return value is ZERO(0).
	virtual unsigned int GetShareSourceID() = 0;

	/// \brief Reset the dialog size to view the sharing content.
	/// \param rc Specify the size of the window. The coordinate of the upper left corner of the window corresponds to the client area of the parent window. 
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Resize(RECT rc) = 0;

	/// \brief Set the view mode of watching the sharing.
	/// \param mode Specify the view mode. For more details, see \link CustomizedViewShareMode \endlink enum. 
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetViewMode(CustomizedViewShareMode mode) = 0;

	/// \brief Redraw the window of showing the sharing.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	///Call the function when the parent window gets WM_MOVE windows message.
	/// \remarks Call the function to complete the redraw work once received the ICustomizedShareRenderEvent::onWindowMsgNotification() callback event and dealt with WM_MOVE. Otherwise there maybe problem with the view.
	virtual SDKError HandleWindowsMoveMsg() = 0;
	virtual ~ICustomizedShareRender(){}
};


/// \brief The share action callback event.
///
class IShareActionEvent
{
public:
	/// \brief Callback event the moment the user receives the shared content.
	virtual void onSharingContentStartReceiving() = 0;

	/// \brief Callback event when the app receives the window messages from the sharer. This is a list of the window messages.
	///WM_MOUSEMOVE
	///WM_MOUSEENTER
	///WM_MOUSELEAVE
	///WM_LBUTTONDOWN
	///WM_LBUTTONUP
	///WM_RBUTTONUP
	///WM_LBUTTONDBLCLK
	///WM_KEYDOWN
	virtual void onWindowMsgNotification(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	/// \brief The callback is triggered before the share action is destroyed.
	/// \param iSharingID Specify the sharing ID.
	/// \remarks The specified share action is destroyed once the function calls end. The user must complete the operations related to the share action before the function calls end.
	virtual void onActionBeforeDestroyed(const unsigned int iSharingID) = 0;

	virtual ~IShareActionEvent() {}
};

/// \brief The share action interface.
///
class IShareAction : public IShareActionBase
{
public:
	/// \brief Set the share action callback event handler.
	/// \param pEvent A pointer to the IShareActionEvent that receives the share render event. 
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetEvent(IShareActionEvent* pEvent) = 0;

	/// \brief Resize the share action in the specified area and reset the parent window.
	/// \param rc Specify a new display area. The coordinate value of the structure is that of the parent window of share action.
	/// \param hParentWnd Specify a new parent window HWND.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Reposition(RECT rc, HWND hParentWnd = nullptr) = 0;

	/// \brief Get the sharing ID.
	/// \return If the function succeeds, the return value is the sharing ID. 
	///Otherwise the function fails, and the return value is ZERO(0).
	virtual const unsigned int GetSharingID() = 0;

	/// \brief Get the name of the sharing user.
	/// \return If the function succeeds, the return value is the name. 
	///Otherwise the function fails, and the return value is nullptr.
	virtual const zchar_t* GetSharingUserName() = 0;

	/// \brief Subscribe the sharing content.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Subscribe() = 0;

	/// \brief Unsubscribe the sharing content.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Unsubscribe() = 0;

	virtual ~IShareAction() {}
};

END_ZOOM_SDK_NAMESPACE

#endif