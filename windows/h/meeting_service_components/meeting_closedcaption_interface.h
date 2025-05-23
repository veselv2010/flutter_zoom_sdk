/*!
* \file meeting_claosecaptioin_interface.h
* \brief Meeting Service Closed Caption Interface.
* \remarks Valid for both ZOOM style and user custom interface mode.
*/
#ifndef _MEETING_CLOSEDCAPTION_INTERFACE_H_
#define _MEETING_CLOSEDCAPTION_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
/**
 * @brief Enumerations of the type for live transcription status.
 */
typedef enum
{
	SDK_LiveTranscription_Status_Stop = 0,   ///<not start
	SDK_LiveTranscription_Status_Start = 1,  ///<start
	SDK_LiveTranscription_Status_User_Sub = 2,  ///<user sub
	SDK_LiveTranscription_Status_Connecting = 10,	 ///<connecting
}SDKLiveTranscriptionStatus;

/**
 * @brief Enumerations of the type for live transcription operation type.
 */
typedef enum
{
	SDK_LiveTranscription_OperationType_None = 0,
	SDK_LiveTranscription_OperationType_Add,
	SDK_LiveTranscription_OperationType_Update,
	SDK_LiveTranscription_OperationType_Delete,
	SDK_LiveTranscription_OperationType_Complete,
	SDK_LiveTranscription_OperationType_NotSupported,
}SDKLiveTranscriptionOperationType;

/// \brief live transcription language interface.
///
class ILiveTranscriptionLanguage
{
public:
	virtual int GetLTTLanguageID() = 0;
	virtual const zchar_t* GetLTTLanguageName() = 0;
	virtual ~ILiveTranscriptionLanguage() {};
};

/// \brief live transcription message interface.
///
class ILiveTranscriptionMessageInfo
{
public:
	/// \brief Get the message ID of the current message.
	/// \return If the function succeeds, the return value is the message ID of the current message.
	///Otherwise it fails, and the return value is the string of length zero(0)
	virtual const zchar_t* GetMessageID() = 0;

	/// \brief Get the speaker's ID.
	/// \return The user object's speaker ID.
	virtual unsigned int GetSpeakerID() = 0;

	/// \brief Get the speaker's name.
	/// \return The user object's speaker name.
	virtual const zchar_t* GetSpeakerName() = 0;

	/// \brief Get the content of the current message.
	/// \return The current message's content.
	virtual const zchar_t* GetMessageContent() = 0;

	/// \brief Get the timestamp of the current message.
	/// \return The current message's timestamp.
	virtual time_t GetTimeStamp() = 0;

	/// \brief Get the type of the current message.
	/// \return The current message's type.
	virtual SDKLiveTranscriptionOperationType GetMessageOperationType() = 0;

	virtual ~ILiveTranscriptionMessageInfo() {};
};

/// \The helper to handle the requested of start captions.
/// When IsRequestTranslationOn is true, use ICCRequestTranslationOnHandler::ApproveStartCaptionsRequest() to approve start captions request.
/// When isRequestTranslationOn is false, use ICCRequestTranslationOffHandler::ApproveStartCaptionsRequest(int languageId) to approve start captions request.
class ICCRequestHandler
{
public:
	/// \return True means request to start captions with translation on. Otherwise false.
	virtual bool IsRequestTranslationOn() = 0;

	/// \return The user ID of the user who send request.
	virtual unsigned int GetSenderUserId() = 0;

	/// \brief Decline the request to start captions.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError Deny() = 0;

	virtual ~ICCRequestHandler() {};
};

class ICCRequestTranslationOnHandler : public ICCRequestHandler
{
public:
	/// \brief Approve the start captions request.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError ApproveStartCaptionsRequest() = 0;

	virtual ~ICCRequestTranslationOnHandler() {};
};

class ICCRequestTranslationOffHandler : public ICCRequestHandler
{
public:
	/// \brief Approve the start captions request.
	/// \param languageId The language to be set for all participants in meeting.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError ApproveStartCaptionsRequest(int languageId) = 0;

	virtual ~ICCRequestTranslationOffHandler() {};
};


/// \brief Closed Caption controller callback event.
///
class IClosedCaptionControllerEvent
{
public:
	virtual ~IClosedCaptionControllerEvent() {}

	/// \brief Callback event when the user is assigned to send Closed Caption messages.
	virtual void onAssignedToSendCC(bool bAssigned) = 0;

	/// \brief Callback event when the user receives Closed Caption messages.
	/// \If the meeting supports multi-language transcription and the host sets manual captions to true,
	/// \attendees must set the translation language id to -1 to receive closed captioned messages.
	/// \param ccMsg: an object pointer to the closed caption message content. 
	/// \param sender_id: the sender id of a closed caption message. 
	/// \param time: the time when a caption message was sent.
	virtual void onClosedCaptionMsgReceived(const zchar_t* ccMsg, unsigned int sender_id, time_t time) = 0;

