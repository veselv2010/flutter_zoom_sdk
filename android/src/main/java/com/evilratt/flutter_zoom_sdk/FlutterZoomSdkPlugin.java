package com.evilratt.flutter_zoom_sdk;

import static com.evilratt.flutter_zoom_sdk.utils.Utils.parseBoolean;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.evilratt.flutter_zoom_sdk.audio.FlutterSdkAudioHandler;

import java.util.Arrays;
import java.util.Map;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.embedding.engine.plugins.activity.ActivityAware;
import io.flutter.embedding.engine.plugins.activity.ActivityPluginBinding;
import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry;
import us.zoom.sdk.CustomizedNotificationData;
import us.zoom.sdk.InMeetingAudioController;
import us.zoom.sdk.InMeetingNotificationHandle;
import us.zoom.sdk.InMeetingService;
import us.zoom.sdk.JoinMeetingOptions;
import us.zoom.sdk.JoinMeetingParams;
import us.zoom.sdk.MeetingError;
import us.zoom.sdk.MeetingOptions;
import us.zoom.sdk.MeetingService;
import us.zoom.sdk.MeetingStatus;
import us.zoom.sdk.MeetingViewsOptions;
import us.zoom.sdk.StartMeetingOptions;
import us.zoom.sdk.StartMeetingParams4NormalUser;
import us.zoom.sdk.ZoomError;
import us.zoom.sdk.ZoomSDK;
import us.zoom.sdk.ZoomSDKInitParams;
import us.zoom.sdk.ZoomSDKInitializeListener;

public class FlutterZoomSdkPlugin implements FlutterPlugin, MethodChannel.MethodCallHandler, ActivityAware {
    private static final String TAG = "FlutterZoomSdkPlugin";
    private static final String MAIN_CHANNEL = "flutter_zoom_sdk";
    private static final String MIC_CHANNEL = "flutter_zoom_sdk/microphone";
    private static final String MIC_CHANNEL_STREAM = "flutter_zoom_sdk/microphone/stream";
    private static final String STATUS_CHANNEL = "flutter_zoom_sdk_event_stream";
    private static final int RECORD_AUDIO_PERMISSION_CODE = 100;
    private static final int MEETING_IDLE_RETRY_DELAY_MS = 250;
    private static final int MEETING_IDLE_MAX_RETRIES = 40;

    private final Handler mainHandler = new Handler(Looper.getMainLooper());
    private Activity activity;
    private ActivityPluginBinding activityBinding;
    private Context context;
    private MethodChannel methodChannel;
    private MethodChannel micChannel;
    private EventChannel meetingStatusChannel;
    private EventChannel micStatusChannel;
    private StatusStreamHandler statusHandler;
    private FlutterSdkAudioHandler audioHandler;
    private ZoomSDKInitializeListener initListener;
    private Result pendingInitResult;
    private boolean leaveRequested;
    private boolean autoAudioJoinEnabled;
    private boolean audioPermissionRequested;
    private Result pendingJoinResult;
    private Map<String, String> pendingJoinOptions;
    private final PluginRegistry.RequestPermissionsResultListener audioPermissionListener =
            (requestCode, permissions, grantResults) -> {
                if (requestCode != RECORD_AUDIO_PERMISSION_CODE || pendingJoinResult == null) {
                    return false;
                }
                Result result = pendingJoinResult;
                Map<String, String> options = pendingJoinOptions;
                pendingJoinResult = null;
                pendingJoinOptions = null;
                joinMeetingWithOptions(options, result);
                return true;
            };

    @Override
    public void onAttachedToActivity(@NonNull ActivityPluginBinding binding) {
        activity = binding.getActivity();
        activityBinding = binding;
        binding.addRequestPermissionsResultListener(audioPermissionListener);
    }

    @Override
    public void onReattachedToActivityForConfigChanges(@NonNull ActivityPluginBinding binding) {
        onAttachedToActivity(binding);
    }

    @Override
    public void onDetachedFromActivityForConfigChanges() {
        onDetachedFromActivity();
    }

    @Override
    public void onDetachedFromActivity() {
        if (activityBinding != null) {
            activityBinding.removeRequestPermissionsResultListener(audioPermissionListener);
            activityBinding = null;
        }
        cancelPendingJoin();
        activity = null;
    }

    @Override
    public void onAttachedToEngine(@NonNull FlutterPluginBinding binding) {
        context = binding.getApplicationContext();
        statusHandler = new StatusStreamHandler();
        audioHandler = new FlutterSdkAudioHandler();

        methodChannel = new MethodChannel(binding.getBinaryMessenger(), MAIN_CHANNEL);
        methodChannel.setMethodCallHandler(this);
        micChannel = new MethodChannel(binding.getBinaryMessenger(), MIC_CHANNEL);
        micChannel.setMethodCallHandler(audioHandler);

        meetingStatusChannel = new EventChannel(binding.getBinaryMessenger(), STATUS_CHANNEL);
        meetingStatusChannel.setStreamHandler(statusHandler);
        micStatusChannel = new EventChannel(binding.getBinaryMessenger(), MIC_CHANNEL_STREAM);
        micStatusChannel.setStreamHandler(audioHandler);
    }

    @Override
    public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
        methodChannel.setMethodCallHandler(null);
        micChannel.setMethodCallHandler(null);
        meetingStatusChannel.setStreamHandler(null);
        micStatusChannel.setStreamHandler(null);
        statusHandler.onCancel(null);
        audioHandler.onCancel(null);
        statusHandler.setMeetingService(null);
        audioHandler.setMeetingService(null);

        cancelPendingJoin();
        initListener = null;
        leaveRequested = false;
        autoAudioJoinEnabled = false;
        if (pendingInitResult != null) {
            pendingInitResult.error("ENGINE_DETACHED", "Flutter engine detached", null);
            pendingInitResult = null;
        }
        activity = null;
        context = null;
    }

    @Override
    public void onMethodCall(@NonNull MethodCall call, @NonNull Result result) {
        switch (call.method) {
            case "init":
                init(call, result);
                break;
            case "login":
                startMeeting(call, result, false);
                break;
            case "start_normal":
                startMeeting(call, result, true);
                break;
            case "logout":
                result.success(ZoomSDK.getInstance().isInitialized() && ZoomSDK.getInstance().logoutZoom());
                break;
            case "join":
                joinMeeting(call, result);
                break;
            case "leave_meeting":
                leaveMeeting(result);
                break;
            case "meeting_status":
                meetingStatus(result);
                break;
            case "meeting_details":
                meetingDetails(result);
                break;
            case "show_meeting":
                showMeeting(result);
                break;
            case "hide_meeting":
                hideMeeting(result);
                break;
            default:
                result.notImplemented();
        }
    }

    private void init(MethodCall call, Result result) {
        Map<String, String> options = call.arguments();
        if (options == null || options.get("jwtToken") == null || options.get("jwtToken").isEmpty()
                || pendingInitResult != null) {
            result.success(Arrays.asList(ZoomError.ZOOM_ERROR_INVALID_ARGUMENTS, 0));
            return;
        }

        ZoomSDK sdk = ZoomSDK.getInstance();
        MeetingService meetingService = sdk.isInitialized() ? sdk.getMeetingService() : null;
        MeetingStatus status = meetingService != null ? meetingService.getMeetingStatus() : MeetingStatus.MEETING_STATUS_IDLE;
        if (isMeetingActive(status) && !leaveRequested) {
            Log.w(TAG, "Cannot initialize Zoom during a meeting");
            result.success(Arrays.asList(ZoomError.ZOOM_ERROR_INVALID_ARGUMENTS, 0));
            return;
        }

        initListener = null;
        pendingInitResult = result;
        initializeWhenIdle(options, result, MEETING_IDLE_MAX_RETRIES);
    }

    private void initializeWhenIdle(Map<String, String> options, Result result, int retries) {
        if (pendingInitResult != result) {
            return;
        }
        ZoomSDK sdk = ZoomSDK.getInstance();
        MeetingService service = getMeetingService();
        if (service != null && service.getMeetingStatus() != MeetingStatus.MEETING_STATUS_IDLE) {
            if (retries == 0) {
                pendingInitResult = null;
                Log.w(TAG, "Previous Zoom meeting did not finish");
                result.success(Arrays.asList(ZoomError.ZOOM_ERROR_UNKNOWN, 0));
                return;
            }
            // Classroom can request a new meeting before the previous leave has completed.
            mainHandler.postDelayed(() -> initializeWhenIdle(options, result, retries - 1), MEETING_IDLE_RETRY_DELAY_MS);
            return;
        }

        leaveRequested = false;
        autoAudioJoinEnabled = false;
        ZoomSDKInitParams params = new ZoomSDKInitParams();
        params.jwtToken = options.get("jwtToken");
        params.domain = options.get("domain");
        params.enableLog = parseBoolean(options, "debug");
        MyMeetingActivity.setReturnButtonText(options.get("returnBtnMsg"));

        initListener = createInitListener();
        // Each classroom supplies a fresh JWT. Do not reinitialize during a meeting.
        try {
            statusHandler.setMeetingService(null);
            audioHandler.setMeetingService(null);
            sdk.initialize(context, initListener, params);
        } catch (RuntimeException e) {
            pendingInitResult = null;
            Log.w(TAG, "Zoom initialization could not start", e);
            result.success(Arrays.asList(ZoomError.ZOOM_ERROR_UNKNOWN, 0));
        }
    }

    private ZoomSDKInitializeListener createInitListener() {
        return new ZoomSDKInitializeListener() {
            @Override
            public void onZoomSDKInitializeResult(int errorCode, int internalErrorCode) {
                mainHandler.post(() -> {
                    if (initListener != this || pendingInitResult == null) {
                        return;
                    }
                    Result reply = pendingInitResult;
                    pendingInitResult = null;
                    if (errorCode == ZoomError.ZOOM_ERROR_SUCCESS) {
                        try {
                            configureSdk(ZoomSDK.getInstance());
                        } catch (RuntimeException e) {
                            Log.w(TAG, "Zoom SDK configuration failed", e);
                            reply.success(Arrays.asList(ZoomError.ZOOM_ERROR_UNKNOWN, 0));
                            return;
                        }
                    } else {
                        Log.w(TAG, "Zoom initialization failed: " + errorCode + ", " + internalErrorCode);
                    }
                    reply.success(Arrays.asList(errorCode, internalErrorCode));
                });
            }

            @Override
            public void onZoomAuthIdentityExpired() {
                mainHandler.post(() -> {
                    if (initListener == this) {
                        Log.w(TAG, "Zoom SDK authorization expired");
                    }
                });
            }
        };
    }

    private void configureSdk(ZoomSDK sdk) {
        CustomizedNotificationData data = new CustomizedNotificationData();
        data.setContentTitleId(R.string.app_name_zoom_local);
        sdk.getMeetingSettingsHelper().setCustomizedNotificationData(data, (notificationContext, intent) -> {
            if (notificationContext == null) {
                return false;
            }
            Intent meetingIntent = new Intent(notificationContext, MyMeetingActivity.class);
            meetingIntent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
            if (!(notificationContext instanceof Activity)) {
                meetingIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            }
            meetingIntent.setAction(InMeetingNotificationHandle.ACTION_RETURN_TO_CONF);
            notificationContext.startActivity(meetingIntent);
            return true;
        });
        sdk.getMeetingSettingsHelper().enableShowMyMeetingElapseTime(true);
        sdk.getZoomUIService().enableMinimizeMeeting(true);
        sdk.getZoomUIService().setNewMeetingUI(MyMeetingActivity.class);
        statusHandler.setMeetingService(sdk.getMeetingService());
        audioHandler.setMeetingService(sdk.getInMeetingService());
    }

    private void joinMeeting(MethodCall call, Result result) {
        Map<String, String> options = call.arguments();
        MeetingService service = getMeetingService();
        if (pendingJoinResult != null || pendingInitResult != null || activity == null || options == null
                || service == null || service.getMeetingStatus() != MeetingStatus.MEETING_STATUS_IDLE) {
            result.success(false);
            return;
        }

        boolean autoAudioJoin = !options.containsKey("autoAudioJoin") || parseBoolean(options, "autoAudioJoin");
        autoAudioJoinEnabled = autoAudioJoin && !parseBoolean(options, "noAudio");
        audioPermissionRequested = false;
        if (autoAudioJoinEnabled && activity.checkSelfPermission(Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED) {
            pendingJoinResult = result;
            pendingJoinOptions = options;
            audioPermissionRequested = true;
            activity.requestPermissions(new String[]{Manifest.permission.RECORD_AUDIO}, RECORD_AUDIO_PERMISSION_CODE);
            return;
        }
        joinMeetingWithOptions(options, result);
    }

    private void joinMeetingWithOptions(Map<String, String> options, Result result) {
        MeetingService service = getMeetingService();
        if (activity == null || service == null || service.getMeetingStatus() != MeetingStatus.MEETING_STATUS_IDLE) {
            autoAudioJoinEnabled = false;
            result.success(false);
            return;
        }

        JoinMeetingOptions opts = new JoinMeetingOptions();
        configureMeetingOptions(opts, options);
        opts.no_audio = parseBoolean(options, "noAudio");

        JoinMeetingParams params = new JoinMeetingParams();
        params.displayName = options.get("displayName");
        params.meetingNo = options.get("meetingId");
        params.password = options.get("meetingPassword");

        // Join classroom audio with the microphone enabled by default.
        ZoomSDK.getInstance().getMeetingSettingsHelper().setAutoConnectVoIPWhenJoinMeeting(autoAudioJoinEnabled);
        ZoomSDK.getInstance().getMeetingSettingsHelper().setMuteMyMicrophoneWhenJoinMeeting(false);
        int error = service.joinMeetingWithParams(activity, params, opts);
        if (error != MeetingError.MEETING_ERROR_SUCCESS) {
            autoAudioJoinEnabled = false;
            Log.w(TAG, "Zoom join failed: " + error);
        }
        result.success(error == MeetingError.MEETING_ERROR_SUCCESS);
    }

    private void cancelPendingJoin() {
        if (pendingJoinResult != null) {
            pendingJoinResult.success(false);
            pendingJoinResult = null;
            pendingJoinOptions = null;
        }
    }

    // Legacy host methods require an already signed-in Zoom user.
    private void startMeeting(MethodCall call, Result result, boolean normalMeeting) {
        ZoomSDK sdk = getInitializedZoomInstance();
        Map<String, String> options = call.arguments();
        if (sdk == null || pendingInitResult != null || activity == null || options == null) {
            result.success(Arrays.asList("SDK ERROR", "001"));
            return;
        }
        if (!sdk.isLoggedIn()) {
            result.success(Arrays.asList("LOGIN REQUIRED", "001"));
            return;
        }
        MeetingService service = sdk.getMeetingService();
        if (service == null || service.getMeetingStatus() != MeetingStatus.MEETING_STATUS_IDLE) {
            result.success(Arrays.asList("SDK ERROR", "001"));
            return;
        }

        StartMeetingOptions opts = new StartMeetingOptions();
        configureMeetingOptions(opts, options);
        opts.no_audio = parseBoolean(options, "noAudio");
        autoAudioJoinEnabled = !opts.no_audio;
        audioPermissionRequested = false;
        int error;
        if (normalMeeting) {
            StartMeetingParams4NormalUser params = new StartMeetingParams4NormalUser();
            params.meetingNo = options.get("meetingId");
            error = service.startMeetingWithParams(activity, params, opts);
        } else {
            error = service.startInstantMeeting(activity, opts);
        }
        if (error != MeetingError.MEETING_ERROR_SUCCESS) {
            autoAudioJoinEnabled = false;
        }
        result.success(error == MeetingError.MEETING_ERROR_SUCCESS
                ? Arrays.asList("MEETING SUCCESS", "200")
                : Arrays.asList("MEETING ERROR", String.valueOf(error)));
    }

    private void configureMeetingOptions(MeetingOptions opts, Map<String, String> options) {
        opts.no_invite = parseBoolean(options, "disableInvite");
        opts.no_share = parseBoolean(options, "disableShare");
        opts.no_titlebar = parseBoolean(options, "disableTitlebar");
        opts.no_driving_mode = parseBoolean(options, "disableDrive");
        opts.no_dial_in_via_phone = parseBoolean(options, "disableDialIn");
        opts.no_disconnect_audio = parseBoolean(options, "noDisconnectAudio");
        opts.no_video = parseBoolean(options, "noVideo");
        if (parseBoolean(options, "viewOptions")) {
            opts.meeting_views_options = MeetingViewsOptions.NO_TEXT_MEETING_ID | MeetingViewsOptions.NO_TEXT_PASSWORD;
        }
    }

    private void meetingDetails(Result result) {
        ZoomSDK sdk = getInitializedZoomInstance();
        InMeetingService service = sdk != null ? sdk.getInMeetingService() : null;
        if (service == null || !service.isMeetingConnected()) {
            result.success(Arrays.asList("MEETING_STATUS_UNKNOWN", "No meeting available"));
            return;
        }
        result.success(Arrays.asList(service.getCurrentMeetingNumber(), service.getMeetingPassword()));
    }

    private void meetingStatus(Result result) {
        MeetingService service = getMeetingService();
        MeetingStatus status = service != null ? service.getMeetingStatus() : null;
        result.success(status != null ? Arrays.asList(status.name(), "")
                : Arrays.asList("MEETING_STATUS_UNKNOWN", "No status available"));
        if (status == MeetingStatus.MEETING_STATUS_INMEETING) {
            connectClassroomAudio();
        }
    }

    private void connectClassroomAudio() {
        if (!autoAudioJoinEnabled || activity == null) {
            return;
        }
        InMeetingService service = ZoomSDK.getInstance().getInMeetingService();
        if (service == null) {
            return;
        }
        InMeetingAudioController controller = service.getInMeetingAudioController();
        if (controller == null || controller.isAudioConnected()) {
            return;
        }

        // Classroom can hide Zoom before its audio setup completes.
        if (activity.checkSelfPermission(Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED) {
            controller.connectAudioWithVoIP();
        } else if (!audioPermissionRequested && activity.hasWindowFocus()) {
            audioPermissionRequested = true;
            activity.requestPermissions(new String[]{Manifest.permission.RECORD_AUDIO}, RECORD_AUDIO_PERMISSION_CODE);
        }
    }

    private void leaveMeeting(Result result) {
        cancelPendingJoin();
        autoAudioJoinEnabled = false;
        MeetingService service = getMeetingService();
        if (service != null && isMeetingActive(service.getMeetingStatus()) && !leaveRequested) {
            // Leave only this participant; the SDK finishes the meeting asynchronously.
            leaveRequested = true;
            service.leaveCurrentMeeting(false);
        }
        result.success(true);
    }

    private void showMeeting(Result result) {
        ZoomSDK sdk = getInitializedZoomInstance();
        MeetingService service = getMeetingService();
        if (sdk == null || service == null || activity == null || !isMeetingActive(service.getMeetingStatus())) {
            result.success(false);
            return;
        }
        sdk.getZoomUIService().hideMiniMeetingWindow();
        service.returnToMeeting(activity);
        result.success(true);
    }

    private void hideMeeting(Result result) {
        MeetingService service = getMeetingService();
        if (service == null || !isMeetingActive(service.getMeetingStatus())) {
            result.success(false);
            return;
        }
        Intent intent = new Intent(MyMeetingActivity.ACTION_HIDE_MEETING);
        intent.setPackage(context.getPackageName());
        context.sendBroadcast(intent);
        result.success(true);
    }

    private boolean isMeetingActive(MeetingStatus status) {
        return status != null && status != MeetingStatus.MEETING_STATUS_IDLE
                && status != MeetingStatus.MEETING_STATUS_FAILED
                && status != MeetingStatus.MEETING_STATUS_ENDED
                && status != MeetingStatus.MEETING_STATUS_DISCONNECTING;
    }

    @Nullable
    private MeetingService getMeetingService() {
        ZoomSDK sdk = getInitializedZoomInstance();
        return sdk != null ? sdk.getMeetingService() : null;
    }

    @Nullable
    private ZoomSDK getInitializedZoomInstance() {
        ZoomSDK sdk = ZoomSDK.getInstance();
        return sdk.isInitialized() ? sdk : null;
    }
}
