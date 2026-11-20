package com.evilratt.flutter_zoom_sdk.audio;

import androidx.annotation.NonNull;

import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import us.zoom.sdk.InMeetingAudioController;
import us.zoom.sdk.InMeetingService;
import us.zoom.sdk.MobileRTCSDKError;

public class FlutterSdkAudioHandler implements MethodChannel.MethodCallHandler, EventChannel.StreamHandler {
    private InMeetingService meetingService;
    private EventChannel.EventSink events;
    private InMeetingAudioStateListener listener;

    public void setMeetingService(InMeetingService service) {
        if (meetingService == service) {
            return;
        }
        removeListener();
        meetingService = service;
        addListener();
    }

    @Override
    public void onMethodCall(@NonNull MethodCall call, @NonNull MethodChannel.Result result) {
        switch (call.method) {
            case "getMicStatus":
            case "muteMyAudio":
                break;
            default:
                result.notImplemented();
                return;
        }

        InMeetingAudioController controller = meetingService != null && meetingService.isMeetingConnected()
                ? meetingService.getInMeetingAudioController() : null;
        if (controller == null) {
            result.error("NO_MEETING", "Zoom meeting is not connected", null);
            return;
        }
        if ("getMicStatus".equals(call.method)) {
            result.success(controller.isMyAudioMuted());
            return;
        }
        if (!(call.arguments instanceof Boolean)) {
            result.error("INVALID_ARGUMENTS", "Expected a mute state", null);
            return;
        }
        boolean muted = (Boolean) call.arguments;
        MobileRTCSDKError error = controller.muteMyAudio(muted);
        if (error == MobileRTCSDKError.SDKERR_SUCCESS) {
            result.success(muted);
        } else {
            result.error("ZOOM_AUDIO_ERROR", error != null ? error.name() : "Unknown error", null);
        }
    }

    @Override
    public void onListen(Object arguments, EventChannel.EventSink events) {
        onCancel(null);
        this.events = events;
        addListener();
    }

    @Override
    public void onCancel(Object arguments) {
        removeListener();
        events = null;
    }

    private void addListener() {
        if (meetingService != null && events != null) {
            listener = new InMeetingAudioStateListener(meetingService, events);
            meetingService.addListener(listener);
        }
    }

    private void removeListener() {
        if (listener != null) {
            listener.close();
            meetingService.removeListener(listener);
            listener = null;
        }
    }
}
