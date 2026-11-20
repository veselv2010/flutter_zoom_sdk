/**
 * @file meeting_backstage_interface.h
 * @brief Meeting Service Webinar Backstage Interface.
 */

#ifndef _MEETING_BACKSTAGE_INTERFACE_H_
#define _MEETING_BACKSTAGE_INTERFACE_H_
#include "zoom_sdk_def.h"
BEGIN_ZOOM_SDK_NAMESPACE

/**
 * @brief Enumerations of the reason why the local user entered or left the backstage.
 * Delivered through the iResult parameter of
 * IMeetingServiceEvent::onMeetingStatusChanged() when the status is
 * MEETING_STATUS_JOIN_BACKSTAGE or MEETING_STATUS_LEAVE_BACKSTAGE.
 */
enum SDKBackstageChangeReason
{
	/** Unknown reason. */
	SDKBackstageChangeReason_Unknown = 0,
	/** Entered the backstage automatically when joining the webinar. */
	SDKBackstageChangeReason_FirstJoinMeeting,
	/** Entered the backstage because the local user was promoted to panelist. */
	SDKBackstageChangeReason_BePromoted,
	/** Moved by the host or the co-host. */
	SDKBackstageChangeReason_MovedByHost,
	/** The host ended the webinar for attendees only and the panelists were moved for debrief. */
	SDKBackstageChangeReason_EnterDebriefMode,
	/** Triggered by the local user. */
	SDKBackstageChangeReason_TriggeredByUser,
	/** Triggered by a failover reconnection. */
	SDKBackstageChangeReason_Failover,
};

/**
 * @class IMeetingBackstageController
 * @brief Webinar backstage controller interface.
 */
class IMeetingBackstageController
{
public:
	virtual ~IMeetingBackstageController() {};
	/**
	 * @brief Determines if the backstage feature is enabled for the current webinar.
	 * @return true if the backstage feature is enabled. Otherwise false.
	 */
	virtual bool IsBackstageEnabled() = 0;

	/**
	 * @brief Determines if the local user can enter the backstage.
	 * @return true if the local user can enter the backstage. Otherwise false.
	 * @note Returns false when the local user is a panelist and the host does not allow
	 * panelists to switch between the main session and the backstage on their own.
	 */
	virtual bool CanJoinBackstage() = 0;

	/**
	 * @brief Makes the local user enter the backstage.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note If the function succeeds, the user will receive the
	 * IMeetingServiceEvent::onMeetingStatusChanged() callback event with
	 * MEETING_STATUS_JOIN_BACKSTAGE.
	 */
	virtual SDKError JoinBackstage() = 0;

	/**
	 * @brief Determines if the local user can leave the backstage.
	 * @return true if the local user can leave the backstage. Otherwise false.
	 */
	virtual bool CanLeaveBackstage() = 0;

	/**
	 * @brief Makes the local user leave the backstage and return to the main session.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note If the function succeeds, the user will receive the
	 * IMeetingServiceEvent::onMeetingStatusChanged() callback event with
	 * MEETING_STATUS_LEAVE_BACKSTAGE. The switch takes effect immediately; the SDK does
	 * not apply the countdown used by the Zoom client.
	 */
	virtual SDKError LeaveBackstage() = 0;

	/**
	 * @brief Determines if the specified user can be moved into the backstage.
	 * @param userid Specifies the user ID to check.
	 * @return true if the user can be moved into the backstage. Otherwise false.
	 * @note Available only for the host and the co-host.
	 */
	virtual bool CanMoveUserToBackstage(unsigned int userid) = 0;

	/**
	 * @brief Moves the specified users into the backstage.
	 * @param lstUserID Specifies the list of user IDs to move. Use IMeetingParticipantsController::GetUserIDListBuilder() to construct it.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Available only for the host and the co-host. The return value only indicates
	 * that the request was sent. Track the per-user outcome through
	 * IMeetingParticipantsCtrlEvent::onUserLeft() and onUserJoin().
	 */
	virtual SDKError MoveUsersToBackstage(IList<unsigned int>* lstUserID) = 0;

	/**
	 * @brief Determines if the specified user can be moved back to the main session.
	 * @param userid Specifies the user ID to check.
	 * @return true if the user can be moved back to the main session. Otherwise false.
	 * @note Available only for the host and the co-host. Valid whether or not the local user is currently in the backstage.
	 */
	virtual bool CanMoveUserToMainSession(unsigned int userid) = 0;

	/**
	 * @brief Moves the specified users from the backstage back to the main session.
	 * @param lstUserID Specifies the list of user IDs to move. Use IMeetingParticipantsController::GetUserIDListBuilder() to construct it.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Available only for the host and the co-host.
	 */
	virtual SDKError MoveUsersToMainSession(IList<unsigned int>* lstUserID) = 0;

	/**
	 * @brief Determines if the local user can set the local playback volume of the main session audio.
	 * @return true if the local user is in the backstage. Otherwise false.
	 * @note When this method returns false, SetMainSessionAudioVolume() returns SDKERR_WRONG_USAGE.
	 */
	virtual bool CanSetMainSessionAudioVolume() = 0;

	/**
	 * @brief Sets the local playback volume of the main session audio while in the backstage.
	 * @param value The volume that ranges from 0 to 255.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Available only when the local user is in the backstage. The change is local and does not affect other users.
	 */
	virtual SDKError SetMainSessionAudioVolume(FLOAT value) = 0;

	/**
	 * @brief Gets the local playback volume of the main session audio while in the backstage.
	 * @param value The current volume that ranges from 0 to 255.
	 * @return If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise, this function returns an error.
	 * @note Available only when the local user is in the backstage.
	 */
	virtual SDKError GetMainSessionAudioVolume(FLOAT& value) = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif
