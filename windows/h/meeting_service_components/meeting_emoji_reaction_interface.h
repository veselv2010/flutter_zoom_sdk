/*!
* \file meeting_emoji_reaction_interface.h
* \brief Meeting Service Emoji Reaction Interface. 
* \remarks Valid for both ZOOM style and user custom interface mode.
*/
#ifndef _MEETING_EMOJI_REACTION_INTERFACE_H_
#define _MEETING_EMOJI_REACTION_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/*! \enum SDKEmojiReactionType
	\brief Specify the emoji reaction type.
	Here are more detailed enum descriptions.
*/
typedef enum tagSDKEmojiReactionType
{
	SDKEmojiReactionType_None = 0,
	SDKEmojiReactionType_Clap,
	SDKEmojiReactionType_Thumbsup,
	SDKEmojiReactionType_Heart,
	SDKEmojiReactionType_Joy,
	SDKEmojiReactionType_Openmouth,
	SDKEmojiReactionType_Tada,
}SDKEmojiReactionType;

/*! \enum SDKEmojiReactionDisplayType
	\brief Specify the emoji display type.
	Here are more detailed enum descriptions.
*/
typedef enum tagSDKEmojiReactionDisplayType
{
	SDKEmojiReactionDisplayType_None,
	SDKEmojiReactionDisplayType_Full,
	SDKEmojiReactionDisplayType_Medium,
	SDKEmojiReactionDisplayType_Hidden
}SDKEmojiReactionDisplayType;

/*! \enum SDKEmojiFeedbackType
	\brief Specify the emoji feedback type.
	Here are more detailed enum descriptions.
*/
typedef enum tagSDKEmojiFeedbackType
{
	SDKEmojiFeedbackType_None,
	SDKEmojiFeedbackType_Yes,
	SDKEmojiFeedbackType_No,
	SDKEmojiFeedbackType_SpeedUp,
	SDKEmojiFeedbackType_SlowDown,
	SDKEmojiFeedbackType_Away
}SDKEmojiFeedbackType;

/// \brief Emoji Reaction controller callback event.
///
class IEmojiReactionControllerEvent
{
public:
	virtual ~IEmojiReactionControllerEvent() {}
	/// \brief Emoji reaction callback. This function is used to inform the user once received the reaction sent by others or user himself.
	/// \param sender_id Specify the user id of the reaction sender.
	/// \param type Specify the type of the received reaction.
	virtual void OnEmojiReactionReceived(unsigned int sender_id, SDKEmojiReactionType type) = 0;

	/// \brief Emoji reaction callback. This callback notifies the user when an emoji is received in the webinar.
	/// \param type Specify the type of the received reaction. 
	virtual void OnEmojiReactionReceivedInWebinar(SDKEmojiReactionType type) = 0;

	/// \brief Emoji feedback received callback. This function is used to inform the user once received the emoji feedback sent by others or user himself.
	/// \param sender_id Specify the user id of the emoji feedback sender.
	/// \param type Specify the type of the received emoji feedback.
	virtual void OnEmojiFeedbackReceived(unsigned int sender_id, SDKEmojiFeedbackType type) = 0;

	/// \brief Emoji feedback canceled callback. This function is used to inform the user once the received emoji feedback sent by others or user himself was canceled.
	/// \param sender_id Specify the user id of the emoji feedback sender.
	virtual void OnEmojiFeedbackCanceled(unsigned int sender_id) = 0;
};

/// \brief Emoji Reaction controller interface.
class IEmojiReactionController
{
public:
	/// \brief Configure the meeting emoji reaction controller callback event handler.
	/// \param pEvent An object pointer to the IEmojiReactionControllerEvent that receives the meeting reaction callback event. For more details, see \link IEmojiReactionControllerEvent \endlink.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \remarks The SDK use pEvent to transmit the callback event to the user's application. If the function is not called or failed, the user's application is unabled to retrieve the callback event.
	virtual SDKError SetEvent(IEmojiReactionControllerEvent* pEvent) = 0;
	
	/// \brief Determine if the reactions feature is enabled in the meeting.
	virtual bool IsEmojiReactionEnabled() = 0;
	
	/// \brief Send the reaction.
	/// \param type Specify the reaction type to be sent.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SendEmojiReaction(SDKEmojiReactionType type) = 0;

	/// \brief Set emoji reaction display type in webinar.
    /// \param type The display type of the emoji reaction.
    /// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetWebinarEmojiReactionDisplayType(SDKEmojiReactionDisplayType type) = 0;

	/// \brief Get emoji reaction display type in webinar.
	/// \return If the function succeeds, it will return the display type of the emoji reaction, otherwise returns SDKEmojiReactionDisplayType_None.
	virtual SDKEmojiReactionDisplayType GetWebinarEmojiReactionDisplayType() = 0;

	/// \brief Send the emoji feedback.
	/// \param type Specify the emoji feedback type to be sent.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SendEmojiFeedback(SDKEmojiFeedbackType type) = 0;

	/// \brief Cancel the emoji feedback.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError CancelEmojiFeedback() = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif