@JS()
library;

import 'dart:js_interop';

/// Initial Parameter Required For Zoom Web
@JS()
@staticInterop
@anonymous
class InitParams {
  external factory InitParams({
    String? leaveUrl,
    String? helper,
    bool? debug,
    bool? showMeetingHeader,
    bool? disableInvite,
    bool? disableCallOut,
    bool? disableRecord,
    bool? disableJoinAudio,
    bool? audioPanelAlwaysOpen,
    bool? isSupportAV,
    bool? isSupportChat,
    bool? isSupportQA,
    bool? isSupportCC,
    bool? isSupportPolling,
    bool? isSupportBreakout,
    bool? screenShare,
    bool? videoDrag,
    String? sharingMode,
    bool? videoHeader,
    bool? isLockBottom,
    bool? isSupportNonverbal,
    bool? isShowJoiningErrorDialog,
    bool? disablePreview,
    bool? disableCORP,
    String? inviteUrlFormat,
    bool? disableVoIP,
    bool? disableReport,
    JSAny? meetingInfo,
    JSFunction? success,
    JSFunction? error,
    String? defaultView,
    bool? enableHD,
    bool? enableFullHD,
    bool? enableWaitingRoomPreview,
    bool? leaveOnPageUnload,
    bool? patchJsMedia,
  });
}

extension InitParamsExtension on InitParams {
  external String? get leaveUrl;
}

@JS()
@staticInterop
@anonymous
class JoinParams {
  external factory JoinParams({
    String? meetingNumber,
    String? userName,
    String? sdkKey,
    String signature,
    String? passWord,
    JSFunction? success,
    JSFunction? error,
  });
}

/// Signature helper
@JS()
@staticInterop
@anonymous
class SignatureParams {
  external factory SignatureParams({
    String meetingNumber,
    String apiKey,
    String apiSecret,
    int role,
  });
}

/// meeting status helper
@JS()
@staticInterop
@anonymous
class MeetingStatus {
  external factory MeetingStatus();
}

extension MeetingStatusExtension on MeetingStatus {
  external int get meetingStatus;
}

@JS()
@staticInterop
@anonymous
class ShowJoinAudioParams {
  external factory ShowJoinAudioParams({bool? show});
}

@JS()
@staticInterop
@anonymous
class LeaveParams {
  external factory LeaveParams({
    JSFunction? cancelCallback,
    bool? confirm,
    JSFunction? success,
    JSFunction? error,
  });
}

@JS()
@staticInterop
@anonymous
class MuteParams {
  external factory MuteParams({String userId, bool? mute});
}

@JS()
@staticInterop
@anonymous
class UserParams {
  external factory UserParams({required JSFunction success, JSFunction? error});
}

@JS()
@staticInterop
@anonymous
class UserSuccessParams {
  external factory UserSuccessParams();
}

extension UserSuccessParamsExtension on UserSuccessParams {
  external UserSuccessResultParams get result;
}

@JS()
@staticInterop
@anonymous
class UserSuccessResultParams {
  external factory UserSuccessResultParams();
}

extension UserSuccessResultParamsExtension on UserSuccessResultParams {
  external CurrentUserParams get currentUser;
}

@JS()
@staticInterop
@anonymous
class CurrentUserParams {
  external factory CurrentUserParams();
}

extension CurrentUserParamsExtension on CurrentUserParams {
  external bool? get muted;

  external String? get audio;
}

/// Basic Zoom SDK Web Functions helper
@JS()
@staticInterop
class ZoomMtg {
  external static void setZoomJSLib(String path, String dir);

  external static ZoomMtgLang get i18n;

  external static void preLoadWasm();

  external static void prepareWebSDK();

  external static void prepareJssdk();

  external static void init(InitParams initParams);

  external static void join(JoinParams joinParams);

  external static void mute(MuteParams params);

  @JS('getAttendeeslist')
  external static JSAny getAttendeeslist();

  external static void getCurrentUser(UserParams params);

  external static void showJoinAudioFunction(ShowJoinAudioParams params);

  external static void leaveMeeting(LeaveParams params);

  external static String generateSDKSignature(SignatureParams signatureParams);

  external static JSAny checkFeatureRequirements();

  external static void inMeetingServiceListener(
    String event,
    JSFunction callback,
  );
}

@JS()
@staticInterop
class ZoomMtgLang {
  external factory ZoomMtgLang();
}

extension ZoomMtgLangExtension on ZoomMtgLang {
  external String getCurrentLang();

  external void load(String lang);
}
