/*!
* \file customized_ui_mgr.h
* \brief ZOOM Custom UI Manager Interface.
* 
*/
#ifndef _ZOOM_CUSTOMIZED_UI_MGR_H_
#define _ZOOM_CUSTOMIZED_UI_MGR_H_
#include "zoom_sdk_def.h"
#include "customized_ui\customized_immersive_container.h"

BEGIN_ZOOM_SDK_NAMESPACE
class ICustomizedVideoContainer;
class ICustomizedShareRender;

/// \brief The callback event of custom UI manager interface.
///
class ICustomizedUIMgrEvent
{
public:
	/// \brief The callback will be triggered before the video container is destroyed.
	/// \param pContainer Specify the video container to be destroyed.
	/// \remarks The specified container will be destroyed once the function calls end. The user should complete the operations related to the video container before the function calls end. 
	virtual void onVideoContainerDestroyed(ICustomizedVideoContainer* pContainer) = 0;

	/// \brief The callback will be triggered before the shared render is destroyed.
	/// \param pRender Specify the shared render to be destroyed.
	/// \remarks The specified shared render will be destroyed once the function calls end. The user should complete the operations related to the shared render before the function calls end.
	virtual void onShareRenderDestroyed(ICustomizedShareRender* pRender) = 0;

	/// \brief The callback will be triggered before the immersive container is destroyed.
	/// \remarks The immersive container will be destroyed once the function calls end. The user should complete the operations related to the immersive container before the function calls end. 
	virtual void onImmersiveContainerDestroyed() = 0;

	virtual	~ICustomizedUIMgrEvent() {};
};


/// \brief Custom UI manager interface.
///
class ICustomizedUIMgr
{
public:
	/// \brief Determine if the user owns the license to enable the custom UI module.
	/// \return If the user owns the license, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError HasLicense() = 0;

	/// \brief Set custom UI manager interface callback event handler.
	/// \param pEvent A pointer to the ICustomizedUIMgrEvent that receives user manager custom interface event. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetEvent(ICustomizedUIMgrEvent* pEvent) = 0;

	/// \brief Create the video container.
	/// \param ppContainer Once called the function successfully, this parameter will store the pointer to the ICustomizedVideoContainer*.
	/// \param hParentWnd Specify the parent window of the video container.
	/// \param rc Specify the position of client area of the video render elements.
	///The client coordinates corresponds to the client area of the upper-left corner of the parent window.
	/// \return If the function succeeds, the return value is SDKErr_Success, and the return value of ppContainer is not nullptr.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError CreateVideoContainer(ICustomizedVideoContainer** ppContainer, HWND hParentWnd, RECT rc) = 0;

	/// \brief Destroy the specified video container.
	/// \param pContainer A pointer to the ICustomizedVideoContainer to be destroyed. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError DestroyVideoContainer(ICustomizedVideoContainer* pContainer) = 0;

	/// \brief Destroy all the video containers.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError DestroyAllVideoContainer() = 0;

	/// \brief Create shared render.
	/// \param ppRender Once the function is called successfully, this parameter will store the pointer to the ICustomizedShareRender*.
	/// \param hParentWnd Specify the parent window of the shared render.
	/// \param rc Specify the position of client area of the shared render.
	///The client coordinates corresponds to the client area of the upper-left corner of the parent window.
	/// \return If the function succeeds, the return value is SDKErr_Success, and the return value of ppRender is not nullptr.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError CreateShareRender(ICustomizedShareRender** ppRender, HWND hParentWnd, RECT rc) = 0;

	/// \brief Destroy the specified shared render.
	/// \param pRender A pointer to the ICustomizedShareRender to be destroyed. 
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError DestroyShareRender(ICustomizedShareRender* pRender) = 0;

	/// \brief Destroy all the shared renders.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError DestroyAllShareRender() = 0;

	/// \brief Create the immersive container.
	/// \param ppContainer Once called the function successfully, this parameter will store the pointer to the ICustomizedImmersiveContainer*.
	/// \param hParentWnd Specify the parent window of the immersive container.
	/// \param rc Specify the position of client area of the immersive container.
	///The client coordinates corresponds to the client area of the upper-left corner of the parent window.
	///Only allow creating an object before DestroyImmersiveContainer.
	/// \return If the function succeeds, the return value is SDKErr_Success, and the return value of ppContainer is not nullptr.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError CreateImmersiveContainer(ICustomizedImmersiveContainer** ppContainer, HWND hParentWnd, RECT rc) = 0;

	/// \brief Destroy the immersive container. Once destroyed, the container can no longer be used.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError DestroyImmersiveContainer() = 0;

	virtual ~ICustomizedUIMgr() {}
};
END_ZOOM_SDK_NAMESPACE
#endif