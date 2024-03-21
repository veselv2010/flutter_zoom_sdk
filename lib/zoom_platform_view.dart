import 'dart:async';
import 'dart:convert';

import 'package:flutter/foundation.dart';
import 'package:flutter_zoom_sdk/zoom_options.dart';
import 'package:flutter_zoom_sdk/zoom_view.dart';
import 'package:crypto/crypto.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'models/audio_types.dart';
export 'zoom_options.dart';
export 'models/audio_types.dart';

abstract class ZoomPlatform extends PlatformInterface {
  ZoomPlatform() : super(token: _token);
  static final Object _token = Object();
  static ZoomPlatform _instance = ZoomView();

  static ZoomPlatform get instance => _instance;

  static set instance(ZoomPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  /// Flutter prepare Zoom SDK function only for web
  void prepareWebZoomSDK() {
    throw UnimplementedError('prepareWebZoomSDK() has not been implemented.');
  }

  /// Flutter Zoom SDK Initialization function
  /// alreadyInit only for web
  Future<List> initZoom(ZoomOptions options, {bool alreadyInit = false}) async {
    throw UnimplementedError('initZoom() has not been implemented.');
  }

  /// Flutter Zoom SDK Initialization and Join Meeting function for desktop
  Future<bool> initZoomAndJoinMeeting(
    ZoomOptions zoomOptions,
    ZoomMeetingOptions meetingOptions,
  ) async {
    throw UnimplementedError(
        'initZoomAndJoinMeeting() has not been implemented.');
  }

  /// Flutter Zoom SDK Generate Signature for Web and native platforms
  String generateSignature(
    String sdkKey,
    String sdkSecret,
    String meetingNumber,
    int role,
  ) {
    final iat = (DateTime.now().millisecondsSinceEpoch - 30000) ~/ 1000;
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
    if (kDebugMode) {
      print(oPayload.entries
          .map((entry) => '${entry.key}: ${entry.value.toString()}')
          .join('\n'));
    }
    final oHeader64 = base64Encode(utf8.encode(jsonEncode(oHeader)));
    final oPayload64 = base64Encode(utf8.encode(jsonEncode(oPayload)));

    final bytes = utf8.encode('$oHeader64.$oPayload64');
    final hmac = Hmac(sha256, utf8.encode(sdkSecret)).convert(bytes);
    final signature = '$oHeader64.$oPayload64.${base64Encode(hmac.bytes)}';

    return signature.replaceAll(RegExp('='), '').replaceAll('+/', '-_');
  }

  /// Flutter Zoom SDK Start Meeting function
  Future<List> startMeeting(ZoomMeetingOptions options) async {
    throw UnimplementedError('startMeetingLogin() has not been implemented.');
  }

  /// Flutter Zoom SDK Start Meeting with Custom Meeting ID function
  Future<List> startMeetingNormal(ZoomMeetingOptions options) async {
    throw UnimplementedError('startMeetingNormal() has not been implemented.');
  }

  /// Flutter Zoom SDK Join Meeting function
  Future<bool> joinMeeting(
    ZoomMeetingOptions options,
    bool autoAudioJoin,
  ) async {
    throw UnimplementedError('joinMeeting() has not been implemented.');
  }

  /// Flutter Zoom SDK Get Meeting Status function
  Future<List> meetingStatus(String meetingId) async {
    throw UnimplementedError('meetingStatus() has not been implemented.');
  }

  /// Flutter Zoom SDK Listen to Meeting Status function
  Stream<dynamic> onMeetingStatus() {
    throw UnimplementedError('onMeetingStatus() has not been implemented.');
  }

  /// Flutter Zoom SDK Get Meeting ID & Passcode after Starting Meeting function
  Future<List> meetingDetails() async {
    throw UnimplementedError('meetingDetails() has not been implemented.');
  }

  Future<bool> leaveMeeting() async {
    throw UnimplementedError('leaveMeeting() has not been implemented.');
  }

  Future<bool> showMeeting() async {
    throw UnimplementedError('openZoomActivity() has not been implemented.');
  }

  Future<bool> hideMeeting({bool isWindows = false}) async {
    throw UnimplementedError('putToBgZoomActivity() has not been implemented.');
  }

  Future<bool> getMicStatus() {
    throw UnimplementedError('getMicStatus() has not been implemented.');
  }

  Future<bool> muteMyAudio({bool isMuted = true}) {
    throw UnimplementedError('muteMyAudio() has not been implemented.');
  }

  Stream<ZmAudioStatus> getMicStatusStream() {
    throw UnimplementedError('getMicStatusStream() has not been implemented.');
  }
}
