import 'dart:async';
import 'dart:convert';
import 'dart:html';
import 'dart:js';

import 'package:crypto/crypto.dart';
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
    var sus = ZoomMtg.checkSystemRequirements();
    var susmap = convertToDart(sus);

    if (kDebugMode) {
      print(susmap);
    }

    ZoomMtg.preLoadWasm();
    ZoomMtg.prepareWebSDK();
    ZoomMtg.init(InitParams(
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
        rwcBackup: options.rwcBackup,
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

  String generateSignature(
      String sdkKey, String sdkSecret, String meetingNumber, int role) {
    final iat =
        ((DateTime.now().millisecondsSinceEpoch - 30000) / 1000).truncate();
    final exp = iat + 60 * 60 * 2;

    final oHeader = {'alg': 'HS256', 'typ': 'JWT'};
    final oPayload = {
      'sdkKey': sdkKey,
      'mn': meetingNumber,
      'role': role,
      'iat': iat,
      'exp': exp,
      'appKey': sdkKey,
      'tokenExp': exp
    };
    final oHeader64 = base64Encode(utf8.encode(jsonEncode(oHeader)));
    final oPayload64 = base64Encode(utf8.encode(jsonEncode(oPayload)));

    final bytes = utf8.encode('$oHeader64.$oPayload64');
    final hmac = Hmac(sha256, utf8.encode(sdkSecret)).convert(bytes);
    final signature = '$oHeader64.$oPayload64.${base64Encode(hmac.bytes)}';

    return signature.replaceAll(RegExp("="), "");
  }

  ButtonElement? _getCloseButton() {
    final btn = document.getElementsByClassName(
        'zm-btn join-dialog__close zm-btn--default zm-btn__outline--blue');
    if (btn.isNotEmpty) {
      return btn[0] as ButtonElement;
    }

    return null;
  }

  ButtonElement? _getJoinButton() {
    final btn = document.getElementsByClassName('join-audio-by-voip__join-btn');
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

  /// Start Meeting Function for Zoom Web
  @override
  Future<List> startMeeting(ZoomMeetingOptions options) async {
    final Completer<List> completer = Completer();
    ZoomMtg.join(JoinParams(
        meetingNumber: options.meetingId,
        userName: options.displayName ?? options.userId,
        signature: options.jwtSignature!,
        sdkKey: options.jwtAPIKey!,
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
        signature: options.jwtSignature!,
        sdkKey: options.jwtAPIKey!,
        passWord: options.meetingPassword,
        success: allowInterop((var res) {
          int counter = 0;
          Future.doWhile(() async {
            await Future.delayed(const Duration(seconds: 1), _joinAudioAuto);
            counter++;
            return counter < 15;
          });
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
