/**
 * @file meeting_phone_helper_interface.h
 * @brief Meeting Service Phone Interface.
 * @note for both ZOOM style and user custom interface mode.
 */
#ifndef _MEETING_PHONE_HELPER_INTERFACE_H_
#define _MEETING_PHONE_HELPER_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE
/**
 * @class IMeetingPhoneSupportCountryInfo
 * @brief Phone meeting support country information interface. 
 */
class IMeetingPhoneSupportCountryInfo
{
public:
	virtual ~IMeetingPhoneSupportCountryInfo(){}
	/**
	* @brief Gets the country ID of the current information.  
	* @return The country ID.
	*/	
	virtual const zchar_t* GetCountryID() = 0;
	/**
	* @brief Gets the country name of the current information.
	* @return The country name.
	*/	
	virtual const zchar_t* GetCountryName() = 0;
	/**
	* @brief Gets the country code of the current information. 
	* @return The country code.
	*/	
	virtual const zchar_t* GetCountryCode() = 0;
};

/**
 * @brief Enumeration of telephone call type. 
 */
enum CALLINNUMTYPE
{
	/** For initialization. */
	CALLINNUMTYPE_NONE,
	/** Paid. */
	CALLINNUMTYPE_TOLL,
	/** Free. */
	CALLINNUMTYPE_TOLLFREE,
};

/**
 * @class IMeetingCallInPhoneNumberInfo
 * @brief Call-in meeting Interface. 
 */
class IMeetingCallInPhoneNumberInfo
{
public:
	virtual ~IMeetingCallInPhoneNumberInfo(){}
	/**
	 * @brief Gets the current call-in number's country ID.
	 * @return The country ID.	
	 */
	virtual const zchar_t* GetID() = 0;
	/**
	 * @brief Gets the current call-in number's country code.
	 * @return The country code.	
	 */
	virtual const zchar_t* GetCode() = 0;
	/**
	 * @brief Gets the current call-in number's country name.
	 * @return The country name.
	 */
	virtual const zchar_t* GetName() = 0;
	 /**
	 * @brief Gets the current call-in number's telephone number.
	 * @return The telephone number.
	 */
	virtual const zchar_t* GetNumber() = 0;
	/**
	 * @brief Gets the current call-in number's display number. 
	 * @return The display number.
	 */
	virtual const zchar_t* GetDisplayNumber() = 0;
	/**
	 * @brief Gets the current call-in number's call type.
	 * @return Call type.
	 */
	virtual CALLINNUMTYPE  GetType() = 0;
};

/** 
 * @brief Enumeration of telephone status.
 */
enum PhoneStatus
{
	/** No status. */
	PhoneStatus_None,
	/** In process of calling out. */
	PhoneStatus_Calling,
	/** In process of ringing. */
	PhoneStatus_Ringing,
	/** Accept the call. */
	PhoneStatus_Accepted,
	/** Call successful. */
	PhoneStatus_Success,
	/** Call failed. */
	PhoneStatus_Failed,
	/** In process of canceling the response to the previous state. */
	PhoneStatus_Canceling,
	/** Cancel successfully. */
	PhoneStatus_Canceled, 
	/** Cancel fails. */
	PhoneStatus_Cancel_Failed,
	/** Timeout. */
	PhoneStatus_Timeout,
};

/**
 * @brief Enumeration of telephone call failure reason.
 */
enum PhoneFailedReason
{
	/** For initialization. */
	PhoneFailedReason_None,
	/** The telephone service is busy. */
	PhoneFailedReason_Busy,
	/** The telephone is out of the service. */
	PhoneFailedReason_Not_Available,
	/** The user hangs up. */
	PhoneFailedReason_User_Hangup,
	/** Other reasons. */
	PhoneFailedReason_Other_Fail,
	/** The telephone does not reply. */
	PhoneFailedReason_No_Answer,
	/** Disable the international call-out function before the host joins the meeting. */
	PhoneFailedReason_Block_No_Host,
	/** The call-out is blocked by the system due to the high cost. */
	PhoneFailedReason_Block_High_Rate,
	/** All the users invited by the call should press one(1) to join the meeting. If many invitees do not press the button and instead are timed out, the call invitation for this meeting is blocked. */
	PhoneFailedReason_Block_Too_Frequent,
};

/**
 * @class IMeetingPhoneHelperEvent
 * @brief Meeting phone helper callback event. 
 */
