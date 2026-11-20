/**
 * @file zoom_sdk_util_define.h
 * @brief SDK utility definition of ZOOM windows.
 */

#ifndef _ZOOM_SDK_UTIL_DEFINE_H_
#define _ZOOM_SDK_UTIL_DEFINE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
/**
 * @class ICameraControllerEvent
 * @brief Camera controller event callback.
 */
class ICameraControllerEvent
{
public:
	/**
	 * @brief Callback event when the controller status changes.
	 * @param valid true if the controller is valid, false otherwise.
	 */
	virtual void onControllerStatusChanged(bool valid) = 0;
};

/**
 * @class ICameraController
 * @brief Camera controller interface.
 */
class ICameraController
{
public:
	virtual ~ICameraController(){}
	
	/**
	 * @brief Sets the event handler for camera controller.
	 * @param pEvent A pointer to the ICameraControllerEvent that receives camera controller events.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetEvent(ICameraControllerEvent* pEvent) = 0;
	
	/**
	 * @brief Determines whether the camera controller is valid.
	 * @return true if the camera controller is valid. Otherwise, false.
	 */
	virtual bool IsValid() = 0;
	/**
	 * @brief Begins turning the camera left.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError BeginTurnLeft() = 0;
	
	/**
	 * @brief Continues turning the camera left.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ContinueTurnLeft() = 0;
	
	/**
	 * @brief Ends turning the camera left.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EndTurnLeft() = 0;
	/**
	 * @brief Begins turning the camera right.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError BeginTurnRight() = 0;
	
	/**
	 * @brief Continues turning the camera right.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ContinueTurnRight() = 0;
	
	/**
	 * @brief Ends turning the camera right.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EndTurnRight() = 0;
	/**
	 * @brief Begins turning the camera up.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError BeginTurnUp() = 0;
	
	/**
	 * @brief Continues turning the camera up.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ContinueTurnUp() = 0;
	
	/**
	 * @brief Ends turning the camera up.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EndTurnUp() = 0;
	/**
	 * @brief Begins turning the camera down.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError BeginTurnDown() = 0;
	
	/**
	 * @brief Continues turning the camera down.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ContinueTurnDown() = 0;
	
	/**
	 * @brief Ends turning the camera down.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EndTurnDown() = 0;
	/**
	 * @brief Begins zooming the camera in to move the camera angle closer to the subject.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError BeginZoomIn() = 0;
	
	/**
	 * @brief Continues zooming the camera in.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ContinueZoomIn() = 0;
	
	/**
	 * @brief Ends zooming the camera in.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EndZoomIn() = 0;
	/**
	 * @brief Begins zooming the camera out to move the camera angle farther from the subject.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError BeginZoomOut() = 0;
	
	/**
	 * @brief Continues zooming the camera out.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ContinueZoomOut() = 0;
	
	/**
	 * @brief Ends zooming the camera out.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError EndZoomOut() = 0;
	/**
	 * @brief Determines whether the camera can be controlled.
	 * @param bCan true if the camera can be controlled, false otherwise.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError CanControlCamera(bool& bCan) = 0;
};
END_ZOOM_SDK_NAMESPACE

#endif