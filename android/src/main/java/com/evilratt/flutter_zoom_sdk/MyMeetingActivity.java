package com.evilratt.flutter_zoom_sdk;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.annotation.SuppressLint;

import com.evilratt.flutter_zoom_sdk.base.SimpleInMeetingBOControllerListener;

import us.zoom.sdk.BOStatus;
import us.zoom.sdk.NewMeetingActivity;
import us.zoom.sdk.ReturnToMainSessionHandler;
import us.zoom.sdk.ZoomSDK;

public class MyMeetingActivity extends NewMeetingActivity {
    SimpleInMeetingBOControllerListener mBOControllerListener = new SimpleInMeetingBOControllerListener() {
        @Override
        public void onBOStatusChanged(BOStatus boStatus) {
            showMeetingActivity();

            super.onBOStatusChanged(boStatus);
        }

        @Override
        public void onHostInviteReturnToMainSession(String name, ReturnToMainSessionHandler handler) {
            handler.returnToMainSession();

            super.onHostInviteReturnToMainSession(name, handler);
        }
    };

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
    protected int getLayoutForTablet() {
        return R.layout.my_meeting_tablet_layout;
    }

    @Override
    protected boolean isSensorOrientationEnabled() {
        return false;
    }

    @Override
    protected void onStop() {
        super.onStop();

        ZoomSDK.getInstance().getZoomUIService().showMiniMeetingWindow();
    }

    @SuppressLint("UnspecifiedRegisterReceiverFlag")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ZoomSDK zoomSDK = ZoomSDK.getInstance();

        if (!zoomSDK.isInitialized()) {
            finish();
            return;
        }

        Intent myIntent = getIntent();
        String returnBtnMsg = myIntent.getStringExtra("returnBtnMsg");

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            registerReceiver(myBroadcast, new IntentFilter("my_meeting_activity_broadcast"), RECEIVER_NOT_EXPORTED);
        } else {
            registerReceiver(myBroadcast, new IntentFilter("my_meeting_activity_broadcast"));
        }

        Button returnBtn = (Button) findViewById(R.id.returnBtn);

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
    protected void onStart() {
        super.onStart();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            finish();
        }

        return true;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        unregisterReceiver(myBroadcast);
    }

    public void showMeetingActivity() {
        Intent myIntent = getIntent();

        this.startActivity(myIntent);

        ZoomSDK zoomSDK = ZoomSDK.getInstance();

        if (zoomSDK.isInitialized()) {
            zoomSDK.getZoomUIService().hideMiniMeetingWindow();
        }
    }
}