class IMeetingPhoneHelperEvent
{
public:
	virtual ~IMeetingPhoneHelperEvent() {}

	/**
	 * @brief Invite others by telephone call-out and send the response to the application according to the others' status.
	 * @param status The telephone's status.
	 * @param reason The reason for the failure if the status value is PhoneStatus_Failed.
	 */
	virtual void onInviteCallOutUserStatus(PhoneStatus status, PhoneFailedReason reason) = 0;
	
	/**
	 * @brief Invite others to join the meeting by CALL ME and give the response to the application according to the status. 
	 * @param status The telephone's status.
	 * @param reason The reason for the failure if the status value is PhoneStatus_Failed.
	 */
	virtual void onCallMeStatus(PhoneStatus status, PhoneFailedReason reason) = 0;
};
/**
 * @class IMeetingPhoneHelper
 * @brief Meeting phone helper interface. 
 */
class IMeetingPhoneHelper
{
public:
	/**
	 * @brief Sets meeting phone helper callback event handler 123, 456, 123
	 * @param pEvent A pointer to the IMeetingPhoneHelperEvent that receives the phone event. 
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError SetEvent(IMeetingPhoneHelperEvent* pEvent) = 0;

	 /**
	 * @brief Determines whether the meeting supports to join by the phone or not.
	 * @return true if to support to join by phone. Otherwise, false.
	 */	
	 virtual bool IsSupportPhoneFeature() = 0;
	
	 /**
	 * @brief Query if it is able to dial out in the current meeting.
	 * @return true if able to dial out. Otherwise, false.
	 */	
	 virtual bool IsDialoutSupported() = 0;

	 /**
	  * @brief Determines whether any phone dial-out action is in progress.
	  * @return true if either CALL ME or invite-callout is in progress. Otherwise, false.
	  */
	 virtual bool IsDialOutInProgress() = 0;

	 /**
	 * @brief Gets the list of the country information where the meeting supports to join by telephone.
	 * @return List of the country information returns if the meeting supports to join by telephone. Otherwise nullptr.
	 */	
	 virtual IList<IMeetingPhoneSupportCountryInfo* >* GetSupportCountryInfo() = 0;

	 /**
	  * @brief Gets the selected caller ID for phone call-out.
	  * @return If the function succeeds, it returns the caller ID. Otherwise returns nullptr.
	  */
	 virtual const zchar_t* GetCallOutCallerID() = 0;

	 /**
	 * @brief Invite the specified user to join the meeting by calling out.
	 * @param countryCode The specified users' country code must be in the support list.
	 * @param phoneNumber The specified users' phone number.
	 * @param name The specified users' screen name in the meeting.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError InviteCallOutUser(const zchar_t* countryCode, const zchar_t* phoneNumber, const zchar_t* name) = 0;

	/**
	 * @brief Cancel the invitation that is being called out by phone.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError CancelCallOut() = 0;

	 /**
	 * @brief Gets the invited users' status by calling out.
	 * @return If the function succeeds, it returns the current call-out process.
	 */	
	 virtual PhoneStatus GetInviteCalloutUserStatus() = 0;

	/**
	 * @brief Use the CALL ME to invite the attendee who uses the specified number to join the meeting.
	 * @param countryCode The country code of the specified user must be in the support list.
	 * @param phoneNumber The specified phone number.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError CallMe(const zchar_t* countryCode, const zchar_t* phoneNumber) = 0;

	/**
	 * @brief Cancel the current invitation by CALL ME.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 */
	virtual SDKError Hangup() = 0;

	/**
	 * @brief Gets the process of the invitation by CALL ME.
	 * @return If the function succeeds, the return value is the process of the invitation by CALL ME.
	 */
	virtual PhoneStatus GetCallMeStatus() = 0;

	/**
	 * @brief Gets the list of call in numbers supported by meeting.
	 * @return If the function succeeds, the return value is the list of the call-in numbers.
	 */	
	virtual IList<IMeetingCallInPhoneNumberInfo*>* GetCurrentMeetingCallinNumber() = 0;

	/**
	 * @brief Gets the ID of the participant who joins the meeting by calling in. 
	 * @return If the function succeeds, it returns the participant ID.
	 */	
	virtual unsigned int GetCurrentMeetingCallinParticipantID() = 0;
};
END_ZOOM_SDK_NAMESPACE
#endif
