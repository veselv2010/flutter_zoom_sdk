package com.evilratt.flutter_zoom_sdk;

import io.flutter.plugin.common.EventChannel;
import us.zoom.sdk.MeetingService;

/**
 * This class implements the handler for the Zoom meeting event in the flutter event channel
 */
public class StatusStreamHandler implements EventChannel.StreamHandler {
    private MeetingService meetingService;
    private EventChannel.EventSink events;
    private StatusListener listener;

    public void setMeetingService(MeetingService service) {
        if (meetingService == service) {
            return;
        }
        removeListener();
        meetingService = service;
        addListener();
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
            listener = new StatusListener(events);
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
