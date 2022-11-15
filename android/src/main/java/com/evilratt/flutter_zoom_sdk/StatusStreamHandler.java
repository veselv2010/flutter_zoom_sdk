package com.evilratt.flutter_zoom_sdk;

import java.util.Arrays;
import java.util.List;
import java.util.Objects;

import io.flutter.plugin.common.EventChannel;
import us.zoom.sdk.MeetingError;
import us.zoom.sdk.MeetingService;
import us.zoom.sdk.MeetingServiceListener;
import us.zoom.sdk.MeetingStatus;

/**
 * This class implements the handler for the Zoom meeting event in the flutter event channel
 */
public class StatusStreamHandler implements EventChannel.StreamHandler {
    private final MeetingService meetingService;
    private MeetingServiceListener statusListener;

    public StatusStreamHandler(MeetingService meetingService) {
        this.meetingService = meetingService;
    }

    @Override
    public void onListen(Object arguments, final EventChannel.EventSink events) {
        statusListener = new StatusListener(events);

        this.meetingService.addListener(statusListener);
    }

    @Override
    public void onCancel(Object arguments) {
        this.meetingService.removeListener(statusListener);

    }
}
