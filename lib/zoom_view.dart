import 'dart:async';
import 'package:flutter/services.dart';
import 'package:flutter_zoom_sdk/zoom_platform_view.dart';

class ZoomView extends ZoomPlatform {
  final MethodChannel channel = const MethodChannel('flutter_zoom_sdk');

  /// The event channel used to interact with the native platform.
  final EventChannel eventChannel =
      const EventChannel('flutter_zoom_sdk_event_stream');

  /// The event channel used to interact with the native platform init function
  @override
  Future<List> initZoom(ZoomOptions options) async {
    var optionMap = <String, String?>{};

    if (options.appKey != null) {
      optionMap.putIfAbsent("appKey", () => options.appKey!);
    }
    if (options.appSecret != null) {
      optionMap.putIfAbsent("appSecret", () => options.appSecret!);
    }
    if (options.returnBtnMsg != null) {
      optionMap.putIfAbsent("returnBtnMsg", () => options.returnBtnMsg!);
    }

    optionMap.putIfAbsent("domain", () => options.domain);
    return await channel
        .invokeMethod<List>('init', optionMap)
        .then<List>((List? value) => value ?? List.empty());
  }

  /// The event channel used to interact with the native platform init function
  @override
  Future<bool> initZoomAndJoinMeeting(
    ZoomOptions zoomOptions,
    ZoomMeetingOptions meetingOptions,
  ) async {
    var optionsMap = <String, Map>{};
    var initOptionsMap = <String, String?>{};
    var meetingOptionsMap = <String, String?>{};

    initOptionsMap.putIfAbsent("appKey", () => zoomOptions.appKey);
    initOptionsMap.putIfAbsent("appSecret", () => zoomOptions.appSecret);
    initOptionsMap.putIfAbsent("returnBtnMsg", () => zoomOptions.returnBtnMsg);
    initOptionsMap.putIfAbsent("domain", () => zoomOptions.domain);

    meetingOptionsMap.putIfAbsent("displayName", () => meetingOptions.displayName);
    meetingOptionsMap.putIfAbsent("meetingId", () => meetingOptions.meetingId);
    meetingOptionsMap.putIfAbsent("meetingPassword", () => meetingOptions.meetingPassword);
    meetingOptionsMap.putIfAbsent("noAudio", () => meetingOptions.noAudio);
    meetingOptionsMap.putIfAbsent("noVideo", () => meetingOptions.noVideo);

    optionsMap.putIfAbsent('initOptions', () => initOptionsMap);
    optionsMap.putIfAbsent('meetingOptions', () => meetingOptionsMap);

    return await channel
        .invokeMethod<bool>('init_and_join', optionsMap)
        .then<bool>((bool? value) => value ?? false);
  }

  /// The event channel used to interact with the native platform startMeetingNormal function
  @override
  Future<List> startMeetingNormal(ZoomMeetingOptions options) async {
    var optionMap = <String, String?>{};
    optionMap.putIfAbsent("userId", () => options.userId);
    optionMap.putIfAbsent("userPassword", () => options.userPassword);
    optionMap.putIfAbsent("meetingId", () => options.meetingId);
    optionMap.putIfAbsent("disableDialIn", () => options.disableDialIn);
    optionMap.putIfAbsent("disableDrive", () => options.disableDrive);
    optionMap.putIfAbsent("disableInvite", () => options.disableInvite);
    optionMap.putIfAbsent("disableShare", () => options.disableShare);
    optionMap.putIfAbsent("disableTitlebar", () => options.disableTitlebar);
    optionMap.putIfAbsent("noDisconnectAudio", () => options.noDisconnectAudio);
    optionMap.putIfAbsent("noAudio", () => options.noAudio);
    optionMap.putIfAbsent("viewOptions", () => options.viewOptions);

    return await channel
        .invokeMethod<List>('start_normal', optionMap)
        .then<List>((List? value) => value ?? List.empty());
  }

