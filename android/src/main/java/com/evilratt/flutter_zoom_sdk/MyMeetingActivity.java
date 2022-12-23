package com.evilratt.flutter_zoom_sdk;

import android.content.Intent;
import us.zoom.sdk.MeetingActivity;

public class MyMeetingActivity extends MeetingActivity{
    @Override
    public void onStart() {
        Intent myIntent = getIntent();
        boolean isClose = myIntent.getBooleanExtra("isClose", true);

        super.onStart();

        if(isClose) {
            finish();
        }
    }

    @Override
    public void onBackPressed() {
        finish();
    }
}
