/**
 * @file meeting_sign_interpretation_interface.h
 * @brief Meeting Service Sign Interpretation Interface
 * Valid for both ZOOM style and user custom interface mode.
 */
#ifndef _MEETING_SIGN_INTERPRETATION_INTERFACE_H_
#define _MEETING_SIGN_INTERPRETATION_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/**
 * @brief Enumeration of sign interpretation status.
 * Here are more detailed structural descriptions.
 */
enum SignInterpretationStatus
{
	/** The initial status */
	SignInterpretationStatus_Initial, 
	/** sign interpretation stared. */
	SignInterpretationStatus_Started, 
	/** sign interpretation stopped. */
	SignInterpretationStatus_Stopped,  
};
/**
 * @class ISignInterpretationLanguageInfo
 * @brief sign interpretation language interface.
 */
class ISignInterpretationLanguageInfo
{
public:
	virtual ~ISignInterpretationLanguageInfo() {}
	virtual const zchar_t* GetSignLanguageID() = 0;
	virtual const zchar_t* GetSignLanguageName() = 0;
};
/**
 * @class ISignInterpreter
 * @brief sign interpreter interface.
 */
class ISignInterpreter
{
public:
	virtual ~ISignInterpreter() {}
	virtual unsigned int GetUserID() = 0;
	virtual const zchar_t* GetSignLanguageID() = 0;
	virtual bool IsAvailable() = 0;
};
/**
 * @class IRequestSignInterpreterToTalkHandler
 * @brief Process after the sign interpreter receives the requirement from the host to allow to talk.
 */
class IRequestSignInterpreterToTalkHandler
{
public:
	virtual ~IRequestSignInterpreterToTalkHandler() {};
	
	/**
	 * @brief Instance to ignore the requirement, return nothing and finally self-destroy.
	 */
	virtual SDKError Ignore() = 0;
	
	/**
	 * @brief Instance to accept the requirement, unmute the audio and finally self-destroy.
	 */
	virtual SDKError Accept() = 0;
	
	/**
	 * @brief Ignore the request to unmute the audio in the meeting and finally the instance self-destroys.
	 */
	virtual SDKError Cancel() = 0;
};
/**
 * @class IMeetingSignInterpretationControllerEvent
 * @brief Meeting sign interpretation callback event.
 */
class IMeetingSignInterpretationControllerEvent
{
public:
	virtual ~IMeetingSignInterpretationControllerEvent() {}
	
	/**
	 * @brief Sign interpretation status change callback. This function is used to inform the user sign interpretation has been started or stopped, and all users in the meeting can get the event.
	 * @param status Specify current sign interpretation status.
	 */
	virtual void OnSignInterpretationStatusChanged(SignInterpretationStatus status) = 0;
	
	/**
	 * @brief Sign interpreter list changed callback. When an interpreter leave the meeting, preset interpreter joins the meeting, or the host adds or removes an interpreter, this will be triggered.
	 */
	virtual void OnSignInterpreterListChanged() = 0;
	
	/**
	 * @brief Available sign languages changed callback. When the available sign languages in a meeting change, all users in the meeting can get the event.
	 * @param pAvailableSignLanguageList Specify the available sign languages list.
	 */
	virtual void OnAvailableSignLanguageListUpdated(IList<ISignInterpretationLanguageInfo*>* pAvailableSignLanguageList) = 0;
	
	/**
	 * @brief Interpreters role changed callback. when myself role changed(participant <-> interpreter), and only myself in meeting can get the event. 
	 */
	virtual void OnSignInterpreterRoleChanged() = 0;
	
	/**
	 * @brief Sign interpreter languages changed callback. when a sign interpreter's languages changed, and only the sign interpreter self can get the event.
	 */
	virtual void OnSignInterpreterLanguageChanged() = 0;
	
	/**
	 * @brief Callback event for the user talk privilege changed. When the interpreter role or host changed, host allows/disallows interpreter talk, this will be triggered. Only the sign interpreter can get the event.
	 * @param hasPrivilege Specify whether the user has talk privilege or not.
	 */
	virtual void OnTalkPrivilegeChanged(bool hasPrivilege) = 0;
	
	/**
	 * @brief Callback event of the requirement to unmute the audio.
	 * @param handler_ A pointer to the IRequestSignInterpreterToTalkHandler.
	 */
	virtual void OnRequestSignInterpreterToTalk(IRequestSignInterpreterToTalkHandler* handler) = 0;
};
/**
 * @class IMeetingSignInterpretationController
 * @brief Meeting interpretation controller interface
 */
class IMeetingSignInterpretationController
{
public:
	virtual ~IMeetingSignInterpretationController() {}

	/** 
	 * @note Common (for all) 
	 */
	 
	/**
	 * @brief Sets the interpretation controller callback event handler.
	 * @param event A pointer to the IMeetingInterpretationControllerEvent that receives the interpretation event. . 
	 */
	virtual void SetEvent(IMeetingSignInterpretationControllerEvent* event) = 0;
	
	/**
	 * @brief Determines if sign interpretation feature is enabled in the meeting.
	 */
	virtual bool IsSignInterpretationEnabled() = 0;
	
	/**
	 * @brief Gets sign interpretation status of current meeting.
	 * @return If the function succeeds, the return value is the sign interpretation status of current meeting.
	 */
	virtual SignInterpretationStatus GetSignInterpretationStatus() = 0;
	
	/**
	 * @brief Determines if myself is interpreter.
	 */
	virtual bool IsSignInterpreter() = 0;
	
	/**
	 * @brief Gets the sign interpretation language object of specified sign language ID.
	 * @param signLanguageId Specify the sign language ID for which you want to get the information. 
	 * @return If the function succeeds, it returns a pointer to the IInterpretationLanguage. Otherwise, this function fails and returns nullptr.
	 */
	virtual ISignInterpretationLanguageInfo* GetSignInterpretationLanguageInfoByID(const zchar_t* signLanguageId) = 0;
	
	/**
	 * @brief Gets the available sign interpretation language list.
	 * @return If the function succeeds, it returns a pointer to the IList<ISignInterpretationLanguageInfo*>. Otherwise, this function fails and returns nullptr.
	 */
	virtual IList<ISignInterpretationLanguageInfo*>* GetAvailableSignLanguageInfoList() = 0;

	/** 
	 * @note Admin (only for host) 
	 */
	 
	/**
	 * @brief Gets all supported sign interpretation language list.
	 * @return If the function succeeds, it returns a pointer to the IList<ISignInterpretationLanguageInfo*>. Otherwise, this function fails and returns nullptr.
	 */
	virtual IList<ISignInterpretationLanguageInfo*>* GetAllSupportedSignLanguageInfoList() = 0;
	
	/**
	 * @brief Gets the sign interpreters list.
	 * @return If the function succeeds, it returns a pointer to the IList<ISignInterpreter*>. Otherwise, this function fails and returns nullptr.
	 */
	virtual IList<ISignInterpreter*>* GetSignInterpreterList() = 0;
	
	/**
	 * @brief Add someone as a sign interpreter.
	 * @param userID Specify the user.
	 * @param signLanguageId Specify the sign language.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError AddSignInterpreter(unsigned int userID, const zchar_t* signLanguageId) = 0;
	
	/**
	 * @brief Remove someone from the list of sign interpreters.
	 * @param userID Specify the user.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError RemoveSignInterpreter(unsigned int userID) = 0;
	
	/**
	 * @brief Modify the language of a sign interpreter.
	 * @param userID Specify the interpreter.
	 * @param signLanguageId Specify the new sign language.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError ModifySignInterpreterLanguage(unsigned int userID, const zchar_t* signLanguageId) = 0;
	
	/**
	 * @brief Determines if I can start the sign interpretation in the meeting.
	 * @return If it can start the sign interpretation, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError CanStartSignInterpretation() = 0;
	
	/**
	 * @brief Starts sign interpretation.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError StartSignInterpretation() = 0;
	
	/**
	 * @brief Stops sign interpretation.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError StopSignInterpretation() = 0;
	
	/**
	 * @brief Host allow sign language interpreter to talk.
	 * @param userID Specify the sign language interpreter.
	 * @param allowToTalk true indicates to allow to talk. Otherwise, false.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError RequestSignLanguageInterpreterToTalk(unsigned int userID, bool allowToTalk) = 0;
	
	/**
	 * @brief Determines if the sign language interpreter be allowed to talk.
	 * @param [out] canTalk indicates if allow to talk.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError CanSignLanguageInterpreterTalk(unsigned int userID, bool& canTalk) = 0;

	/** 
	 * @note Interpreter (only for interpreter) 
	 */
	 
	/**
	 * @brief Gets sign language id if myself is a sign interpreter.
	 * @return If the function succeeds, the return value is the current assigned sign language id. Otherwise, this function fails and returns an empty string of length ZERO(0).
	 */
	virtual const zchar_t* GetSignInterpreterAssignedLanID() = 0;

	/** 
	 * @note Listener (for non interpreter) 
	 */
	 
	/**
	 * @brief Joins a sign language channel if myself is not a sign interpreter.
	 * @param signLanguageId Specify the sign language channel ID.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for Zoom style user interface mode. 
	 */
	virtual SDKError JoinSignLanguageChannel(const zchar_t* signLanguageId) = 0;
	
	/**
	 * @brief Off sign language if myself is not a sign interpreter..
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Valid only for Zoom style user interface mode.
	 */
	virtual SDKError LeaveSignLanguageChannel() = 0;
};
END_ZOOM_SDK_NAMESPACE
#endif