  /// The event channel used to interact with the native platform joinMeeting function
  @override
  Future<bool> joinMeeting(
    ZoomMeetingOptions options,
    bool autoAudioJoin,
  ) async {
    var optionMap = <String, String?>{};
    optionMap.putIfAbsent("userId", () => options.userId);
    optionMap.putIfAbsent("displayName", () => options.displayName);
    optionMap.putIfAbsent("meetingId", () => options.meetingId);
    optionMap.putIfAbsent("meetingPassword", () => options.meetingPassword);
    optionMap.putIfAbsent("disableDialIn", () => options.disableDialIn);
    optionMap.putIfAbsent("disableDrive", () => options.disableDrive);
    optionMap.putIfAbsent("disableInvite", () => options.disableInvite);
    optionMap.putIfAbsent("disableShare", () => options.disableShare);
    optionMap.putIfAbsent("disableTitlebar", () => options.disableTitlebar);
    optionMap.putIfAbsent("noDisconnectAudio", () => options.noDisconnectAudio);
    optionMap.putIfAbsent("viewOptions", () => options.viewOptions);
    optionMap.putIfAbsent("noAudio", () => options.noAudio);
    optionMap.putIfAbsent("noVideo", () => options.noVideo);

    return await channel
        .invokeMethod<bool>('join', optionMap)
        .then<bool>((bool? value) => value ?? false);
  }

  /// The event channel used to interact with the native platform startMeeting(login on iOS & Android) function
  @override
  Future<List> startMeeting(ZoomMeetingOptions options) async {
    var optionMap = <String, String?>{};
    optionMap.putIfAbsent("userId", () => options.userId);
    optionMap.putIfAbsent("userPassword", () => options.userPassword);
    optionMap.putIfAbsent("disableDialIn", () => options.disableDialIn);
    optionMap.putIfAbsent("disableDrive", () => options.disableDrive);
    optionMap.putIfAbsent("disableInvite", () => options.disableInvite);
    optionMap.putIfAbsent("disableShare", () => options.disableShare);
    optionMap.putIfAbsent("disableTitlebar", () => options.disableTitlebar);
    optionMap.putIfAbsent("viewOptions", () => options.viewOptions);
    optionMap.putIfAbsent("noDisconnectAudio", () => options.noDisconnectAudio);
    optionMap.putIfAbsent("noAudio", () => options.noAudio);

    return await channel
        .invokeMethod<List>('login', optionMap)
        .then<List>((List? value) => value ?? List.empty());
  }

  /// The event channel used to interact with the native platform meetingStatus function
  @override
  Future<List> meetingStatus(String meetingId) async {
    var optionMap = <String, String>{};
    optionMap.putIfAbsent("meetingId", () => meetingId);

    return await channel
        .invokeMethod<List>('meeting_status', optionMap)
        .then<List>((List? value) => value ?? List.empty());
  }

  /// The event channel used to interact with the native platform onMeetingStatus(iOS & Android) function
  @override
  Stream<dynamic> onMeetingStatus() {
    return eventChannel.receiveBroadcastStream();
  }

  /// The event channel used to interact with the native platform meetinDetails(iOS & Android) function
  @override
  Future<List> meetingDetails() async {
    return await channel
        .invokeMethod<List>('meeting_details')
        .then<List>((List? value) => value ?? List.empty());
  }

  /// The event channel used to interact with the native platform showMeeting(Android & Windows) function
  @override
  Future<bool> showMeeting() async {
    return await channel
        .invokeMethod<bool>('show_meeting')
        .then<bool>((bool? value) => value ?? false);
  }

  /// The event channel used to interact with the native platform hideMeeting(Android & Windows) function
  @override
  Future<bool> hideMeeting() async {
    return await channel
        .invokeMethod<bool>('hide_meeting')
        .then<bool>((bool? value) => value ?? false);
  }

  @override
  Future<bool> leaveMeeting() async {
    return await channel
        .invokeMethod<bool>('leave_meeting')
        .then<bool>((bool? value) => value ?? false);
  }
}
