/**
 * @file zoom_sdk_sms_helper_interface.h
 * @brief ZOOM SDK SMS helper interface.
 */

#ifndef _ZOOM_SDK_SMS_HELPER_INTERFACE_H_
#define _ZOOM_SDK_SMS_HELPER_INTERFACE_H_
#include "zoom_sdk_platform.h"

/**
 * @class IZoomRetrieveSMSVerificationCodeHandler
 * @brief Retrieve SMS verification code handler interface.
 */
class IZoomRetrieveSMSVerificationCodeHandler
{
public:
	/**
	 * @brief Retrieves the SMS verification code. 
	 * @param country_code The country code.
	 * @param phone_number The phone number.
	 * @return true if the function succeeds. Otherwise, false.
	 * @note The handler will become invalid after calling this function successfully.
	 */
	virtual bool Retrieve(const zTCHAR* country_code, const zTCHAR* phone_number) = 0;
	
	/**
	 * @brief Ignores the prompt of retrieving the verification code and leaves meeting.
	 * @return true if the function succeeds. Otherwise, false.
	 * @note The handler will become invalid and destroyed after calling this function successfully.
	 */
	virtual bool CancelAndLeaveMeeting() = 0;
	
	virtual ~IZoomRetrieveSMSVerificationCodeHandler(){}
};

/**
 * @class IZoomVerifySMSVerificationCodeHandler
 * @brief Verify SMS verification code handler interface.
 */
class IZoomVerifySMSVerificationCodeHandler
{
public:
	/**
	 * @brief Verifies the SMS verification code. 
	 * @param country_code The country code.
	 * @param phone_number The phone number.
	 * @param verification_code The verification code.
	 * @return true if the function succeeds. Otherwise, false.
	 * @note The handler will become invalid after calling this function successfully.
	 */
	virtual bool Verify(const zTCHAR* country_code, const zTCHAR* phone_number, const zTCHAR* verification_code) = 0;
	
	/**
	 * @brief Ignores the prompt of verifying the verification code and leaves meeting.
	 * @return true if the function succeeds. Otherwise, false.
	 * @note The handler will become invalid and destroyed after calling this function successfully.
	 */
	virtual bool CancelAndLeaveMeeting() = 0;
	
	virtual ~IZoomVerifySMSVerificationCodeHandler() {}
};

/**
 * @brief Enumeration of SMS verification error types.
 * Here are more detailed structural descriptions.
 */
enum SMSVerificationCodeErr
{
	/** For initialization. */
	SMSVerificationCodeErr_Unknown,
	/** Success. */
	SMSVerificationCodeErr_Success,
	/** Send SMS Failed. */
	SMSVerificationCodeErr_Retrieve_SendSMSFailed,
	/** Invalid phone number. */
	SMSVerificationCodeErr_Retrieve_InvalidPhoneNum,
	/** The phone number is already bound. */
	SMSVerificationCodeErr_Retrieve_PhoneNumAlreadyBound,
	/** Send phone number too frequently. */
	SMSVerificationCodeErr_Retrieve_PhoneNumSendTooFrequent,
	/** Verification code is incorrect. */
	SMSVerificationCodeErr_Verify_CodeIncorrect,
	/** Verification code is expired. */
	SMSVerificationCodeErr_Verify_CodeExpired,
	/** Unknown error for verification. */
	SMSVerificationCodeErr_Verify_UnknownError,
};

/**
 * @class IZoomRealNameAuthCountryInfo
 * @brief Interface of country information that supports real name auth.
 */
class IZoomRealNameAuthCountryInfo
{
public:
	/**
	 * @brief Gets the country ID of the current information. 
	 * @return If the function succeeds, it returns the country ID. 
	 */
	virtual const zTCHAR* GetCountryID() = 0;
	
	/**
	 * @brief Gets the country name of the current information.
	 * @return If the function succeeds, it returns the country name.
	 */
	virtual const zTCHAR* GetCountryName() = 0;
	
	/**
	 * @brief Gets the country code of the current information.
	 * @return If the function succeeds, it returns the country code.
	 */
	virtual const zTCHAR* GetCountryCode() = 0;
	
