package com.evilratt.flutter_zoom_sdk;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import us.zoom.sdk.MeetingActivity;

public class MyMeetingActivity extends MeetingActivity {
    BroadcastReceiver myBroadcast = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String extra = intent.getStringExtra("broadcast");

            if (extra != null) {
                if (extra.equalsIgnoreCase("finishMyMeetingActivity")) {
                    finish();
                }
            }
        }
    };

    @Override
    public void onCreate(android.os.Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        registerReceiver(myBroadcast, new IntentFilter("my_meeting_activity_broadcast"));
    }

    @Override
    public void onStart() {
        Intent myIntent = getIntent();
        boolean isClose = myIntent.getBooleanExtra("isClose", true);

        super.onStart();

        if (isClose) {
            finish();
        }
    }

    @Override
    public void onBackPressed() {
        finish();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        unregisterReceiver(myBroadcast);
    }
}