	/// \brief live transcription status changed callback.
	/// \param status: the live transcription status. For more details, see \link SDKLiveTranscriptionStatus \endlink.
	virtual void onLiveTranscriptionStatus(SDKLiveTranscriptionStatus status) = 0;

	/// \brief original language message received callback.
	/// \param messageInfo The spoken language message, see \link ILiveTranscriptionMessageInfo \endlink.
	virtual void onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo) = 0;

	/// \brief live transcription message received callback.
	/// \param messageInfo The live transcription message, see \link ILiveTranscriptionMessageInfo \endlink.
	virtual void onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) = 0;

	/// \brief The translation message error callback.
	/// \param speakingLanguage: an object of the spoken message language. 
	/// \param transcriptLanguageId: an object of the message language you want to translate.
	virtual void onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage) = 0;

	/// \brief only host can receive this callback.
	/// \param requester_id: the request user id, if param bAnonymous is true, the requester_id is 0, no meanings. 
	/// \param bAnonymous: specify the request whether user anonymous request.
	virtual void onRequestForLiveTranscriptReceived(unsigned int requester_id, bool bAnonymous) = 0;

	/// \brief request live transcription status changed callback.
	/// \param bEnabled Specify the request live transcription status is enable, true enable, false disabled.
	virtual void onRequestLiveTranscriptionStatusChange(bool bEnabled) = 0;

	/// \brief Sink the event of captions enabled status changed.
	/// \param bEnabled True means the host enables the captions, otherwise means the host disables the captions.
	virtual void onCaptionStatusChanged(bool bEnabled) = 0;

	/// \brief Sink the event to start captions request.
	/// \param handler A pointer to the ICCRequestHandler. For more details, see \link ICCRequestHandler \endlink.
	virtual void onStartCaptionsRequestReceived(ICCRequestHandler* handler) = 0;

	/// \brief Sink the event to start captions request was approved.
	virtual void onStartCaptionsRequestApproved() = 0;

	/// \brief Sink the event of manual captions enablement status change.
	/// \param bEnabled True means the host enables the manual captions. Otherwise the host disabled the manual captions.
	virtual void onManualCaptionStatusChanged(bool bEnabled) = 0;
};

/// \brief Closed caption controller interface.
class IClosedCaptionController
{
public:
	/// Set the controller event of closed caption(CC).
	virtual SDKError SetEvent(IClosedCaptionControllerEvent* pEvent) = 0;

	/// Determine if the current meeting supports the CC feature.
	virtual bool IsMeetingSupportCC() = 0;

	/// Query if it is able to assign others to send CC.
	virtual bool CanAssignOthersToSendCC() = 0;

	/// Assign a user to send CC.
	/// Zero(0) means to assign the current user himself.
	virtual SDKError AssignCCPrivilege(unsigned int userid, bool bAssigned) = 0;

	/// Determine if the user can be assigned as a CC sender.
	virtual bool CanBeAssignedToSendCC(unsigned int userid) = 0;

	/// Query if the user can send CC.
	virtual bool CanSendClosedCaption() = 0;

	/// Send the CC message.
	virtual SDKError SendClosedCaption(const zchar_t* ccMsg) = 0;

	/// Determine if it is enabled to save CC.
	virtual bool IsSaveCCEnabled() = 0;

	/// History of saving CC.
	virtual SDKError SaveCCHistory() = 0;

	/// Get the path of saving CC.
	virtual const zchar_t* GetClosedCaptionHistorySavedPath() = 0;

	/// Get the CC URL used by the third party service.
	virtual const zchar_t* GetClosedCaptionUrlFor3rdParty() = 0;

	/// brief Enable or disable manual captions for the meeting.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError EnableMeetingManualCaption(bool bEnable) = 0;

	/// Determine whether manually added closed captions is enabled for the meeting.
	virtual bool IsMeetingManualCaptionEnabled() = 0;

	/// \brief Determine whether the legal notice for Live transcript is available.
	/// \return True indicates the legal notice for Live transcript is available. Otherwise False.
	virtual bool IsLiveTranscriptLegalNoticeAvailable() = 0;

	/// brief Enable or disable to receive original and translated content.If enable this feature,you need start live transcription.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise the function fails and returns an error. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError EnableReceiveSpokenLanguageContent(bool bEnable) = 0;

	/// \brief Query if the user can disable captions.
	/// \return True means that the host can disable captions.
	virtual bool CanDisableCaptions() = 0;

	/// \brief Enable or disable captions.
	/// \param bEnable True means that captions are enabled; false means that captions are disabled.
	/// \return If the function succeeds, the return value is SDKERR_SUCCESS.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError EnableCaptions(bool bEnable) = 0;

	/// \brief Query if the captions enabled.
	/// \return True means that captions are enabled.
	virtual bool IsCaptionsEnabled() = 0;

