/*!
* \file meeting_indicator_interface.h
* \brief Meeting Service Indicator Interface
*/
#ifndef _MEETING_INDICATOR_INTERFACE_H_
#define _MEETING_INDICATOR_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
/// \brief Meeting Indicator controller interface.
///
class IMeetingIndicatorHandler
{
public:
	virtual ~IMeetingIndicatorHandler() {}

	/// \brief get Indicator item ID.
	/// \return The item ID of the Indicator.
	virtual const zchar_t* GetIndicatorItemId() = 0;

	/// \brief get Indicator name.
	/// \return The name of the Indicator.
	virtual const zchar_t* GetIndicatorName() = 0;

	/// \brief get Indicator Icon URL.
	/// \return The URL of the Indicator.
	virtual const zchar_t* GetIndicatorIcon() = 0;

	/// \brief Show Indicator panel window.
	/// \param x: the horizontal coordinate value. 
	/// \param y: the coordinate coordinate value. 
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError ShowIndicatorPanel(unsigned int x, unsigned int y) = 0;

	/// \brief Hide Indicator panel window.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise the function fails. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError HideIndicatorPanel() = 0;
};



/// \brief Indicator meeting controller callback event.
///
class IMeetingIndicatorCtrlEvent {
public:
	virtual ~IMeetingIndicatorCtrlEvent() {}

	/// \brief Notify receive Callback event IMeetingIndicatorHandler received.
	/// \param handler The handle to show or hide the indicator panel.
	virtual void onIndicatorItemReceived(IMeetingIndicatorHandler* handler) = 0;

	/// \brief Notify receive Callback event IMeetingIndicatorHandler removed.
	/// \param handler The handle will be removed. Do not use it any more.
	virtual void onIndicatorItemRemoved(IMeetingIndicatorHandler* handler) = 0;
};

/// \brief Meeting Indicator controller interface.
///
class IMeetingIndicatorController
{
public:
	virtual ~IMeetingIndicatorController() {}

    /// \brief Set the meeting Indicator controller callback event handler.
    /// \param pEvent A pointer to the IMeetingIndicatorCtrlEvent that receives the meeting Indicator event. For more details, see \link IMeetingIndicatorCtrlEvent \endlink.
    /// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetEvent(IMeetingIndicatorCtrlEvent* pEvent) = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif