package com.evilratt.flutter_zoom_sdk;

import static com.evilratt.flutter_zoom_sdk.utils.Utils.parseBoolean;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.evilratt.flutter_zoom_sdk.audio.FlutterSdkAudioHandler;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

import io.flutter.Log;
import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.embedding.engine.plugins.activity.ActivityAware;
import io.flutter.embedding.engine.plugins.activity.ActivityPluginBinding;
import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.Result;
import us.zoom.sdk.CustomizedNotificationData;
import us.zoom.sdk.InMeetingAudioController;
import us.zoom.sdk.InMeetingNotificationHandle;
import us.zoom.sdk.InMeetingService;
import us.zoom.sdk.JoinMeetingOptions;
import us.zoom.sdk.JoinMeetingParams;
import us.zoom.sdk.MeetingService;
import us.zoom.sdk.MeetingStatus;
import us.zoom.sdk.MeetingViewsOptions;
import us.zoom.sdk.SDKNotificationServiceError;
import us.zoom.sdk.StartMeetingOptions;
import us.zoom.sdk.StartMeetingParams4NormalUser;
import us.zoom.sdk.ZoomAuthenticationError;
import us.zoom.sdk.ZoomError;
import us.zoom.sdk.ZoomSDK;
import us.zoom.sdk.ZoomSDKAuthenticationListener;
import us.zoom.sdk.ZoomSDKInitParams;
import us.zoom.sdk.ZoomSDKInitializeListener;

/**
 * FlutterZoomPlugin
 */
public class FlutterZoomSdkPlugin extends Activity implements FlutterPlugin, MethodChannel.MethodCallHandler, ActivityAware {
    Activity activity;

    private static final String MAIN_CHANNEL = "flutter_zoom_sdk";
    private static final String MIC_CHANNEL = "flutter_zoom_sdk/microphone";
    private static final String MIC_CHANNEL_STREAM = "flutter_zoom_sdk/microphone/stream";
    private static final String STATUS_CHANNEL = "flutter_zoom_sdk_event_stream";
    private Result pendingResult;

    private MethodChannel methodChannel;
    private MethodChannel micChannel;
    private Context context;
    private EventChannel meetingStatusChannel;
    private EventChannel micStatusChannel;
    private InMeetingService inMeetingService;

    private static final int RECORD_AUDIO_PERMISSION_CODE = 100;

    private String returnBtnMsg;
    private FlutterSdkAudioHandler audioHandler;

    @Override
    public void onAttachedToActivity(@NonNull ActivityPluginBinding binding) {
        this.activity = binding.getActivity();
    }

    @Override
    public void onReattachedToActivityForConfigChanges(@NonNull ActivityPluginBinding binding) {
        this.activity = binding.getActivity();
    }

    @Override
    public void onAttachedToEngine(@NonNull FlutterPluginBinding flutterPluginBinding) {
        this.audioHandler = new FlutterSdkAudioHandler();

        context = flutterPluginBinding.getApplicationContext();

        methodChannel = new MethodChannel(flutterPluginBinding.getBinaryMessenger(), MAIN_CHANNEL);
        methodChannel.setMethodCallHandler(this);

        micChannel = new MethodChannel(flutterPluginBinding.getBinaryMessenger(), MIC_CHANNEL);
        micChannel.setMethodCallHandler(audioHandler);

        meetingStatusChannel = new EventChannel(flutterPluginBinding.getBinaryMessenger(), STATUS_CHANNEL);

        micStatusChannel = new EventChannel(flutterPluginBinding.getBinaryMessenger(), MIC_CHANNEL_STREAM);
    }

    @Override
    public void onMethodCall(@NonNull MethodCall methodCall, @NonNull final Result result) {
        switch (methodCall.method) {
            case "init":
                init(methodCall, result);
                break;
            case "login":
                login(methodCall, result);
                break;
            case "logout":
                logout();
                break;
            case "leave_meeting":
                leaveMeeting();
                break;
            case "join":
                joinMeeting(methodCall, result);
                break;
            case "start_normal":
                startMeetingNormal(methodCall, result);
                break;
            case "meeting_status":
                meetingStatus(result);
                break;
            case "meeting_details":
                meetingDetails(result);
                break;
            case "show_meeting":
                showMeeting(methodCall, result);
                break;
            case "hide_meeting":
                hideMeeting(methodCall, result);
                break;
            default:
                result.notImplemented();
        }
    }