	/// \brief Determine whether receive original and translated is available.
	/// \return True indicates receive original and translated is available. Otherwise False.
	virtual bool IsReceiveSpokenLanguageContentEnabled() = 0;

	/// Get the CC legal notices prompt.
	virtual const zchar_t* getLiveTranscriptLegalNoticesPrompt() = 0;

	/// Get the CC legal notices explained.
	virtual const zchar_t* getLiveTranscriptLegalNoticesExplained() = 0;

	/// brief Determine whether the live transcription feature is enabled.
	virtual bool IsLiveTranscriptionFeatureEnabled() = 0;

	/// brief Determine whether the multi-language transcription feature is enabled.
	virtual bool IsMultiLanguageTranscriptionEnabled() = 0;

	/// brief Determine whether the translated captions feature is enabled.
	virtual bool IsTextLiveTranslationEnabled() = 0;

	/// \brief Get the current live transcription status.
	//  \return type: the current live transcription status type. For more details, see \link SDKLiveTranscriptionStatus \endlink.
	virtual SDKLiveTranscriptionStatus GetLiveTranscriptionStatus() = 0;

	/// Query if the user can start live transcription.
	virtual bool CanStartLiveTranscription() = 0;

	/// \brief Start live transcription.
	/// \If the meeting allows multi-language transcription,all users can start live transcription.Otherwise only the host can start it
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError StartLiveTranscription() = 0;

	/// \brief Stop live transcription.
	/// \If the meeting allows multi-language transcription,all user can stop live transcription.Otherwise only the host can stop it
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError StopLiveTranscription() = 0;

	/// \brief Enable or disable the ability for attendees to request live transcriptions.
	/// \If the meeting allows multi-language transcription,the return value is SDKERR_WRONG_USAGE
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError EnableRequestLiveTranscription(bool bEnable) = 0;

	/// \brief Determine whether the request to start live transcription is enabled.If the return value is true, it is enabled, if false, disabled.
	/// \If the meeting allows multi-language transcription, the return value is false
	virtual bool IsRequestToStartLiveTranscriptionEnabled() = 0;

	/// \brief Request the host to start live transcription.
	/// \If the meeting allows multi-language transcription,the return value is SDKERR_WRONG_USAGE
	/// \param bRequestAnonymous: True indicates the user anonymous request.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError RequestToStartLiveTranscription(bool bRequestAnonymous) = 0;

	/// \brief Get the list of all available speaking languages in meeting.
	/// \return If the function succeeds, the return value is the list of the available spoken languages in a meeting.
	///Otherwise failed, the return value is nullptr.	
	virtual IList<ILiveTranscriptionLanguage*>* GetAvailableMeetingSpokenLanguages() = 0;

	/// \brief Set the spoken language of the current user.
	/// \param languageID: The spoken language id.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \deprecated This interface is marked as deprecated, and is replaced by SetMeetingSpokenLanguage(int languageID, bool bIsForAll).
	virtual SDKError SetMeetingSpokenLanguage(int languageID) = 0;

	/// Get the spoken language of the current user.
	virtual ILiveTranscriptionLanguage* GetMeetingSpokenLanguage() = 0;

	/// \brief Get the list of all available translation languages in a meeting.
	/// \return If the function succeeds, the return value is the list of all available translation languages in a meeting.
	///Otherwise failed, the return value is nullptr.
	virtual IList<ILiveTranscriptionLanguage*>* GetAvailableTranslationLanguages() = 0;

	/// \brief Set the translation language of the current user.	
	/// \param languageID: The translation language id.
	/// If the language id is set to -1, live translation will be disabled.You can receive closed captions if the host sets manual captions to true for the meeting.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetTranslationLanguage(int languageID) = 0;

	/// Get the translation language of the current user.
	virtual ILiveTranscriptionLanguage* GetTranslationLanguage() = 0;

	/// \brief Determine whether users can request to start captions.
	/// \return True indicates users can request to start captions. Otherwise False.
	virtual bool IsSupportRequestCaptions() = 0;

	/// \brief Determine whether support translation when users request to start captions.
	/// \return True indicates translation is available when users request to start captions. Otherwise False.
	virtual bool IsSupportTranslationWhenRequestToStartCaptions() = 0;

	/// \brief the host to start captions. If the host approves your request, you receive the callback onStartCaptionsRequestApproved,
	/// and you should start captions of translation there.
	/// \param bEnableTranslation True indicates to enable translation at the same time.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError RequestToStartCaptions(bool bEnableTranslation) = 0;

	/// \brief Set the current user's spoken language.
	/// \param languageID The spoken language ID.
	/// \param bIsForAll True means set spoken language for all users. False means set the language for myself.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual SDKError SetMeetingSpokenLanguage(int languageID, bool bIsForAll) = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif