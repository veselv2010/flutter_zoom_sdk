package com.evilratt.flutter_zoom_sdk.audio;

import android.os.Handler;
import android.os.Looper;

import com.evilratt.flutter_zoom_sdk.base.SimpleInMeetingListener;

import io.flutter.plugin.common.EventChannel;
import us.zoom.sdk.InMeetingService;

public class InMeetingAudioStateListener extends SimpleInMeetingListener {
    private final Handler mainHandler = new Handler(Looper.getMainLooper());
    private final InMeetingService meetingService;
    private EventChannel.EventSink events;

    public InMeetingAudioStateListener(InMeetingService meetingService, EventChannel.EventSink events) {
        this.meetingService = meetingService;
        this.events = events;
    }

    public void close() {
        events = null;
    }

    @Override
    public void onUserAudioStatusChanged(long userId, AudioStatus status) {
        mainHandler.post(() -> {
            if (events != null && status != null && userId == meetingService.getMyUserID()) {
                events.success(status.ordinal());
            }
        });
    }
}
