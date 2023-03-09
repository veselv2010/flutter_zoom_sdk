package com.evilratt.flutter_zoom_sdk;

import us.zoom.sdk.MeetingActivity;
import us.zoom.sdk.ZoomSDK;

import android.Manifest;
import android.content.Intent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MyMeetingActivity extends MeetingActivity {
    private Button returnBtn;

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
    protected int getLayout() {
        return R.layout.my_meeting_layout;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (!ZoomSDK.getInstance().isInitialized()) {
            finish();
            return;
        }

        Intent myIntent = getIntent();
        String returnBtnMsg = myIntent.getStringExtra("returnBtnMsg");

        registerReceiver(myBroadcast, new IntentFilter("my_meeting_activity_broadcast"));

        returnBtn = (Button)findViewById(R.id.returnBtn);

        if (returnBtn != null) {
            if (returnBtnMsg != null) {
                returnBtn.setText(returnBtnMsg);
            }

            returnBtn.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                    finish();
                }
            });
        }
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
