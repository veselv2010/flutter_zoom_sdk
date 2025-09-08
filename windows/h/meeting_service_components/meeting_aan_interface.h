/**
 * @file meeting_aan_interface.h
 * @brief show/hide AAN window.
 * @note Valid for customUI mode.
 */
#ifndef _MEETING_AAN_INTERFACE_H_
#define _MEETING_AAN_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/**
 * @class IMeetingAANController
 * @brief Meeting AAN controller interface.
 */
class IMeetingAANController
{
public:
    /**
	 * @brief Show AAN panel window.
	 * @param x The horizontal coordinate value. 
	 * @param y the coordinate coordinate value. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise failed. For detailed error codes, see \link SDKError \endlink enum.
	 */
	virtual SDKError ShowAANPanel(unsigned int x, unsigned int y) = 0;

    /**
	 * @brief Hide AAN panel window.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise failed. For detailed error codes, see \link SDKError \endlink enum.
	 */
	virtual SDKError HideAANPanel() = 0;

    /**
	 * @brief Show dynamic notice for AI Companion panel window.
	 * @param x The horizontal coordinate value. 
	 * @param y The coordinate coordinate value. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise failed. For detailed error codes, see \link SDKError \endlink enum.
	 */
	virtual SDKError ShowDynamicNoticeForAICompanionPanel(unsigned int x, unsigned int y) = 0;

    /**
	 * @brief Hide dynamic notice for AI Companion panel window.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise failed. For detailed error codes, see \link SDKError \endlink enum.
	 */
	virtual SDKError HideDynamicNoticeForAICompanionPanel() = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif