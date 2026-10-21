/**
 * @file meeting_talkback_ctrl_interface.h
 * @brief Meeting Service Talkback Interface.
 */
#ifndef _MEETING_TALKBACK_CTRL_INTERFACE_H_
#define _MEETING_TALKBACK_CTRL_INTERFACE_H_
#include "zoom_sdk_def.h"

BEGIN_ZOOM_SDK_NAMESPACE

/**
 * @class IMeetingTalkbackChannel
 * @brief Talkback channel information interface.
 */
class IMeetingTalkbackChannel
{
public:
	virtual ~IMeetingTalkbackChannel() {}
	
	/**
	 * @brief Get the channel ID.
	 * @return The channel ID.
	 */
	virtual const zchar_t* GetChannelID() = 0;
	
	/**
	 * @brief Get the list of user IDs in the channel.
	 * @return The list of user IDs in the channel.
	 */
	virtual IList<unsigned int>* GetUserIDList() = 0;
};

/**
 * @class IMeetingTalkbackCtrlEvent
 * @brief Talkback controller callback event.
 */
class IMeetingTalkbackCtrlEvent
{
public:
	virtual ~IMeetingTalkbackCtrlEvent() {}
	
	/**
	 * @brief Talkback error enum.
	 */
	enum TalkbackError
	{
		/**Operation executed successfully. */
		TALKBACK_ERROR_OK,
		/**Permission denied. */
		TALKBACK_ERROR_NOPERMISSION,
		/**The object being operated on already exists, Example: the invited user is already in the channel. */
		TALKBACK_ERROR_ALREADY_EXIST,
		/**The number of channels exceeded the limit. */
		TALKBACK_ERROR_COUNT_OVERFLOW,
		/**The object being operated on does not exist. */
		TALKBACK_ERROR_NOT_EXIST,
		/**Operation denied. */
		TALKBACK_ERROR_REJECTED,
		/**Operation timed out. */
		TALKBACK_ERROR_TIMEOUT,
		/**Unknown reason. */
		TALKBACK_ERROR_UNKNOWN
	};
	
	/**
	 * @brief Callback event for create channel response.
	 * @param channelID Specify the channel ID.
	 * @param error Specify the error code.
	 * @note Determine whether the channel is successfully created. CreateChannel is asynchronous, so check this callback to verify the creation result.
	 */
	virtual void onCreateChannelResponse(const zchar_t* channelID, TalkbackError error) = 0;
	
	/**
	 * @brief Callback event for destroy channel response.
	 * @param channelID Specify the channel ID.
	 * @param error Specify the error code.
	 * @note Determine whether the channel is successfully destroyed. Batch destroy channels is asynchronous, so check this callback to verify the destruction result.
	 */
	virtual void onDestroyChannelResponse(const zchar_t* channelID, TalkbackError error) = 0;
	
	/**
	 * @brief Callback event for user joined channel response.
	 * @param channelID Specify the channel ID.
	 * @param userID Specify the user ID who joined the channel.
	 * @param error Specify the error code.
	 * @note Determine whether the user is successfully invited. Batch inviting users is asynchronous, so check this callback to verify the invitation result.
	 */
	virtual void onChannelUserJoinResponse(const zchar_t* channelID, unsigned int userID, TalkbackError error) = 0;
	
	/**
	 * @brief Callback event for user left channel response.
	 * @param channelID Specify the channel ID.
	 * @param userID Specify the user ID who left the channel.
	 * @param error Specify the error code.
	 * @note Determine whether the user is successfully removed. Batch removing users from channel is asynchronous, so check this callback to verify the removal result.
	 */
	virtual void onChannelUserLeaveResponse(const zchar_t* channelID, unsigned int userID, TalkbackError error) = 0;
	
	/**
	 * @brief Callback event for already joined talkback channel.
	 * @param inviterID Specify the inviter user ID who invited to join the channel.
	 * @note This event notifies you that you have already entered the talkback channel, and the inviter ID is inviterID.
	 */
	virtual void onJoinTalkbackChannel(unsigned int inviterID) = 0;
	
	/**
	 * @brief Callback event for already left talkback channel.
	 * @param inviterID Specify the inviter user ID who invited to leave the channel.
	 * @note This event notifies you that you have left the talkback channel.
	 */
	virtual void onLeaveTalkbackChannel(unsigned int inviterID) = 0;

	/**
	 * @brief Callback event for inviter audio level in talkback channel.
	 * @param inviterID Specify the inviter user ID.
	 * @param audioLevel Inviter audio volume, range 0-15.
	 */
	virtual void onInviterAudioLevel(unsigned int inviterID, unsigned int audioLevel) = 0;
};

/**
 * @class IMeetingTalkbackController
 * @brief Talkback controller interface.
 */
class IMeetingTalkbackController
{
public:
	virtual ~IMeetingTalkbackController() {}
	
	/**
	 * @brief Configure the meeting talkback controller callback event handler.
	 * @param pEvent An object pointer to the IMeetingTalkbackCtrlEvent that receives the meeting talkback callback event.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note The SDK use pEvent to transmit the callback event to the user's application. If the function is not called or fails, the user's application is unable to retrieve the callback event.
	 */
	virtual SDKError SetEvent(IMeetingTalkbackCtrlEvent* pEvent) = 0;
	
	/**
	 * @brief Create talkback channels.
	 * @param count Specify the number of channels to create. Supports a maximum of 16 channels.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note CreateChannel is asynchronous. To determine whether the channel is successfully created, check the onCreateChannelResponse event callback.
	 */
	virtual SDKError CreateChannel(unsigned int count) = 0;
	
	/**
	 * @brief Get a talkback channel by channel ID.
	 * @param channelID Specify the channel ID to query.
	 * @return If the function succeeds, the return value is a pointer to the IMeetingTalkbackChannel. Otherwise, the return value is nullptr.
	 */
	virtual IMeetingTalkbackChannel* GetChannelByID(const zchar_t* channelID) = 0;
	
	/**
	 * @brief Get the list of all talkback channels.
	 * @return If the function succeeds, the return value is a pointer to the IList<IMeetingTalkbackChannel*>. Otherwise, the return value is nullptr.
	 */
	virtual IList<IMeetingTalkbackChannel*>* GetChannelList() = 0;
	
	/////////////////////////////////////////////////////////////// Destroy channels in batches
	/**
	 * @brief Begin batch destroy channels operation.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note Call this function before adding channels to the batch destroy list. After adding all channels, call ExecuteBatchDestroyChannels() to destroy them.
	 */
	virtual SDKError BeginBatchDestroyChannels() = 0;
	
	/**
	 * @brief Add a channel to the batch destroy list.
	 * @param pChanID Specify the channel ID to add to the batch destroy list.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note Call this function after BeginBatchDestroyChannels() and before ExecuteBatchDestroyChannels().
	 */
	virtual SDKError AddChannelToDestroy(const zchar_t* pChanID) = 0;
	
	/**
	 * @brief Remove a channel from the batch destroy list.
	 * @param pChanID Specify the channel ID to remove from the batch destroy list.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note Call this function after BeginBatchDestroyChannels() and before ExecuteBatchDestroyChannels().
	 */
	virtual SDKError RemoveChannelFromDestroy(const zchar_t* pChanID) = 0;
	
	/**
	 * @brief Execute batch destroy operation to destroy all channels in the list.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note This function destroys all channels that were added to the batch destroy list using AddChannelToDestroy(). Call BeginBatchDestroyChannels() before adding channels.
	 * @note Batch destroy channels is asynchronous. To determine whether the channels are successfully destroyed, check the onDestroyChannelResponse event callback.
	 */
	virtual SDKError ExecuteBatchDestroyChannels() = 0;
	
	/////////////////////////////////////////////////////////////// Invite users to channel in batches.
	/**
	 * @brief Begin batch invite users operation to a channel.
	 * @param channelID Specify the channel ID to invite users to.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note Call this function before adding users to the batch invite list. After adding all users, call ExecuteBatchInviteUsers() to invite them.
	 */
	virtual SDKError BeginBatchInviteUsers(const zchar_t* channelID) = 0;
	
	/**
	 * @brief Add a user to the batch invite list.
	 * @param userID Specify the user ID to add to the batch invite list.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note Call this function after BeginBatchInviteUsers() and before ExecuteBatchInviteUsers().
	 * @note A channel can have at most 10 users.
	 */
	virtual SDKError AddUserToInvite(unsigned int userID) = 0;
	
	/**
	 * @brief Remove a user from the batch invite list.
	 * @param userID Specify the user ID to remove from the batch invite list.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note This function should be called after BeginBatchInviteUsers() and before ExecuteBatchInviteUsers().
	 */
	virtual SDKError RemoveUserFromInvite(unsigned int userID) = 0;
	
	/**
	 * @brief Execute batch invite operation to invite all users in the list to the channel.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note This function invites all users that were added to the batch invite list using AddUserToInvite(). Call BeginBatchInviteUsers() before adding users.
	 * @note Batch invite users is asynchronous. To determine whether the users are successfully invited, check the onChannelUserJoinResponse event callback.
	 */
	virtual SDKError ExecuteBatchInviteUsers() = 0;
	
	/////////////////////////////////////////////////////////////// Remove users from channel in batches.
	/**
	 * @brief Begin batch remove users operation from a channel.
	 * @param channelID Specify the channel ID to remove users from.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note Call this function before adding users to the batch remove list. After adding all users, call ExecuteBatchRemoveUsers() to remove them.
	 */
	virtual SDKError BeginBatchRemoveUsers(const zchar_t* channelID) = 0;
	
	/**
	 * @brief Add a user to the batch remove list.
	 * @param userID Specify the user ID to add to the batch remove list.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note Call this function after BeginBatchRemoveUsers() and before ExecuteBatchRemoveUsers().
	 */
	virtual SDKError AddUserToRemove(unsigned int userID) = 0;
	
	/**
	 * @brief Remove a user from the batch remove list.
	 * @param userID Specify the user ID to remove from the batch remove list.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note Call this function after BeginBatchRemoveUsers() and before ExecuteBatchRemoveUsers().
	 */
	virtual SDKError RemoveUserFromRemoveList(unsigned int userID) = 0;
	
	/**
	 * @brief Execute batch remove operation to remove all users in the list from the channel.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 * @note This function removes all users that were added to the batch remove list using AddUserToRemove(). Call BeginBatchRemoveUsers() before adding users.
	 * @note Batch remove users from channel is asynchronous. To determine whether the users are successfully removed, check the onChannelUserLeaveResponse event callback.
	 */
	virtual SDKError ExecuteBatchRemoveUsers() = 0;
	
	/**
	 * @brief Send audio data to a talkback channel.
	 * @param channelID Specify the channel ID.
	 * @param pAudioData Specify the audio data buffer.
	 * @param dataLength Specify the length of audio data in bytes.
	 * @param sampleRate Specify the sample rate of the audio data like 32000 or 48000.
	 * @param channel Specify the audio channel type either mono or stereo.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 *         If the return value is SDKERR_TOO_FREQUENT_CALL, please slow down the sending frequency.
	 * @note The audio data format should be PCM, 16-bit, mono or stereo. Sample rate 32kHz or 48kHz is recommended.
	 *  1. The size of a single input must not exceed the number of audio bytes for 80 ms of audio.
	 *  2. Variable-length input is supported. However, each input is recommended to contain at least 5 ms of audio data to avoid overly frequent system calls.
     *  3. The long-term average input rate must match the real-time rate; that is, N seconds of audio data should be provided within every N-second interval.
	 *	   Simply put, here are some easy-to-understand examples:
	 *	   Send 10 ms of audio data every 10 ms:
	 *	       32 kHz mono, 16-bit  : 32000 / 100 x 2 x 1 bytes per 10 ms
	 *	       32 kHz stereo, 16-bit: 32000 / 100 x 2 x 2 bytes per 10 ms

	 *	   Send 20 ms of audio data every 20 ms.:
	 *	       32 kHz mono, 16-bit  : 32000 / 50 x 2 x 1 bytes per 20 ms
	 *	       32 kHz stereo, 16-bit: 32000 / 50 x 2 x 2 bytes per 20 ms
	 *	4. Common sample rates are supported, including 8 kHz, 16 kHz, 32 kHz, 44.1 kHz, and 48 kHz.
	 *	5. Both mono and stereo audio input are supported.
	 */
	virtual SDKError SendAudioDataToChannel(const zchar_t* channelID, const char* pAudioData, unsigned int dataLength, unsigned int sampleRate, ZoomSDKAudioChannel channel) = 0;
	
	/**
	 * @brief Set the background volume - the main meeting audio volume - that people in the talkback channel can hear.
	 * @param channelID Specify the channel ID.
	 * @param backgroundVolume Specify the background volume from 0.0 to 2.0. If you want people in the channel to hear the channel audio more clearly, decrease the backgroundVolume.
	 * @return If the function succeeds, the return value is SDKErr_Success. Otherwise the function fails.
	 */
	virtual SDKError SetChannelBackgroundVolume(const zchar_t* channelID, float backgroundVolume) = 0;
	
	/**
	 * @brief Check if the meeting supports talkback.
	 * @return true if the meeting supports talkback, false otherwise.
	 */
	virtual bool IsMeetingSupportTalkBack() = 0;
};

END_ZOOM_SDK_NAMESPACE
#endif

