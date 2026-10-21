/**
 * @file zoom_customized_ui.h
 * @brief ZOOM Custom UI interface.
 */
#ifndef _ZOOM_CUSTOMIZED_UI_H_
#define _ZOOM_CUSTOMIZED_UI_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
class ICustomizedUIMgr;
extern "C"
{
	/**
	 * @brief Create a custom UI manager interface.
	 * @param ppCustomizedUIMgr The parameter stores the pointer to ICustomizedUIMgr* once the function calls successfully. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS, while the return value of ppCustomizedUIMgr is not nullptr. Otherwise, this function returns an error.
	 */
	SDK_API SDKError CreateCustomizedUIMgr(ICustomizedUIMgr** ppCustomizedUIMgr);
	
	/**
	 * @brief Destroy the specified custom UI manager interface.
	 * @param pCustomizedUIMgr A pointer to the ICustomizedUIMgr to be destroyed. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	SDK_API SDKError DestroyCustomizedUIMgr(ICustomizedUIMgr* pCustomizedUIMgr);
}

END_ZOOM_SDK_NAMESPACE
#endif