    @Override
    public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
        methodChannel.setMethodCallHandler(null);
    }

//  public void showMeetingWindow(final Context context) {
//
//    ZoomSDK.getInstance().getInMeetingService().getInMeetingShareController().addListener(this);
//
//    refContext = new SoftReference<>(context);
//    if (mbAddedView) {
//      windowView.setVisibility(View.VISIBLE);
//      addVideoUnit();
//      return;
//    }
//
//    if (null == mWindowManager) {
//      mWindowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
//    }
//
//    if (null == windowView) {
//      windowView = LayoutInflater.from(context).inflate(R.layout.layout_meeting_window, null);
//      mobileRTCVideoView = (MobileRTCVideoView) windowView.findViewById(R.id.active_video_view);
//      renderInfo = new MobileRTCVideoUnitRenderInfo(0, 0, 100, 100);
//      renderInfo.is_border_visible = true;
//      gestureDetector = new GestureDetector(context, new SingleTapConfirm());
//      windowView.setOnTouchListener(onTouchListener);
//    }
//
//    mWindowManager.addView(windowView, getLayoutParams(context));
//    mbAddedView = true;
//    addVideoUnit();
//  }
//
//  public void hiddenMeetingWindow(boolean destroy) {
//    if (null == windowView || null == mWindowManager || null == mobileRTCVideoView) {
//      return;
//    }
//    mobileRTCVideoView.getVideoViewManager().removeAllVideoUnits();
//    if (!destroy) {
//      windowView.setVisibility(View.GONE);
//    } else {
//      try {
//        mWindowManager.removeView(windowView);
//      } catch (Exception e) {
//      }
//      mbAddedView = false;
//      windowView = null;
//      mobileRTCVideoView = null;
//    }
//  }

    private void sendReply(List data) {
        if (this.pendingResult == null) {
            return;
        }
        this.pendingResult.success(data);
        this.clearPendingResult();
    }

    private void clearPendingResult() {
        this.pendingResult = null;
    }

    // Initializing the Zoom SDK for Android
    private void init(final MethodCall methodCall, final Result result) {
        Map<String, String> options = methodCall.arguments();

        ZoomSDK zoomSDK = ZoomSDK.getInstance();

        if (zoomSDK.isInitialized()) {
            List<Integer> response = Arrays.asList(0, 0);
            result.success(response);
            return;
        }

        ZoomSDKInitParams initParams = new ZoomSDKInitParams();
        initParams.jwtToken = options.get("jwtToken");
        initParams.domain = options.get("domain");
        initParams.enableLog = true;

        returnBtnMsg = options.get("returnBtnMsg");

        final InMeetingNotificationHandle handle = (context, intent) -> {
            intent = new Intent(context, FlutterZoomSdkPlugin.class);
            intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
            if (context == null) {
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            }
            intent.setAction(InMeetingNotificationHandle.ACTION_RETURN_TO_CONF);
            assert context != null;
            context.startActivity(intent);

            return true;
        };

        // Set custom Notification fro android
        final CustomizedNotificationData data = new CustomizedNotificationData();
        data.setContentTitleId(R.string.app_name_zoom_local);
        data.setLargeIconId(R.drawable.zm_mm_type_emoji);
        data.setSmallIconId(R.drawable.zm_mm_type_emoji);
        data.setSmallIconForLorLaterId(R.drawable.zm_mm_type_emoji);

        ZoomSDKInitializeListener listener = new ZoomSDKInitializeListener() {
            /**
             * @param errorCode {@link us.zoom.sdk.ZoomError#ZOOM_ERROR_SUCCESS} if the SDK
             *                  has been initialized successfully.
             */
            @Override
            public void onZoomSDKInitializeResult(int errorCode, int internalErrorCode) {
                List<Integer> response = Arrays.asList(errorCode, internalErrorCode);

                if (errorCode != ZoomError.ZOOM_ERROR_SUCCESS) {
                    System.out.println("Failed to initialize Zoom SDK");
                    result.success(response);
                    return;
                }

                ZoomSDK zoomSDK = ZoomSDK.getInstance();
                zoomSDK.getMeetingSettingsHelper().enableShowMyMeetingElapseTime(true);
                zoomSDK.getMeetingSettingsHelper().setCustomizedNotificationData(data, handle);
                zoomSDK.getZoomUIService().enableMinimizeMeeting(true);
                zoomSDK.getZoomUIService().setNewMeetingUI(MyMeetingActivity.class);

                MeetingService meetingService = zoomSDK.getMeetingService();
                meetingStatusChannel.setStreamHandler(new StatusStreamHandler(meetingService));
                micStatusChannel.setStreamHandler(audioHandler);
                result.success(response);
            }

            @Override
            public void onZoomAuthIdentityExpired() {
            }
        };
        zoomSDK.initialize(context, listener, initParams);
    }

    // Perform start meeting function with logging in to the zoom account
    private void login(final MethodCall methodCall, final Result result) {
        this.pendingResult = result;
        Map<String, String> options = methodCall.arguments();

        ZoomSDK zoomSDK = getInitializedZoomInstance();

        if (zoomSDK == null) {
            result.success(Arrays.asList("SDK ERROR", "001"));
            return;
        }

        ZoomSDKAuthenticationListener authenticationListener = new ZoomSDKAuthenticationListener() {
            @Override
            public void onZoomSDKLoginResult(long results) {
                if (results != ZoomAuthenticationError.ZOOM_AUTH_ERROR_SUCCESS) {
                    sendReply(Arrays.asList("LOGIN ERROR", String.valueOf(results)));
                    return;
                }
                startMeeting(methodCall);
            }

            @Override
            public void onZoomSDKLogoutResult(long l) {

            }

            @Override
            public void onZoomIdentityExpired() {

            }

            @Override
            public void onZoomAuthIdentityExpired() {

            }

            @Override
            public void onNotificationServiceStatus(SDKNotificationServiceStatus sdkNotificationServiceStatus) {

            }

            @Override
            public void onNotificationServiceStatus(ZoomSDKAuthenticationListener.SDKNotificationServiceStatus status, SDKNotificationServiceError error) {

            }
        };

        // if(!zoomSDK.isLoggedIn()){
        // zoomSDK.loginWithZoom(options.get("userId"), options.get("userPassword"));
        // zoomSDK.addAuthenticationListener(authenticationListener);
        // }

        if (zoomSDK.isLoggedIn()) {
            startMeeting(methodCall);
        }

    }

    // Join Meeting with passed Meeting ID and Passcode
    private void joinMeeting(MethodCall methodCall, Result result) {
        ZoomSDK zoomSDK = getInitializedZoomInstance();
        Map<String, String> options = methodCall.arguments();

        if (zoomSDK == null) {
            result.success(false);
            return;
        }

        MeetingService meetingService = zoomSDK.getMeetingService();

        JoinMeetingOptions opts = new JoinMeetingOptions();
        opts.no_invite = parseBoolean(options, "disableInvite");
        opts.no_share = parseBoolean(options, "disableShare");
        opts.no_titlebar = parseBoolean(options, "disableTitlebar");
        opts.no_driving_mode = parseBoolean(options, "disableDrive");
        opts.no_dial_in_via_phone = parseBoolean(options, "disableDialIn");
        opts.no_disconnect_audio = parseBoolean(options, "noDisconnectAudio");
        opts.no_audio = parseBoolean(options, "noAudio");
        boolean view_options = parseBoolean(options, "viewOptions");
        if (view_options) {
            opts.meeting_views_options = MeetingViewsOptions.NO_TEXT_MEETING_ID + MeetingViewsOptions.NO_TEXT_PASSWORD;
        }

        JoinMeetingParams params = new JoinMeetingParams();

        params.displayName = options.get("displayName");
        params.meetingNo = options.get("meetingId");
        params.password = options.get("meetingPassword");

        meetingService.joinMeetingWithParams(context, params, opts);

        result.success(true);
    }

    // Basic Start Meeting Function called on startMeeting triggered via login
    // function
    private void startMeeting(MethodCall methodCall) {

        Map<String, String> options = methodCall.arguments();

        ZoomSDK zoomSDK = getInitializedZoomInstance();

        if (zoomSDK == null) {
            return;
        }

        if (!zoomSDK.isLoggedIn()) {
            System.out.println("Not LoggedIn!!!!!!");
            sendReply(Arrays.asList("LOGIN REQUIRED", "001"));
            return;
        }

        MeetingService meetingService = zoomSDK.getMeetingService();
        StartMeetingOptions opts = new StartMeetingOptions();
        opts.no_invite = parseBoolean(options, "disableInvite");
        opts.no_share = parseBoolean(options, "disableShare");
        opts.no_driving_mode = parseBoolean(options, "disableDrive");
        opts.no_dial_in_via_phone = parseBoolean(options, "disableDialIn");
        opts.no_disconnect_audio = parseBoolean(options, "noDisconnectAudio");
        opts.no_audio = parseBoolean(options, "noAudio");
        opts.no_titlebar = parseBoolean(options, "disableTitlebar");
        boolean view_options = parseBoolean(options, "viewOptions");
        if (view_options) {
            opts.meeting_views_options = MeetingViewsOptions.NO_TEXT_MEETING_ID + MeetingViewsOptions.NO_TEXT_PASSWORD;
        }

        meetingService.startInstantMeeting(context, opts);
        inMeetingService = zoomSDK.getInMeetingService();
        sendReply(Arrays.asList("MEETING SUCCESS", "200"));
    }

    // Perform start meeting function with logging in to the zoom account (Only for
    // passed meeting id)
    private void startMeetingNormal(final MethodCall methodCall, final Result result) {
        this.pendingResult = result;
        Map<String, String> options = methodCall.arguments();

        ZoomSDK zoomSDK = getInitializedZoomInstance();

        if (zoomSDK == null) {
            return;
        }

        if (zoomSDK.isLoggedIn()) {
            startMeetingNormalInternal(methodCall);
        }

        ZoomSDKAuthenticationListener authenticationListener = new ZoomSDKAuthenticationListener() {

            @Override
            public void onZoomSDKLoginResult(long results) {
                // Log.d("Zoom Flutter", String.format("[onLoginError] : %s", results));
                if (results != ZoomAuthenticationError.ZOOM_AUTH_ERROR_SUCCESS) {
                    sendReply(Arrays.asList("LOGIN ERROR", String.valueOf(results)));
                    return;
                }
                startMeetingNormalInternal(methodCall);
            }

            @Override
            public void onZoomSDKLogoutResult(long l) {

            }

            @Override
            public void onZoomIdentityExpired() {

            }

            @Override
            public void onZoomAuthIdentityExpired() {

            }

            @Override
            public void onNotificationServiceStatus(SDKNotificationServiceStatus sdkNotificationServiceStatus) {

            }

            @Override
            public void onNotificationServiceStatus(ZoomSDKAuthenticationListener.SDKNotificationServiceStatus status, SDKNotificationServiceError error) {

            }
        };

        // if(!zoomSDK.isLoggedIn()){
        // zoomSDK.loginWithZoom(options.get("userId"), options.get("userPassword"));
        // zoomSDK.addAuthenticationListener(authenticationListener);
        // }
    }

    // Meeting ID passed Start Meeting Function called on startMeetingNormal
    // triggered via startMeetingNormal function
    private void startMeetingNormalInternal(MethodCall methodCall) {
        Map<String, String> options = methodCall.arguments();

        ZoomSDK zoomSDK = getInitializedZoomInstance();

        if (zoomSDK == null) {
            return;
        }

        if (zoomSDK.isLoggedIn()) {
            MeetingService meetingService = zoomSDK.getMeetingService();
            StartMeetingOptions opts = new StartMeetingOptions();
            opts.no_invite = parseBoolean(options, "disableInvite");
            opts.no_share = parseBoolean(options, "disableShare");
            opts.no_driving_mode = parseBoolean(options, "disableDrive");
            opts.no_dial_in_via_phone = parseBoolean(options, "disableDialIn");
            opts.no_disconnect_audio = parseBoolean(options, "noDisconnectAudio");
            opts.no_audio = parseBoolean(options, "noAudio");
            opts.no_titlebar = parseBoolean(options, "disableTitlebar");
            boolean view_options = parseBoolean(options, "viewOptions");
            if (view_options) {
                opts.meeting_views_options = MeetingViewsOptions.NO_TEXT_MEETING_ID + MeetingViewsOptions.NO_TEXT_PASSWORD;
            }

            StartMeetingParams4NormalUser params = new StartMeetingParams4NormalUser();
            params.meetingNo = options.get("meetingId");

            meetingService.startMeetingWithParams(context, params, opts);
            inMeetingService = zoomSDK.getInMeetingService();
            sendReply(Arrays.asList("MEETING SUCCESS", "200"));
        }
    }

    // Get Meeting Details Programmatically after Starting the Meeting
    private void meetingDetails(Result result) {
        ZoomSDK zoomSDK = getInitializedZoomInstance();

        if (zoomSDK == null) {
            result.success(Arrays.asList("MEETING_STATUS_UNKNOWN", "SDK not initialized"));
            return;
        }
        MeetingService meetingService = zoomSDK.getMeetingService();

        if (meetingService == null) {
            result.success(Arrays.asList("MEETING_STATUS_UNKNOWN", "No status available"));
            return;
        }
        MeetingStatus status = meetingService.getMeetingStatus();

        result.success(status != null ? Arrays.asList(inMeetingService.getCurrentMeetingNumber(), inMeetingService.getMeetingPassword()) : Arrays.asList("MEETING_STATUS_UNKNOWN", "No status available"));
    }

    // Listen to meeting status on joinning and starting the mmeting
    private void meetingStatus(Result result) {

        ZoomSDK zoomSDK = getInitializedZoomInstance();

        if (zoomSDK == null) {
            result.success(Arrays.asList("MEETING_STATUS_UNKNOWN", "SDK not initialized"));
            return;
        }

        MeetingService meetingService = zoomSDK.getMeetingService();

        if (meetingService == null) {
            result.success(Arrays.asList("MEETING_STATUS_UNKNOWN", "No status available"));
            return;
        }

        MeetingStatus status = meetingService.getMeetingStatus();
        result.success(status != null ? Arrays.asList(status.name(), "") : Arrays.asList("MEETING_STATUS_UNKNOWN", "No status available"));

        if (status == MeetingStatus.MEETING_STATUS_INMEETING) {
            checkAudioPermission();
        }
    }

    public void logout() {
        ZoomSDK zoomSDK = ZoomSDK.getInstance();
        zoomSDK.logoutZoom();
    }

    public void leaveMeeting() {
        ZoomSDK zoomSDK = ZoomSDK.getInstance();
        zoomSDK.getInMeetingService().leaveCurrentMeeting(true);
//        this.activity.finish();
        ZoomSDK.getInstance().uninitialize();
    }

    @Nullable
    private ZoomSDK getInitializedZoomInstance() {
        ZoomSDK zoomSDK = ZoomSDK.getInstance();

        if (!zoomSDK.isInitialized()) {
            Log.e("Incorrect usage of zoom sdk", "Zoom is not initialized");
            sendReply(Arrays.asList("SDK ERROR", "001"));
            return null;
        }

        return zoomSDK;
    }

    @Override
    public void onDetachedFromActivityForConfigChanges() {
        this.activity = null;
    }

    @Override
    public void onDetachedFromActivity() {
        this.activity = null;
    }

    private void showMeeting(MethodCall methodCall, Result result) {
        Intent myIntent = new Intent(this.activity, MyMeetingActivity.class);

        if (returnBtnMsg != null) {
            myIntent.putExtra("returnBtnMsg", returnBtnMsg.toString());
        }

        this.activity.startActivity(myIntent);

        ZoomSDK zoomSDK = getInitializedZoomInstance();

        if (zoomSDK == null) {
            result.success(Arrays.asList("SDK ERROR", "001"));
            return;
        }

        zoomSDK.getZoomUIService().hideMiniMeetingWindow();

        result.success(true);
    }

    private void hideMeeting(MethodCall methodCall, Result result) {
        Intent myIntent = new Intent("my_meeting_activity_broadcast");
        myIntent.putExtra("broadcast", "finishMyMeetingActivity");

        this.activity.sendBroadcast(myIntent);

        result.success(true);
    }

    public void connectAudioInMeeting() {
        InMeetingAudioController controller = ZoomSDK.getInstance().getInMeetingService().getInMeetingAudioController();

        if (!controller.isAudioConnected()) {
            controller.connectAudioWithVoIP();
        }
    }

    public void checkAudioPermission() {
        String permission = Manifest.permission.RECORD_AUDIO;
        int checkResult = this.activity.checkSelfPermission(permission);

        if (checkResult == PackageManager.PERMISSION_DENIED) {
            this.activity.requestPermissions(new String[]{permission}, RECORD_AUDIO_PERMISSION_CODE);
        } else if (checkResult == PackageManager.PERMISSION_GRANTED) {
            connectAudioInMeeting();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode == RECORD_AUDIO_PERMISSION_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                connectAudioInMeeting();
            }
        }
    }
}
