import 'dart:async';
import 'dart:html';
import 'dart:js';

import 'package:flutter/foundation.dart';
import 'package:flutter_web_plugins/flutter_web_plugins.dart';
import 'package:flutter_zoom_sdk/web/zoom_js.dart';
import 'package:flutter_zoom_sdk/zoom_platform_view.dart';

import 'web/js_interop.dart';

export 'package:flutter_zoom_sdk/zoom_platform_view.dart'
    show ZoomOptions, ZoomMeetingOptions;

/// Web Function For Zoom Sdk for Flutter Web Implementation
class ZoomViewWeb extends ZoomPlatform {
  StreamController<dynamic>? streamController;

  static void registerWith(Registrar registrar) {
    ZoomPlatform.instance = ZoomViewWeb();
  }

  /// Initialize Zoom SDK For Web
  @override
  Future<List> initZoom(ZoomOptions options) async {
    final Completer<List> completer = Completer();
    var sus = ZoomMtg.checkFeatureRequirements();
    var susmap = convertToDart(sus);

    if (kDebugMode) {
      print(susmap);
    }

    ZoomMtg.preLoadWasm();
    ZoomMtg.prepareWebSDK();

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
        meetingInfo: options.meetingInfo,
        success: allowInterop((var res) {
          completer.complete([0, 0]);
        }),
        error: allowInterop((var res) {
          completer.complete([1, 0]);
        })));
    return completer.future;
  }

  ButtonElement? _getCloseButton() {
    final btn = document.getElementsByClassName(
        'zm-btn join-dialog__close zm-btn--default zm-btn__outline--blue');
    if (btn.isNotEmpty) {
      return btn[0] as ButtonElement;
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
    final joinDialog = document.getElementsByClassName("join-dialog");

    if (button != null && joinDialog.isEmpty) {
      button.click();
    }
  }

  ButtonElement? _getJoinButton() {
    final btn = document.getElementsByClassName('join-audio-by-voip__join-btn');
    if (btn.isNotEmpty) {
      return btn[0] as ButtonElement;
    }

    return null;
  }

  ButtonElement? _getAudioButton() {
    final btn = document.getElementsByClassName('join-audio-container__btn');
    if (btn.isNotEmpty) {
      return btn[0] as ButtonElement;
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
    final Completer<List> completer = Completer();
    ZoomMtg.join(JoinParams(
        meetingNumber: options.meetingId,
        userName: options.displayName ?? options.userId ?? 'user',
        signature: options.signature!,
        sdkKey: options.sdkKey!,
        passWord: options.meetingPassword,
        success: allowInterop((var res) {
          completer.complete(["MEETING STATUS", "SUCCESS"]);
        }),
        error: allowInterop((var res) {
          completer.complete(["MEETING STATUS", "FAILED"]);
        })));
    return completer.future;
  }

  /// Join Meeting Function for Zoom Web
  @override
  Future<bool> joinMeeting(
      ZoomMeetingOptions options, bool autoAudioJoin) async {
    // final Completer<bool> completer = Completer();
    ZoomMtg.join(
      JoinParams(
        meetingNumber: options.meetingId,
        userName: options.displayName ?? options.userId,
        signature: options.signature!,
        sdkKey: options.sdkKey!,
        passWord: options.meetingPassword,
        success: allowInterop(() async {
          if (autoAudioJoin) {
            bool continueLoop = true;

            await Future.doWhile(() async {
              await _connectAudio();
              await Future.delayed(const Duration(seconds: 1));
              ZoomMtg.getCurrentUser(
                UserParams(
                  success: allowInterop((var res) {
                    final isCurUserMuted = res.result.currentUser.muted;
                    final curUserAudio = res.result.currentUser.audio;

                    if (isCurUserMuted == true) {
                      _clickOnAudioButton();
                    }

                    if (curUserAudio != null && curUserAudio != '') {
                      continueLoop = false;
                    }
                  }),
                ),
              );

              // int counter = 0;
              // Future.doWhile(() async {
              //   await Future.delayed(const Duration(seconds: 1), _joinAudioAuto);
              //   counter++;
              //   return counter < 15;
              // });

              return continueLoop;
            });
          }
          return;
        }),
        // error: allowInterop(
        //   (var res) {
        //     completer.complete(false);
        //   },
        // ),
      ),
    );
    return true;
  }

  @override
  Future<bool> leaveMeeting() async {
    final Completer<bool> completer = Completer();

    ZoomMtg.leaveMeeting(LeaveParams(success: allowInterop((var res) {
      completer.complete(true);
    }), error: allowInterop((var res) {
      completer.complete(false);
    })));

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
    ZoomMtg.inMeetingServiceListener('onMeetingStatus', allowInterop((status) {
      if (status == null) return;

      var r = List<String>.filled(2, "");

      /// 1(connecting), 2(connected), 3(disconnected), 4(reconnecting)
      switch (status.meetingStatus) {
        case 1:
          r[0] = "MEETING_STATUS_CONNECTING";
          break;
        case 2:
          r[0] = "MEETING_STATUS_INMEETING";
          break;
        case 3:
          r[0] = "MEETING_STATUS_DISCONNECTING";
          break;
        case 4:
          r[0] = "MEETING_STATUS_INMEETING";
          break;
        default:
          r[0] = status.meetingStatus.toString();
          break;
      }
      streamController!.add(r);
    }));
    return streamController!.stream;
  }
}
