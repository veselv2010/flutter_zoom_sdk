package com.evilratt.flutter_zoom_sdk.audio;

import static com.evilratt.flutter_zoom_sdk.utils.Utils.parseBoolean;

import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import us.zoom.sdk.MobileRTCSDKError;
import us.zoom.sdk.ZoomSDK;

public class FlutterSdkAudioHandler implements MethodChannel.MethodCallHandler, EventChannel.StreamHandler {
    @Nullable
    InMeetingAudioStateListener listener;

    @Override
    public void onMethodCall(@NonNull MethodCall call, @NonNull MethodChannel.Result result) {
        switch (call.method) {
            case "getMicStatus":
                final boolean status = getMicStatus();
                result.success(status);
                break;
            case "muteMyAudio":
                if (call.arguments() == null) break;
                final boolean value = (boolean) call.arguments();
                final @Nullable String error = changeMicStatus(value);

                if (error != null) {
                    Log.e("muteMyAudio", error);
//                    result.error("-1", "Incorrect usage of zoom sdk", error);
                    break;
                }

                result.success(value);
                break;
            default:
                break;
        }
    }

    private @NonNull boolean getMicStatus() {
        return ZoomSDK.getInstance().getInMeetingService().getInMeetingAudioController().isMyAudioMuted();
    }

    private @Nullable String changeMicStatus(boolean state) {
        final MobileRTCSDKError error = ZoomSDK.getInstance().getInMeetingService().getInMeetingAudioController().muteMyAudio(state);

        if (error != null && error.name() != "SDKERR_SUCCESS") {
            return error.name();
        }

        return null;
    }

    @Override
    public void onListen(Object arguments, EventChannel.EventSink events) {
        final long myId = ZoomSDK.getInstance().getInMeetingService().getMyUserID();
        Log.i("Zoom Audio", String.valueOf(myId) + " MicStream myId");
        if (listener == null) {
            listener = new InMeetingAudioStateListener(arguments, events);
        }

        ZoomSDK.getInstance().getInMeetingService().addListener(listener);
    }

    @Override
    public void onCancel(Object arguments) {
        if (listener == null) return;
        listener.sink.endOfStream();
        ZoomSDK.getInstance().getInMeetingService().removeListener(listener);
    }
}
