/**
 * @file zoom_sdk_ext.h
 * @brief ZOOM SDK Embedded Browser Interface.
 */

#ifndef _ZOOM_SDK_EXT_H_
#define _ZOOM_SDK_EXT_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
extern "C"
{
	class IUIHooker;
	class ICustomizedResourceHelper;
	
	/**
	 * @brief Retrieves user hooker interface.
	 * @param ppUIHooker A pointer to the IUIHooker*. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS, and ppUIHooker is not nullptr. Otherwise, this function returns an error.
	 */
	SDK_API SDKError RetrieveUIHooker(IUIHooker** ppUIHooker);
	
	/**
	 * @brief Retrieves customized resource helper interface.
	 * @param ppCustomiezedResourceHelper A pointer to the ICustomizedResourceHelper*. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS, and ppCustomiezedResourceHelper is not nullptr. Otherwise, this function returns an error.
	 */
	SDK_API SDKError RetrieveCustomizedResourceHelper(ICustomizedResourceHelper** ppCustomiezedResourceHelper);
}

END_ZOOM_SDK_NAMESPACE
#endif