/**
 * @file zoom_sdk_raw_data_def.h
 * @brief ZOOM SDK Raw Data Common Definition File.
 */
#ifndef _ZOOM_SDK_RAW_DATA_DEF_H_
#define _ZOOM_SDK_RAW_DATA_DEF_H_
/**
 * @brief The audio raw data handler interface.
 */
class AudioRawData
{
public:
	/**
	 * @brief Determine if the reference count can be increased.
	 * @return TRUE indicates to the reference count can be increased.
	 */
	virtual bool CanAddRef() = 0;
	
	/**
	 * @brief Add one to the reference count.
	 * @return If the function succeeds, the return value is TRUE.
	 */
	virtual bool AddRef() = 0;
	
	/**
	 * @brief Subtract one from the reference count.
	 * @return The current reference count. If the currrent reference count is 0, the SDK will delete this object instance.
	 */
	virtual int Release() = 0;
	
	/**
	 * @brief Get the audio raw data.
	 * @return A pointer to the audio raw data.
	 */
	virtual char* GetBuffer() = 0;
	
	/**
	 * @brief Get the buffer length of the audio raw data.
	 * @return The length of the audio raw data.
	 */
	virtual unsigned int GetBufferLen() = 0;
	
	/**
	 * @brief Get the sample rate of the audio raw data.
	 * @return The sample rate of the audio raw data.
	 */
	virtual unsigned int GetSampleRate() = 0;
	
	/**
	 * @brief Get the channel number of the audio raw data.
	 * @return The channel number of the audio raw data.
	 */
	virtual unsigned int GetChannelNum() = 0;
	
	/**
	 * @brief Get the timestamp of the raw data.
	 * @return Millisecond timestamp.
	 */
	virtual unsigned long long GetTimeStamp() = 0;

	virtual ~AudioRawData(){}
};

/**
 * @brief Face region of interest reported alongside a decoded frame.
 *
 * Coordinates are normalized to [0.0, 1.0], with the origin at the top-left
 * corner of the decoded (pre-rotation) frame. Callers that need display
 * coordinates must apply the frame rotation reported by
 * @ref YUVRawDataI420::GetRotation().
 */
struct FaceROI
{
	float left;     ///< Normalized left edge,   [0.0, 1.0].
	float top;      ///< Normalized top edge,    [0.0, 1.0].
	float right;    ///< Normalized right edge,  [0.0, 1.0], > left.
	float bottom;   ///< Normalized bottom edge, [0.0, 1.0], > top.
};

/**
 * @brief Per-frame face ROI metadata accessor.
 *
 * Face ROI data is generated on the sender's device and embedded in the
 * video stream. The receiver SDK only surfaces what the sender provides;
 * no face detection is performed locally. As a result, individual
 * participants may deliver frames with zero detected faces — for example,
 * when their device does not support face detection or when detection is
 * not currently active on their end.
 *
 * The lifetime of an @ref IFaceROIInfo instance is bound to its host
 * @ref YUVRawDataI420 object - callers must NOT use it after the host frame
 * is released. To use ROI data asynchronously, call @c AddRef() on the host
 * @ref YUVRawDataI420 object first.
 *
 * Implementations always return a valid (non-null) object from
 * @ref YUVRawDataI420::GetFaceROIInfo(); inspect @ref GetFaceCount() to
 * determine whether any face data was included.
 *
 * @note Face ROI is only supported on video streams (@c RAW_DATA_TYPE_VIDEO).
 */
class IFaceROIInfo
{
public:
	/**
	 * @brief Number of valid face ROIs available for the current frame.
	 *
	 * When multiple faces are present, the ROIs are ordered by the area of the
	 * detected region in descending order — index 0 corresponds to the largest
	 * (typically closest or most prominent) face.
	 *
	 * @return The face count, in the range [0, 50]. Zero indicates that
	 *         no face was detected, the sender has not enabled detection,
	 *         or face metadata was not delivered for this frame.
	 */
	virtual unsigned int GetFaceCount() = 0;

	/**
	 * @brief Get a face ROI by zero-based index.
	 *
	 * Faces are sorted by detected region area in descending order; index 0
	 * is always the largest face in the frame.
	 *
	 * @param index The zero-based index, must be < GetFaceCount().
	 * @return Pointer to the @ref FaceROI for the given index, or @c nullptr
	 *         if @p index is out of range. The returned pointer is valid for
	 *         the lifetime of this @ref IFaceROIInfo object.
	 */
	virtual const FaceROI* GetFaceROIByIndex(unsigned int index) = 0;

	virtual ~IFaceROIInfo() {}
};

/**
 * @brief The YUV raw data handler interface.
 */
class YUVRawDataI420
{
public:
	/**
	 * @brief Determine if the reference count can be increased.
	 * @return TRUE indicates to the reference count can be increased.
	 */
	virtual bool CanAddRef() = 0;
	
	/**
	 * @brief Add one to the reference count.
	 * @return If the function succeeds, the return value is TRUE.
	 * @note
	 *  After increasing the reference count, the current object will not be automatically destroyed by SDK and you can use this object in other context.
	 *  When you no longer use this object, please call 'Release()', otherwise there will be a memory leak.
	 */
	virtual bool AddRef() = 0;
	
