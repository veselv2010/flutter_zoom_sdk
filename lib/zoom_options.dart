/// Basic Zoom Options required for plugin (WEB, iOS, Android)
// ignore_for_file: constant_identifier_names

class ZoomOptions {
  final String? leaveUrl;
  final String? helper;
  final String? domain;
  final String? language; //--for web
  final bool? showMeetingHeader; //--for web
  final bool? disableInvite; //--for web
  final bool? disableCallOut; //--for web
  final bool? disableRecord; //--for web
  final bool? disableJoinAudio; //--for web
  final bool? audioPanelAlwaysOpen; //--for web
  final bool? isSupportAV; //--for web
  final bool? isSupportChat; //--for web
  final bool? isSupportQA; //--for web
  final bool? isSupportCC; //--for web
  final bool? isSupportPolling; //--for web
  final bool? isSupportBreakout; //--for web
  final bool? screenShare; //--for web
  final String? rwcBackup; //--for web
  final bool? videoDrag; //--for web
  final String? sharingMode; //--for web
  final bool? videoHeader; //--for web
  final bool? isLockBottom; //--for web
  final bool? isSupportNonverbal; //--for web
  final bool? isShowJoiningErrorDialog; //--for web
  final bool? disablePreview; //--for web
  final bool? disableCORP; //--for web
  final String? inviteUrlFormat; //--for web
  final bool? disableVOIP; //--for web
  final bool? disableReport; //--for web
  final List<String>? meetingInfo; //--for web

  const ZoomOptions({
    required this.domain,
    this.leaveUrl,
    this.helper,
    this.language = "en-US",
    this.showMeetingHeader = true,
    this.disableInvite = false,
    this.disableCallOut = false,
    this.disableRecord = false,
    this.disableJoinAudio = false,
    this.audioPanelAlwaysOpen = false,
    this.isSupportAV = true,
    this.isSupportChat = true,
    this.isSupportQA = true,
    this.isSupportCC = true,
    this.isSupportPolling = true,
    this.isSupportBreakout = true,
    this.screenShare = true,
    this.rwcBackup = '',
    this.videoDrag = true,
    this.sharingMode = 'both',
    this.videoHeader = true,
    this.isLockBottom = true,
    this.isSupportNonverbal = true,
    this.isShowJoiningErrorDialog = true,
    this.disablePreview = false,
    this.disableCORP = true,
    this.inviteUrlFormat = '',
    this.disableVOIP = false,
    this.disableReport = false,
    this.meetingInfo = const [
      'topic',
      'host',
      'mn',
      'pwd',
      'telPwd',
      'invite',
      'participant',
      'dc',
      'enctype',
      'report'
    ],
  });
}

/// Basic Zoom Meeting Options required for plugin (WEB, iOS, Android)
class ZoomMeetingOptions {
  String? userId;

  /// Username For Join Meeting & Host Email For Start Meeting
  String? userPassword;

  /// Host Password For Start Meeting
  String? displayName;

  /// Display Name
  String? meetingId;

  /// Personal meeting id for start meeting required
  String? meetingPassword;

  /// Personal meeting passcode for start meeting required
  String? disableDialIn;

  /// Disable Dial In Mode
  String? disableDrive;

  /// Disable Drive In Mode
  String? disableInvite;

  /// Disable Invite Mode
  String? disableShare;

  /// Disable Share Mode
  String? disableTitlebar;

  /// Disable Title Bar Mode
  String? noDisconnectAudio;

  /// No Disconnect Audio Mode
  String? viewOptions;

  /// View option to disable zoom icon for Learning system
  String? noAudio;

  /// Disable No Audio
  String? zoomToken;

  /// Zoom token for SDK
  String? zoomAccessToken;

  /// Zoom access token for SDK
  String? jwtAPIKey;

  /// JWT API KEY For Web Only
  String? jwtSignature;

  /// JWT API Signature For Web Only

  ZoomMeetingOptions({
    this.userId,
    this.userPassword,
    this.displayName,
    this.meetingId,
    this.meetingPassword,
    this.disableDialIn,
    this.disableDrive,
    this.disableInvite,
    this.disableShare,
    this.disableTitlebar,
    this.noDisconnectAudio,
    this.viewOptions,
    this.noAudio,
    this.zoomToken,
    this.zoomAccessToken,
    this.jwtAPIKey,
    this.jwtSignature,
  });
}

///Zoom Login Error Codes
class ZoomError {
  ///Login Success
  static const ZOOM_AUTH_ERROR_SUCCESS = 0;

  ///Login Disabled
  static const ZOOM_AUTH_EMAIL_LOGIN_DISABLE = 1;

  ///User Not Exists
  static const ZOOM_AUTH_ERROR_USER_NOT_EXIST = 2;

  ///Wrong Password
  static const ZOOM_AUTH_ERROR_WRONG_PASSWORD = 3;

  ///Multiple Failed Login --- Account Locked
  static const ZOOM_AUTH_ERROR_WRONG_ACCOUNTLOCKED = 4;

  ///Wrong SDK -- Update Required
  static const ZOOM_AUTH_ERROR_WRONG_SDKNEEDUPDATE = 5;

  ///Too Many Failed Attempts
  static const ZOOM_AUTH_ERROR_WRONG_TOOMANY_FAILED_ATTEMPTS = 6;

  ///SMS Code Error
  static const ZOOM_AUTH_ERROR_WRONG_SMSCODEERROR = 7;

  ///SMS Code Expired
  static const ZOOM_AUTH_ERROR_WRONG_SMSCODEEXPIRED = 8;

  ///Phone Number Format Invalid
  static const ZOOM_AUTH_ERROR_WRONG_PHONENUMBERFORMATINVALID = 9;

  ///Login Token Invalid
  static const ZOOM_AUTH_ERROR_LOGINTOKENINVALID = 10;

  ///Login Disclamier Disagreed
  static const ZOOM_AUTH_ERROR_UserDisagreeLoginDisclaimer = 11;

  ///Other Issue
  static const ZOOM_AUTH_ERROR_WRONG_OTHER_ISSUE = 100;
}
