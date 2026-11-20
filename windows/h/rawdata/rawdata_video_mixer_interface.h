/**
 * @file rawdata_video_mixer_interface.h
 * @brief Mixed meeting video raw data interface.
 */
#ifndef _RAWDATA_VIDEO_MIXER_INTERFACE_H_
#define _RAWDATA_VIDEO_MIXER_INTERFACE_H_

#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

enum ZoomSDKMixedVideoLayoutMode
{
	ZoomSDKMixedVideoLayoutMode_None,
	ZoomSDKMixedVideoLayoutMode_Gallery,
	ZoomSDKMixedVideoLayoutMode_ActiveSpeaker,
};

class IZoomSDKVideoMixerDelegate
{
public:
	virtual ~IZoomSDKVideoMixerDelegate() {}
	virtual void onMixedVideoFrameReceived(unsigned char* pDataBuff, unsigned int nDataLen,
		unsigned int nWidth, unsigned int nHeight) = 0;
};

class IZoomSDKVideoMixer
{
public:
	virtual ~IZoomSDKVideoMixer() {}
	virtual SDKError StartMixedVideo(ZoomSDKMixedVideoLayoutMode mode, unsigned int width,
		unsigned int height, IZoomSDKVideoMixerDelegate* delegate) = 0;
	virtual SDKError StopMixedVideo() = 0;
	virtual SDKError SetLayoutMode(ZoomSDKMixedVideoLayoutMode mode) = 0;
};

END_ZOOM_SDK_NAMESPACE

#endif