	virtual ~IZoomRealNameAuthCountryInfo() {}
};

/**
 * @class IZoomRealNameAuthMeetingEvent
 * @brief Real name auth meeting callback event.
 */
class IZoomRealNameAuthMeetingEvent
{
public:
	/**
	 * @brief If real name auth is needed, this callback will be triggered.
	 * @param support_country_list The list of the supporting country information.
	 * @param privacy_url The privacy url.
	 * @param handler A pointer to the IZoomRetrieveSMSVerificationCodeHandler.
	 * The SDK user can retrieve the verification code via the functions of IZoomRetrieveSMSVerificationCodeHandler.
	 */
	virtual void onNeedRealNameAuthMeetingNotification(IVector<IZoomRealNameAuthCountryInfo* >* support_country_list, const zTCHAR* privacy_url, IZoomRetrieveSMSVerificationCodeHandler* handler) = 0;
	
	/**
	 * @brief The callback event for retrieving SMS verification code.
	 * @param result Specifies the result of retrieve.
	 * @param handler A pointer to the IZoomVerifySMSVerificationCodeHandler. It is only valid when the value of result is SMSVerificationCodeErr_Success.
	 * The SDK user can do the verification via the functions of IZoomVerifySMSVerificationCodeHandler. 
	 */
	virtual void onRetrieveSMSVerificationCodeResultNotification(SMSVerificationCodeErr result, IZoomVerifySMSVerificationCodeHandler* handler) = 0;
	
	/**
	 * @brief The callback event for verification.
	 * @param result Specifies the result of verification.
	 */
	virtual void onVerifySMSVerificationCodeResultNotification(SMSVerificationCodeErr result) = 0;
	
	virtual ~IZoomRealNameAuthMeetingEvent() {}
};

/**
 * @class IZoomRealNameAuthMeetingHelper
 * @brief Real name auth meeting helper Interface.
 */
class IZoomRealNameAuthMeetingHelper
{
public:
	/**
	 * @brief Sets the real name auth meeting helper callback event handler.
	 * @param event_ A pointer to the IZoomRealNameAuthMeetingEvent. 
	 * @return true if the function succeeds. Otherwise, false.
	 */
	virtual bool SetEvent(IZoomRealNameAuthMeetingEvent* event_) = 0;
	
	/**
	 * @brief Sets the visibility of the dialog box of auth real name when needed. Default value: true.
	 * @param enable true to display the dialog box, false otherwise.
	 * @return true if the function succeeds. Otherwise, false.
	 */
	virtual bool EnableZoomAuthRealNameMeetingUIShown(bool enable) = 0;
	
	/**
	 * @brief Sets default cellphone information to let the user bypass the real name auth to start or join meeting directly.
	 * @param country_code The country code.
	 * @param phone_number The phone number. 
	 * @return true if the function succeeds. Otherwise, false.
	 */
	virtual bool SetDefaultCellPhoneInfo(const zTCHAR* country_code, const zTCHAR* phone_number) = 0;
	
	/**
	 * @brief Gets retrieve SMS verification code handler interface.
	 * @return If the function succeeds, the return value is a pointer to IZoomRetrieveSMSVerificationCodeHandler. Otherwise, this function fails and returns nullptr.
	 */
	virtual IZoomRetrieveSMSVerificationCodeHandler* GetResendSMSVerificationCodeHandler() = 0;
	
	/**
	 * @brief Gets verify SMS verification code handler interface.
	 * @return If the function succeeds, the return value is a pointer to IZoomVerifySMSVerificationCodeHandler. Otherwise, this function fails and returns nullptr.
	 */
	virtual IZoomVerifySMSVerificationCodeHandler* GetReVerifySMSVerificationCodeHandler() = 0;
	
	/**
	 * @brief Gets the list of the country information where the meeting supports real name auth.
	 * @return If the function succeeds, it returns the list of the country information where the meeting supports real name auth. Otherwise, this function fails and returns nullptr.
	 */
	virtual IVector<IZoomRealNameAuthCountryInfo* >* GetSupportPhoneNumberCountryList() = 0;
	
	virtual ~IZoomRealNameAuthMeetingHelper() {}
};
#endif