	/**
	 * @brief Subtract one from the reference count.
	 * @return The current reference count. If the current reference count is 0, the SDK will delete this object instance.
	 */
	virtual int Release() = 0;
	
	/**
	 * @brief Get the starting address of Y raw data.
	 * @return A pointer to the Y raw data.
	 */
	virtual char* GetYBuffer() = 0;
	
	/**
	 * @brief Get the starting address of U raw data.
	 * @return A pointer to the U raw data.
	 */
	virtual char* GetUBuffer() = 0;
	
	/**
	 * @brief Get the starting address of V raw data.
	 * @return A pointer to the V raw data.
	 */
	virtual char* GetVBuffer() = 0;
	
	/**
	 * @brief Get the alpha mask data of YUV raw data.
	 * @return A pointer to the alpha mask data.
	 */
	virtual char* GetAlphaBuffer() = 0;
	
	/**
	 * @brief Get the starting address of YUV raw data.
	 * @return A pointer to the YUV raw data.
	 */

	virtual char* GetBuffer() = 0;
	
	/**
	 * @brief Get the buffer length of the YUV raw data.
	 * @return The length of the YUV raw data.
	 */
	virtual unsigned int GetBufferLen() = 0;
	
	/**
	 * @brief Get the alpha mask data length.
	 * @return The length of alpha mask data.
	 */
	virtual unsigned int GetAlphaBufferLen() = 0;
	
	/**
	 * @brief Determine if the YUV raw data is limited range mode.
	 * @return True indicates to the YUV raw data is limited range mode.
	 */
	virtual bool IsLimitedI420() = 0;
	
	/**
	 * @brief Get the stream width.
	 * @return The stream width.
	 */
	virtual unsigned int GetStreamWidth() =0;
	
	/**
	 * @brief Get the stream height.
	 * @return The stream height.
	 */
	virtual unsigned int GetStreamHeight() = 0;
	
	/**
	 * @brief Get the rotation angle of the local video device.
	 * @return If the function succeeds, the return is enumerated in LocalVideoDeviceRotation enum 
	 */
	virtual unsigned int GetRotation() = 0;
	
	/**
	 * @brief Get the source_id of the current YUV raw data.
	 * @return The source_id.
	 */
	virtual unsigned int GetSourceID() = 0;
	
	/**
	 * @brief Get the timestamp of the raw data.
	 * @return Millisecond timestamp.
	 */
	virtual unsigned long long GetTimeStamp() = 0;

	/**
	 * @brief Get the face ROI metadata associated with this frame.
	 *
	 * Implementations always return a valid (non-null) object. Callers should
	 * inspect @ref IFaceROIInfo::GetFaceCount() to determine whether any face
	 * was reported. The returned object's lifetime is bound to this
	 * @ref YUVRawDataI420 instance - to use it asynchronously, retain the
	 * frame via @ref AddRef() first.
	 *
	 * @return Pointer to the face ROI accessor; never null.
	 */
	virtual IFaceROIInfo* GetFaceROIInfo() = 0;

	virtual ~YUVRawDataI420(){}
};

class YUVProcessDataI420
{
public:
	virtual unsigned int GetWidth() =0;
	virtual unsigned int GetHeight() = 0;

	virtual char* GetYBuffer(unsigned int lineNum = 0) = 0;
	virtual char* GetUBuffer(unsigned int lineNum = 0) = 0;
	virtual char* GetVBuffer(unsigned int lineNum = 0) = 0;

	virtual unsigned int GetYStride() = 0;
	virtual unsigned int GetUStride() = 0;
	virtual unsigned int GetVStride() = 0;

	virtual unsigned int GetRotation() = 0;
	virtual bool IsLimitedI420() = 0;
	virtual ~YUVProcessDataI420(){}
};

/**
 * @brief I420 YUV raw data converter interface.
 */
class IYUVRawDataI420Converter
{
public:
	/**
	 * @brief Convert the intermediate YUV raw data to YUV raw data.
	 * @return The YUV raw data. A pointer to YUVRawDataI420.
	 */
	virtual YUVRawDataI420* ConvertToYUV() = 0;
	
	/**
	 * @brief Convert the intermediate YUV raw data to YUV raw data via external buffer.
	 * @param buffer Specifies the external buffer.
	 * @param size_ Specifies the size of the YUV raw data.
	 * @return The YUV raw data. A pointer to YUVRawDataI420.
	 */
	virtual YUVRawDataI420* ConvertToYUVViaExternalBuffer(char* buffer_, int size_) = 0;
	
	/**
	 * @brief Fill data to the pixel buffer if expanding the size of the intermediate YUV raw data.
	 * @param ybuffer_ Specifies the y-data of intermediate YUV raw data.
	 * @param ybuffer_pre_row_bytes Specifies the width of the y-data.
	 * @param uvbuffer_ Specifies the uv-data of intermediate YUV raw data.
	 * @param uvbuffer_pre_row_bytes Specifies the width of the uv-data.
	 * @param width Specifies the new width to extended.
	 * @param height Specifies the new height to extended.
	 */
	virtual void FillToPixelBuffer(char* ybuffer_, int ybuffer_pre_row_bytes, char* uvbuffer_, int uvbuffer_pre_row_bytes,int width, int height) = 0;

	virtual ~IYUVRawDataI420Converter(){}
};

#endif