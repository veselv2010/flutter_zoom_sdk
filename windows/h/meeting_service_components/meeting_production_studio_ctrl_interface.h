/**
 * @file meeting_production_studio_ctrl_interface.h
 * @brief Meeting Service production studio Interface.
 */
#ifndef _MEETING_PRODUCTION_STUDIO_CTRL_INTERFACE_H_
#define _MEETING_PRODUCTION_STUDIO_CTRL_INTERFACE_H_
#include "zoom_sdk_def.h"
#include "rawdata/rawdata_video_source_helper_interface.h"
#include "rawdata/rawdata_audio_helper_interface.h"

BEGIN_ZOOM_SDK_NAMESPACE

typedef struct tagPSVideoSourceCapability
{
	unsigned int width;
	unsigned int height;
	FrameDataFormat format;
	tagPSVideoSourceCapability()
	{
		width = 0;
		height = 0;
		format = FrameDataFormat_I420_FULL;
	}
}PSVideoSourceCapability;


class IZoomSDKPSSender
{
public:
	virtual ~IZoomSDKPSSender() {}
	/**
	 * @brief Sends video raw data.
	 * @param frameBuffer The video data address.
	 * @param format The video data format.
	 * @note The params width, height, format should be consistent with param capability of StartPSMode. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError sendVideoFrame(char* frameBuffer, int width, int height, int frameLength, FrameDataFormat format = FrameDataFormat_I420_FULL) = 0;
	
	/**
	 * @brief Sends audio raw data.
	 * @param data The audio data address.
	 * @param data_length The audio data length. Must be an even number.
	 * @param sample_rate The audio data sampling rate. Support 32k and 48k. 48k is recommended.
	 * @param channelNum Channel number.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError sendAudio(char* data, unsigned int data_length, int sample_rate, ZoomSDKAudioChannel channelNum) = 0;
};
/**
 * @class IMeetingProductionStudioCtrlEvent
 * @brief Production studio controller callback event.
 */
class IMeetingProductionStudioCtrlEvent
{
public:
	virtual ~IMeetingProductionStudioCtrlEvent() {}
	
	/**
	 * @brief Callback event notify the result of starting production studio mode.
	 * @param bSuccess Specify whether the production studio mode is started successfully or not.
	 */
	virtual void onStartPSModeResult(bool bSuccess) = 0;
	
	/**
	 * @brief Callback event for production studio starts.
	 * @param sender You can send video or audio data based on this object.
	 */
	virtual void onStartSend(IZoomSDKPSSender* sender) = 0;
	
	/**
	 * @brief Callback event for production studio stops.
	 */
	virtual void onStopSend() = 0;
	
	/**
	 * @brief Callback event of production studio user status changes.
	 * @param userID The user ID of production studio user.
	 * @param bStart The production studio mode stated.
	 */
	virtual void onPSUserStatusChanged(unsigned int userID, bool bStart) = 0;
};
/**
 * @class IMeetingProductionStudioController
 * @brief Production studio controller interface.
 */
class IMeetingProductionStudioController
{
public:
	/**
	 * @brief Configure the meeting production studio controller callback event handler.
	 * @param pEvent An object pointer to the IMeetingProductionStudioCtrlEvent that receives the meeting production studio callback event.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note The SDK use pEvent to transmit the callback event to the user's application. If the function is not called or fails, the user's application is unable to retrieve the callback event.
	 */
	virtual SDKError SetEvent(IMeetingProductionStudioCtrlEvent* pEvent) = 0;
	
	/**
	 * @brief Determines whether the current meeting supports the production mode or not.
	 * @return true indicates to support.
	 */
	virtual bool IsSupportPSMode() = 0;
	
	/**
	 * @brief Determines whether you have permission to start production studio. Only host or co-host can start production studio mode. 
	 * @return true indicates you can start production studio mode.
	 */
	virtual bool CanStartPSMode() = 0;
	
	/**
	 * @brief Starts production studio mode.  
	 * @param capability Specify the format description of the video data send.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError StartPSMode(PSVideoSourceCapability capability) = 0;
	
	/**
	 * @brief Stops production studio mode.  
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError StopPSMode() = 0;
	
	/**
	 * @brief Gets the user ID of current production studio user.
	 */
	virtual unsigned int GetPSUserID() = 0;
	
	/**
	 * @brief Determines whether the current meeting production mode has started.
	 * @return true indicates to started.
	 */
	virtual bool IsPSModeStarted() = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif