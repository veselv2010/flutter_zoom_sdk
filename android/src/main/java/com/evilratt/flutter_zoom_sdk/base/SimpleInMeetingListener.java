package com.evilratt.flutter_zoom_sdk.base;

import java.util.List;

import us.zoom.sdk.CameraControlRequestType;
import us.zoom.sdk.ChatMessageDeleteType;
import us.zoom.sdk.FreeMeetingNeedUpgradeType;
import us.zoom.sdk.ICameraControlRequestHandler;
import us.zoom.sdk.IMeetingArchiveConfirmHandler;
import us.zoom.sdk.IMeetingInputUserInfoHandler;
import us.zoom.sdk.IRequestLocalRecordingPrivilegeHandler;
import us.zoom.sdk.InMeetingAudioController;
import us.zoom.sdk.InMeetingChatController;
import us.zoom.sdk.InMeetingChatMessage;
import us.zoom.sdk.InMeetingEventHandler;
import us.zoom.sdk.InMeetingServiceListener;
import us.zoom.sdk.LocalRecordingRequestPrivilegeStatus;
import us.zoom.sdk.MobileRTCFocusModeShareType;
import us.zoom.sdk.VideoQuality;
import us.zoom.sdk.CameraControlRequestResult;
import us.zoom.sdk.ZoomSDKFileSender;
import us.zoom.sdk.ZoomSDKFileReceiver;
import us.zoom.sdk.ZoomSDKFileTransferInfo;

public class SimpleInMeetingListener implements InMeetingServiceListener {

    @Override
    public void onMeetingNeedPasswordOrDisplayName(boolean b, boolean b1, InMeetingEventHandler inMeetingEventHandler) {

    }

    @Override
    public void onWebinarNeedRegister(String s) {

    }

    @Override
    public void onJoinMeetingNeedUserInfo(IMeetingInputUserInfoHandler iMeetingInputUserInfoHandler) {

    }

    @Override
    public void onJoinWebinarNeedUserNameAndEmail(InMeetingEventHandler inMeetingEventHandler) {

    }

    @Override
    public void onMeetingNeedCloseOtherMeeting(InMeetingEventHandler inMeetingEventHandler) {

    }

    @Override
    public void onMeetingFail(int i, int i1) {

    }

    @Override
    public void onMeetingLeaveComplete(long l) {

    }

    @Override
    public void onMeetingUserJoin(List<Long> list) {

    }

    @Override
    public void onMeetingUserLeave(List<Long> list) {

    }

    @Override
    public void onMeetingUserUpdated(long l) {

    }

    @Override
    public void onInMeetingUserAvatarPathUpdated(long l) {

    }

    @Override
    public void onMeetingHostChanged(long l) {

    }

    @Override
    public void onMeetingCoHostChange(long l, boolean b) {

    }

    @Override
    public void onActiveVideoUserChanged(long l) {

    }

    @Override
    public void onActiveSpeakerVideoUserChanged(long l) {

    }

    @Override
    public void onHostVideoOrderUpdated(List<Long> list) {

    }

    @Override
    public void onFollowHostVideoOrderChanged(boolean b) {

    }

    @Override
    public void onSpotlightVideoChanged(List<Long> list) {

    }

    @Override
    public void onUserVideoStatusChanged(long l, VideoStatus videoStatus) {

    }

    @Override
    public void onSinkMeetingVideoQualityChanged(VideoQuality videoQuality, long l) {

    }

    @Override
    public void onMicrophoneStatusError(InMeetingAudioController.MobileRTCMicrophoneError mobileRTCMicrophoneError) {

    }

    @Override
    public void onUserAudioStatusChanged(long l, AudioStatus audioStatus) {

    }

    @Override
    public void onHostAskUnMute(long l) {

    }

    @Override
    public void onHostAskStartVideo(long l) {

    }

    @Override
    public void onUserAudioTypeChanged(long l) {

    }

    @Override
    public void onMyAudioSourceTypeChanged(int i) {

    }

    @Override
    public void onLowOrRaiseHandStatusChanged(long l, boolean b) {

    }

    @Override
    public void onChatMessageReceived(InMeetingChatMessage inMeetingChatMessage) {

    }

    @Override
    public void onChatMsgDeleteNotification(String s, ChatMessageDeleteType chatMessageDeleteType) {

    }

    @Override
    public void onShareMeetingChatStatusChanged(boolean b) {

    }

    @Override
    public void onSilentModeChanged(boolean b) {

    }

    @Override
    public void onFreeMeetingReminder(boolean b, boolean b1, boolean b2) {

    }

    @Override
    public void onMeetingActiveVideo(long l) {

    }

    @Override
    public void onSinkAttendeeChatPriviledgeChanged(int i) {

    }

    @Override
    public void onSinkAllowAttendeeChatNotification(int i) {

    }

    @Override
    public void onSinkPanelistChatPrivilegeChanged(InMeetingChatController.MobileRTCWebinarPanelistChatPrivilege mobileRTCWebinarPanelistChatPrivilege) {

    }

    @Override
    public void onUserNamesChanged(List<Long> list) {

    }

    @Override
    public void onFreeMeetingNeedToUpgrade(FreeMeetingNeedUpgradeType freeMeetingNeedUpgradeType, String s) {

    }

    @Override
    public void onFreeMeetingUpgradeToGiftFreeTrialStart() {

    }

    @Override
    public void onFreeMeetingUpgradeToGiftFreeTrialStop() {

    }

    @Override
    public void onFreeMeetingUpgradeToProMeeting() {

    }

    @Override
    public void onClosedCaptionReceived(String s, long l) {

    }

    @Override
    public void onRecordingStatus(RecordingStatus recordingStatus) {

    }

    @Override
    public void onLocalRecordingStatus(long l, RecordingStatus recordingStatus) {

    }

    @Override
    public void onInvalidReclaimHostkey() {

    }

    @Override
    public void onPermissionRequested(String[] strings) {

    }

    @Override
    public void onAllHandsLowered() {

    }

    @Override
    public void onLocalVideoOrderUpdated(List<Long> list) {

    }

    @Override
    public void onLocalRecordingPrivilegeRequested(IRequestLocalRecordingPrivilegeHandler iRequestLocalRecordingPrivilegeHandler) {

    }

    @Override
    public void onSuspendParticipantsActivities() {

    }

    @Override
    public void onAllowParticipantsStartVideoNotification(boolean allow) {

    }

    @Override
    public void onAllowParticipantsRenameNotification(boolean allow) {

    }

    @Override
    public void onAllowParticipantsUnmuteSelfNotification(boolean allow) {

    }

    @Override
    public void onAllowParticipantsShareWhiteBoardNotification(boolean allow) {

    }

    @Override
    public void onAICompanionActiveChangeNotice(boolean active) {

    }

    @Override
    public void onMeetingLockStatus(boolean isLock) {

    }

    @Override
    public void onRequestLocalRecordingPrivilegeChanged(LocalRecordingRequestPrivilegeStatus status) {

    }

    @Override
    public void onParticipantProfilePictureStatusChange(boolean hidden) {

    }

    @Override
    public void onCloudRecordingStorageFull(long gracePeriodDate) {

    }

    @Override
    public void onUVCCameraStatusChange(java.lang.String cameraId, InMeetingServiceListener.UVCCameraStatus status) {

    }

    @Override
    public void onFocusModeStateChanged(boolean on) {

    }

    @Override
    public void onFocusModeShareTypeChanged(MobileRTCFocusModeShareType shareType) {

    }

    @Override
    public void onVideoAlphaChannelStatusChanged(boolean isAlphaModeOn) {

    }

    @Override
    public void onAllowParticipantsRequestCloudRecording(boolean bAllow) {

    }

    @Override
    public void onSinkJoin3rdPartyTelephonyAudio(java.lang.String audioInfo) {

    }

    @Override
    public void onUserConfirmToStartArchive(IMeetingArchiveConfirmHandler iMeetingArchiveConfirmHandler) {

    }

    @Override
    public void onCameraControlRequestReceived(long l, CameraControlRequestType cameraControlRequestType, ICameraControlRequestHandler iCameraControlRequestHandler) {

    }

    @Override
    public void onWebinarNeedInputScreenName(InMeetingEventHandler handler) {

    }

    @Deprecated
    @Override
    public void onCameraControlRequestResult(long userId, boolean isApproved) {

    }

    @Override
    public void onCameraControlRequestResult(long userId, CameraControlRequestResult result) {

    }

    @Override
    public void onFileSendStart(ZoomSDKFileSender sender) {

    }

    @Override
    public void onFileReceived(ZoomSDKFileReceiver receiver) {

    }

    @Override
    public void onFileTransferProgress(ZoomSDKFileTransferInfo info) {

    }

    @Override
    public void onMuteOnEntryStatusChange(boolean enable) {

    }

    @Override
    public void onMeetingTopicChanged(java.lang.String topic) {

    }
}
