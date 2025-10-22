import 'dart:async';
import 'dart:js_interop';
import 'package:web/web.dart' as web;
import 'package:flutter/foundation.dart';
import 'package:flutter_web_plugins/flutter_web_plugins.dart';
import 'package:flutter_zoom_sdk/web/zoom_js.dart';
import 'package:flutter_zoom_sdk/zoom_platform_view.dart';
import 'package:flutter_zoom_sdk/web/stringify_js.dart';

export 'package:flutter_zoom_sdk/zoom_platform_view.dart'
    show ZoomOptions, ZoomMeetingOptions;

/// Web Function For Zoom Sdk for Flutter Web Implementation
class ZoomViewWeb extends ZoomPlatform {
  StreamController<dynamic>? streamController;

  static void registerWith(Registrar registrar) {
    ZoomPlatform.instance = ZoomViewWeb();
  }

  @override
  void prepareWebZoomSDK() {
    ZoomMtg.prepareWebSDK();
  }

  JSArray<JSString>? _listToJSArray(List<String>? list) {
    if (list == null) return null;

    final jsStrings = list.map((s) => s.toJS).toList();

    return jsStrings.toJS;
  }

  /// Initialize Zoom SDK For Web
  /// alreadyInit when method prepareWebZoomSDK was called earlier
  @override
  Future<List> initZoom(ZoomOptions options, {bool alreadyInit = false}) async {
    final completer = Completer<List>();
    var sus = ZoomMtg.checkFeatureRequirements();

    if (kDebugMode) {
      print(stringify(sus));
    }

    ZoomMtg.preLoadWasm();

    if (!alreadyInit) {
      prepareWebZoomSDK();
    }

    ZoomMtg.i18n.load(options.language ?? 'en-US');
    debugPrint('Current language of Zoom - ${ZoomMtg.i18n.getCurrentLang()}');

    ZoomMtg.init(InitParams(
      debug: options.debug ?? kDebugMode,
      helper: options.helper,
      leaveUrl: options.leaveUrl ?? "/index.html",
      showMeetingHeader: options.showMeetingHeader,
      disableInvite: options.disableInvite,
      disableCallOut: options.disableCallOut,
      disableRecord: options.disableRecord,
      disableJoinAudio: options.disableJoinAudio,
      audioPanelAlwaysOpen: options.audioPanelAlwaysOpen,
      isSupportAV: options.isSupportAV,
      isSupportChat: options.isSupportChat,
      isSupportQA: options.isSupportQA,
      isSupportCC: options.isSupportCC,
      isSupportPolling: options.isSupportPolling,
      isSupportBreakout: options.isSupportBreakout,
      screenShare: options.screenShare,
      videoDrag: options.videoDrag,
      sharingMode: options.sharingMode,
      videoHeader: options.videoHeader,
      isLockBottom: options.isLockBottom,
      isSupportNonverbal: options.isSupportNonverbal,
      isShowJoiningErrorDialog: options.isShowJoiningErrorDialog,
      disablePreview: options.disablePreview,
      disableCORP: options.disableCORP,
      inviteUrlFormat: options.inviteUrlFormat,
      disableVoIP: options.disableVOIP,
      disableReport: options.disableReport,
      meetingInfo: _listToJSArray(options.meetingInfo),
      defaultView: options.defaultView,
      enableHD: options.enableHD,
      enableFullHD: options.enableFullHD,
      leaveOnPageUnload: options.leaveOnPageUnload,
      patchJsMedia: options.patchJsMedia,
      success: (([JSAny? initResult]) {
        completer.complete([0, 0]);
      }).toJS,
      error: (([JSAny? initError]) {
        completer.complete([1, 0]);
      }).toJS,
    ));

    return completer.future;
  }

  web.HTMLButtonElement? _getCloseButton() {
    final btn = web.document.getElementsByClassName(
      'zm-btn join-dialog__close zm-btn--default zm-btn__outline--blue',
    );

    if (btn.length > 0) {
      return btn.item(0) as web.HTMLButtonElement;
    }

    return null;
  }

  Future<bool> _connectAudio() async {
    final button = _getJoinButton();

    if (button == null) {
      await Future.delayed(const Duration(seconds: 1), _openJoinAudio);
    }

    return Future.delayed(const Duration(seconds: 1), _joinAudioAuto);
  }

  Future<void> _openJoinAudio() async {
    final button = _getAudioButton();
    final joinDialog = web.document.getElementsByClassName("join-dialog");

    if (button != null && joinDialog.length == 0) {
      button.click();
    }
  }

  web.HTMLButtonElement? _getJoinButton() {
    final btn =
        web.document.getElementsByClassName('join-audio-by-voip__join-btn');

    if (btn.length > 0) {
      return btn.item(0) as web.HTMLButtonElement;
    }

    return null;
  }

  web.HTMLButtonElement? _getAudioButton() {
    final btn =
        web.document.getElementsByClassName('join-audio-container__btn');

    if (btn.length > 0) {
      return btn.item(0) as web.HTMLButtonElement;
    }

    return null;
  }

  bool _joinAudioAuto() {
    final button = _getJoinButton();
    // final button = _getCloseButton();

    if (button != null) {
      button.click();
      return true;
    }

    return false;
  }

  void _clickOnAudioButton() {
    final button = _getAudioButton();

    if (button != null) {
      button.click();
    }
  }

  /// Start Meeting Function for Zoom Web
  @override
  Future<List> startMeeting(ZoomMeetingOptions options) async {
    final completer = Completer<List>();

    ZoomMtg.join(JoinParams(
      meetingNumber: options.meetingId,
      userName: options.displayName ?? options.userId ?? 'user',
      signature: options.signature!,
      passWord: options.meetingPassword,
      success: (([JSAny? startResult]) {
        completer.complete(["MEETING STATUS", "SUCCESS"]);
      }).toJS,
      error: (([JSAny? startError]) {
        completer.complete(["MEETING STATUS", "FAILED"]);
      }).toJS,
    ));

    return completer.future;
  }

  /// Join Meeting Function for Zoom Web
  @override
  Future<bool> joinMeeting(
    ZoomMeetingOptions options,
    bool autoAudioJoin,
  ) async {
    ZoomMtg.join(JoinParams(
      meetingNumber: options.meetingId,
      userName: options.displayName ?? options.userId,
      signature: options.signature!,
      passWord: options.meetingPassword,
      sdkKey: options.sdkKey,
      success: (([JSAny? joinResult]) {
        if (autoAudioJoin) {
          bool continueLoop = true;

          Future.doWhile(() async {
            await _connectAudio();
            await Future.delayed(const Duration(seconds: 1));

            ZoomMtg.getCurrentUser(UserParams(
                success: (([JSAny? currentUserJs]) {
              if (currentUserJs == null) return;

              final userSuccess = currentUserJs as UserSuccessParams;
              final isCurUserMuted = userSuccess.result.currentUser.muted;
              final curUserAudio = userSuccess.result.currentUser.audio;

              if (isCurUserMuted == true) {
                _clickOnAudioButton();
              }

              if (curUserAudio != null && curUserAudio != '') {
                continueLoop = false;
              }
            }).toJS));

            return continueLoop;
          });
        }
      }).toJS,
      error: (([JSAny? joinError]) {}).toJS,
    ));

    return true;
  }

  @override
  Future<bool> leaveMeeting({bool confirm = false}) async {
    final completer = Completer<bool>();

    ZoomMtg.leaveMeeting(LeaveParams(
      confirm: confirm,
      cancelCallback: (([JSAny? _]) {
        completer.complete(false);
      }).toJS,
      success: (([JSAny? leaveResult]) {
        completer.complete(true);
      }).toJS,
      error: (([JSAny? leaveError]) {
        completer.complete(false);
      }).toJS,
    ));

    return completer.future;
  }

  /// Listen to Ongoing Meeting Function for Zoom Web
  @override
  Future<List> meetingStatus(String meetingId) async {
    return ["a", "b"];
  }

  /// Meeting Status Response Function for Zoom Web
  @override
  Stream<dynamic> onMeetingStatus() {
    streamController?.close();
    streamController = StreamController<dynamic>();

    ZoomMtg.inMeetingServiceListener(
        'onMeetingStatus',
        (([JSAny? statusJs]) {
          if (statusJs == null) return;

          final status = statusJs as MeetingStatus;
          var statusArray = List<String>.filled(2, "");

          /// 1(connecting), 2(connected), 3(disconnected), 4(reconnecting)
          switch (status.meetingStatus) {
            case 1:
              statusArray[0] = "MEETING_STATUS_CONNECTING";
              break;
            case 2:
              statusArray[0] = "MEETING_STATUS_INMEETING";
              break;
            case 3:
              statusArray[0] = "MEETING_STATUS_DISCONNECTING";
              break;
            case 4:
              statusArray[0] = "MEETING_STATUS_INMEETING";
              break;
            default:
              statusArray[0] = status.meetingStatus.toString();
              break;
          }

          streamController!.add(statusArray);
        }).toJS);

    return streamController!.stream;
  }
}
