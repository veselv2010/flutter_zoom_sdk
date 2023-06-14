package com.evilratt.flutter_zoom_sdk.audio;

import android.util.Log;

import com.evilratt.flutter_zoom_sdk.base.SimpleInMeetingListener;

import io.flutter.plugin.common.EventChannel;
import kotlin.jvm.Volatile;
import us.zoom.sdk.ZoomSDK;

public class InMeetingAudioStateListener extends SimpleInMeetingListener {
    final Object arguments;
    final EventChannel.EventSink sink;

    public InMeetingAudioStateListener(Object arguments, EventChannel.EventSink sink) {
        this.arguments = arguments;
        this.sink = sink;
    }

    @Override
    public void onUserAudioStatusChanged(long var1, AudioStatus var3) {
        Log.i("Zoom Audio",  String.valueOf(var1) + " AudioStatus " + String.valueOf(var3.ordinal()) + " " + var3.name());
        if (var1 == ZoomSDK.getInstance().getInMeetingService().getMyUserID()) {
            sink.success(var3.ordinal());
        }
    }
}
