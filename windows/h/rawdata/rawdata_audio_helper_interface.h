/**
 * @file rawdata_audio_helper_interface.h
 * @brief Raw Data Audio Helper Interface.
 */
#ifndef _RAWDATA_AUDIO_HELPER_INTERFACE_H_
#define _RAWDATA_AUDIO_HELPER_INTERFACE_H_
#include "zoom_sdk_def.h"

class AudioRawData;

BEGIN_ZOOM_SDK_NAMESPACE
class IZoomSDKAudioRawDataDelegate
{
public:
	~IZoomSDKAudioRawDataDelegate(){}
	virtual void onMixedAudioRawDataReceived(AudioRawData* data_) = 0;
	virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, uint32_t user_id) = 0;
	virtual void onShareAudioRawDataReceived(AudioRawData* data_, uint32_t user_id) = 0;
	/**
	 * @brief Invoked when an individual interpreter's raw audio data is received.
	 * @param data_ Raw audio data.
	 * @param pLanguageName The pointer to interpreter language name.
	 */
	virtual void onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName) = 0;
};

class IZoomSDKAudioRawDataSender
{
public:
	virtual ~IZoomSDKAudioRawDataSender() {}
	/**
	 * @brief Sends audio raw data. Audio sample must be 16-bit audio.
	 * @param data the audio data's address.
	 * @param data_length the audio data's length. Must be an even number.
	 * @param sample_rate the audio data's sampling rate.
	 *  When the channel is mono, supported sample rates: 8000/11025/16000/32000/44100/48000/50000/50400/96000/192000/2822400
	 *  When the channel is stereo, supported sample rates: 8000/16000/32000/44100/48000/50000/50400/96000/192000
	 * @note
	 * 1. The size of a single input must not exceed the number of bytes corresponding to 100 ms of audio. Exceeding this limit returns SDKERR_SEND_TOO_MUCH_DATA_IN_SINGLE_TIME.
	 * 2. Variable-length input is supported; it is recommended to send audio data in 10 ms, 20 ms, or 30 ms chunks (not exceeding 100 ms).
	 * 3. If SDKERR_TOO_FREQUENT_CALL is returned, this is not a genuine error; please wait briefly before retrying—for example, sleep for 10 ms before attempting to send the data again.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError send(char* data, unsigned int data_length, int sample_rate, ZoomSDKAudioChannel channel = ZoomSDKAudioChannel_Mono) = 0;
};

class IZoomSDKVirtualAudioMicEvent
{
public:
	virtual ~IZoomSDKVirtualAudioMicEvent() {}
	/**
	 * @brief Callback for virtual audio mic to do some initialization.
	 * @param pSender, You can send audio data based on this object.
	 */
	virtual void onMicInitialize(IZoomSDKAudioRawDataSender* pSender) = 0;
	/**
	 * @brief Callback for virtual audio mic can send raw data with 'pSender'.
	 */
	virtual void onMicStartSend() = 0;
	/**
	 * @brief Callback for virtual audio mic should stop send raw data.
	 */
	virtual void onMicStopSend() = 0;
	/**
	 * @brief Callback for virtual audio mic is uninitialized.
	 */
	virtual void onMicUninitialized() = 0;
};

class IZoomSDKAudioRawDataHelper
{
public:
	virtual ~IZoomSDKAudioRawDataHelper(){}
	/**
	 * @brief Subscribe raw audio data.
	 * @param pDelegate Callback handler. Different source requires a different delegate.
	 * @param bWithInterpreters If true, also receive interpreter raw audio.
	 * @param source CurrentSession (default) or MainSession (webinar backstage only).
	 * @note if bWithInterpreters is true, it will cause your local interpreter related functions to be unavailable.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise fails.
	 */
	virtual SDKError subscribe(IZoomSDKAudioRawDataDelegate* pDelegate, bool bWithInterpreters = false,
		ZoomSDKSessionSource source = SessionSource_CurrentSession) = 0;
	/**
	 * @brief Stop receiving raw audio data for one source.
	 * @param source CurrentSession (default) or MainSession. One value per call.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise fails.
	 */
	virtual SDKError unSubscribe(ZoomSDKSessionSource source = SessionSource_CurrentSession) = 0;
	/**
	 * @brief Subscribe audio mic raw data with a callback.
	 * @param pSource, Callback sink object.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError setExternalAudioSource(IZoomSDKVirtualAudioMicEvent* pSource) = 0;
};
END_ZOOM_SDK_NAMESPACE
#endif