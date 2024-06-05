package com.evilratt.flutter_zoom_sdk.base;

import us.zoom.sdk.BOOption;
import us.zoom.sdk.BOStatus;
import us.zoom.sdk.IBOAdmin;
import us.zoom.sdk.IBOAssistant;
import us.zoom.sdk.IBOAttendee;
import us.zoom.sdk.IBOCreator;
import us.zoom.sdk.IBOData;
import us.zoom.sdk.IShareAction;
import us.zoom.sdk.InMeetingBOControllerListener;
import us.zoom.sdk.ReturnToMainSessionHandler;
import us.zoom.sdk.SharingStatus;

public abstract class SimpleInMeetingBOControllerListener implements InMeetingBOControllerListener {
    @Override
    public void onBOOptionChanged(BOOption newOption) {

    }

    @Override
    public void onBroadcastBOVoiceStatus(boolean start) {

    }

    @Override
    public void onHasCreatorRightsNotification(IBOCreator iboCreator) {

    }

    @Override
    public void onHasAdminRightsNotification(IBOAdmin iboAdmin) {

    }

    @Override
    public void onHasAssistantRightsNotification(IBOAssistant iboAssistant) {

    }

    @Override
    public void onHasAttendeeRightsNotification(IBOAttendee iboAttendee) {

    }

    @Override
    public void onHasDataHelperRightsNotification(IBOData iboData) {

    }

    @Override
    public void onLostCreatorRightsNotification() {

    }

    @Override
    public void onLostAdminRightsNotification() {

    }

    @Override
    public void onLostAssistantRightsNotification() {

    }

    @Override
    public void onLostAttendeeRightsNotification() {

    }

    @Override
    public void onLostDataHelperRightsNotification() {

    }

    @Override
    public void onNewBroadcastMessageReceived(String message,long senderId,String senderName) {

    }

    @Override
    public void onBOStopCountDown(int seconds) {

    }

    @Override
    public void onHostInviteReturnToMainSession(String name, ReturnToMainSessionHandler handler) {

    }

    @Override
    public void onBOStatusChanged(BOStatus status) {

    }

    @Override
    public void onBOSwitchRequestReceived(String strNewBOName, String strNewBOID) {

    }

    @Override
    public void onShareFromMainSession(long l, SharingStatus sharingStatus, IShareAction iShareAction) {

    }
}
