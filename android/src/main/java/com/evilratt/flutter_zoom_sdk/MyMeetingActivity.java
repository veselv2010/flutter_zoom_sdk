package com.evilratt.flutter_zoom_sdk;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

import us.zoom.sdk.NewMeetingActivity;
import us.zoom.sdk.ZoomSDK;

public class MyMeetingActivity extends NewMeetingActivity {
    @Override
    protected int getLayout() {
        return R.layout.my_new_meeting_layout;
    }

    @Override
    protected int getLayoutForTablet() {
        return R.layout.my_new_meeting_tablet_layout;
    }

    @Override
    protected boolean isSensorOrientationEnabled() {
        return false;
    }

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
    protected void onStop() {
        super.onStop();

        ZoomSDK.getInstance().getZoomUIService().showMiniMeetingWindow();
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
    public void onStart() {
        super.onStart();
    }


    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        if(keyCode== KeyEvent.KEYCODE_BACK)   {
            finish();
        }

        return true;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        unregisterReceiver(myBroadcast);
    }
}
