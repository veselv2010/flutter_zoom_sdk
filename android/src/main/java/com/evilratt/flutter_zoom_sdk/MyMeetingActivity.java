package com.evilratt.flutter_zoom_sdk;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.view.KeyEvent;
import android.widget.Button;

import androidx.core.content.ContextCompat;

import us.zoom.sdk.NewMeetingActivity;
import us.zoom.sdk.ZoomSDK;

public class MyMeetingActivity extends NewMeetingActivity {
    static final String ACTION_HIDE_MEETING = "com.evilratt.flutter_zoom_sdk.HIDE_MEETING";
    private static String returnButtonText;
    private boolean receiverRegistered;

    private final BroadcastReceiver hideMeetingReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (ACTION_HIDE_MEETING.equals(intent.getAction())) {
                finish();
            }
        }
    };

    static void setReturnButtonText(String text) {
        returnButtonText = text;
    }

    @Override
    protected int getLayoutForMutiltask() {
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
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (!ZoomSDK.getInstance().isInitialized()) {
            finish();
            return;
        }

        ContextCompat.registerReceiver(this, hideMeetingReceiver,
                new IntentFilter(ACTION_HIDE_MEETING), ContextCompat.RECEIVER_NOT_EXPORTED);
        receiverRegistered = true;

        Button returnBtn = findViewById(R.id.returnBtn);
        if (returnBtn != null) {
            if (returnButtonText != null) {
                returnBtn.setText(returnButtonText);
            }
            returnBtn.setOnClickListener(view -> finish());
        }
    }

    @Override
    protected void onStop() {
        super.onStop();

        ZoomSDK sdk = ZoomSDK.getInstance();
        if (sdk.isInitialized() && !isChangingConfigurations() && !isInPictureInPictureMode()) {
            sdk.getZoomUIService().showMiniMeetingWindow();
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            finish();
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    protected void onDestroy() {
        if (receiverRegistered) {
            unregisterReceiver(hideMeetingReceiver);
            receiverRegistered = false;
        }
        super.onDestroy();
    }
}
