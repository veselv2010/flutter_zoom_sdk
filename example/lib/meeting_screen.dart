import 'dart:async';
import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_zoom_sdk/zoom_options.dart';
import 'package:flutter_zoom_sdk/zoom_platform_view.dart';

class MeetingWidget extends StatefulWidget {
  const MeetingWidget({super.key});

  @override
  _MeetingWidgetState createState() => _MeetingWidgetState();
}

class _MeetingWidgetState extends State<MeetingWidget> {
  late final TextEditingController meetingIdController;
  late final TextEditingController meetingPasswordController;
  late final ZoomPlatform zoom;
  late Timer timer;

  static const String sdkKey = '';
  static const String sdkSecret = '';

  @override
  void initState() {
    meetingIdController = TextEditingController();
    meetingPasswordController = TextEditingController();
    zoom = ZoomPlatform.instance;
    super.initState();
  }

  @override
  void dispose() {
    meetingIdController.dispose();
    meetingPasswordController.dispose();
    zoom.leaveMeeting();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () {
        FocusScope.of(context).unfocus();
      },
      child: Scaffold(
        appBar: AppBar(
          title: const Text('Join meeting'),
          leading: IconButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            icon: const Icon(Icons.arrow_back),
          ),
        ),
        body: Padding(
          padding: const EdgeInsets.symmetric(
            vertical: 8.0,
            horizontal: 32.0,
          ),
          child: SingleChildScrollView(
            child: Column(
              children: [
                Padding(
                  padding: const EdgeInsets.only(bottom: 8.0),
                  child: TextField(
                    controller: meetingIdController,
                    decoration: const InputDecoration(
                      border: OutlineInputBorder(),
                      labelText: 'Meeting ID',
                    ),
                  ),
                ),
                Padding(
                  padding: const EdgeInsets.only(bottom: 8.0),
                  child: TextField(
                    controller: meetingPasswordController,
                    decoration: const InputDecoration(
                      border: OutlineInputBorder(),
                      labelText: 'Password',
                    ),
                  ),
                ),
                Padding(
                  padding: const EdgeInsets.all(16.0),
                  child: Builder(
                    builder: (context) {
                      // The basic Material Design action button.
                      return ElevatedButton(
                        style: ElevatedButton.styleFrom(
                          foregroundColor: Colors.white,
                          backgroundColor: Colors.blue, // foreground
                        ),
                        onPressed: () {
                          FocusScope.of(context).unfocus();
                          joinMeeting(context);
                        },
                        child: const Text('Join'),
                      );
                    },
                  ),
                ),
                Padding(
                  padding: const EdgeInsets.all(16.0),
                  child: Builder(
                    builder: (context) {
                      // The basic Material Design action button.
                      return ElevatedButton(
                        style: ElevatedButton.styleFrom(
                          foregroundColor: Colors.white,
                          backgroundColor: Colors.blue, // foreground
                        ),
                        onPressed: () {
                          startMeeting(context);
                        },
                        child: const Text('Start Meeting'),
                      );
                    },
                  ),
                ),
                Padding(
                  padding: const EdgeInsets.all(16.0),
                  child: Builder(
                    builder: (context) {
                      // The basic Material Design action button.
                      return ElevatedButton(
                        style: ElevatedButton.styleFrom(
                          foregroundColor: Colors.white,
                          backgroundColor: Colors.blue, // foreground
                        ),
                        onPressed: () => startMeetingNormal(context),
                        child: const Text('Start Meeting With Meeting ID'),
                      );
                    },
                  ),
                ),
                Padding(
                  padding: const EdgeInsets.all(16.0),
                  child: Builder(
                    builder: (context) {
                      // The basic Material Design action button.
                      return ElevatedButton(
                        style: ElevatedButton.styleFrom(
                          foregroundColor: Colors.white,
                          backgroundColor: Colors.blue, // foreground
                        ),
                        onPressed: () async => openZoomActivity(context),
                        child: const Text('Open zoom activity'),
                      );
                    },
                  ),
                )
              ],
            ),
          ),
        ),
      ),
    );
  }

  //API KEY & SECRET is required for below methods to work
  //Join Meeting With Meeting ID & Password
  void joinMeeting(BuildContext context) {
    bool _isMeetingEnded(String status) {
      var result = false;

      if (Platform.isAndroid) {
        result = status == 'MEETING_STATUS_DISCONNECTING' ||
            status == 'MEETING_STATUS_FAILED';
      } else {
        result = status == 'MEETING_STATUS_IDLE';
      }

      return result;
    }

    if (meetingIdController.text.isNotEmpty &&
        meetingPasswordController.text.isNotEmpty) {
      String? signature;

      if (kIsWeb) {
        signature = zoom.generateSignature(
          sdkKey,
          sdkSecret,
          meetingIdController.text,
          0,
        );
      }

      ZoomOptions zoomOptions = const ZoomOptions(
        domain: 'zoom.us',
        appKey: sdkKey, //API KEY FROM ZOOM
        appSecret: sdkSecret, //API SECRET FROM ZOOM
      );
      var meetingOptions = ZoomMeetingOptions(
        userId: 'userId',

        /// pass username for join meeting only
        displayName: 'userName',
        sdkKey: sdkKey,

        /// pass meeting id for join meeting only
        meetingId: meetingIdController.text,

        /// pass meeting password for join meeting only
        meetingPassword: meetingPasswordController.text,
        disableDialIn: 'true',
        disableDrive: 'true',
        disableInvite: 'true',
        disableShare: 'true',
        disableTitlebar: 'false',
        viewOptions: 'true',
        noAudio: 'false',
        noVideo: 'false',
        noDisconnectAudio: 'false',
        signature: signature,
      );

      if (Platform.isWindows) {
        zoom.initZoomAndJoinMeeting(zoomOptions, meetingOptions).then((result) {
          if (result) {
            zoom.onMeetingStatus().listen((status) {
              if (status[0] == 'MEETING_STATUS_DISCONNECTING') {
                if (kDebugMode) {
                  print('[Meeting Status] :- Ended');
                }
              }
            });
          }
        });

        return;
      }

      zoom.initZoom(zoomOptions).then((results) {
        if (results[0] == 0) {
          zoom.onMeetingStatus().listen((status) {
            if (kDebugMode) {
              print(
                  '[Meeting Status Stream] : ' + status[0] + ' - ' + status[1]);
            }
            if (_isMeetingEnded(status[0])) {
              if (kDebugMode) {
                print('[Meeting Status] :- Ended');
              }
              timer.cancel();
            }
          });
          if (kDebugMode) {
            print('listen on event channel');
          }
          zoom.joinMeeting(meetingOptions, true).then((joinMeetingResult) {
            timer = Timer.periodic(const Duration(seconds: 2), (timer) {
              zoom.meetingStatus(meetingOptions.meetingId!).then((status) {
                if (kDebugMode) {
                  print('[Meeting Status Polling] : ' +
                      status[0] +
                      ' - ' +
                      status[1]);
                }
              });
            });
          });
        }
      }).catchError((error) {
        if (kDebugMode) {
          print('[Error Generated] : ' + error);
        }
      });
    } else {
      if (meetingIdController.text.isEmpty) {
        ScaffoldMessenger.of(context).showSnackBar(const SnackBar(
          content: Text('Enter a valid meeting id to continue.'),
        ));
      } else if (meetingPasswordController.text.isEmpty) {
        ScaffoldMessenger.of(context).showSnackBar(const SnackBar(
          content: Text('Enter a meeting password to start.'),
        ));
      }
    }
  }

  //Start Meeting With Random Meeting ID ----- Emila & Password For Zoom is required.
  void startMeeting(BuildContext context) {
    bool _isMeetingEnded(String status) {
      var result = false;

      if (Platform.isAndroid) {
        result = status == 'MEETING_STATUS_DISCONNECTING' ||
            status == 'MEETING_STATUS_FAILED';
      } else {
        result = status == 'MEETING_STATUS_IDLE';
      }

      return result;
    }

    ZoomOptions zoomOptions = const ZoomOptions(
      domain: 'zoom.us',
      appKey: sdkKey, //API KEY FROM ZOOM -- SDK KEY
      appSecret: sdkSecret, //API SECRET FROM ZOOM -- SDK SECRET
    );
    var meetingOptions = ZoomMeetingOptions(
        userId: 'user@gmail.com', //pass host email for zoom
        userPassword: 'userPsw', //pass host password for zoom
        disableDialIn: 'false',
        disableDrive: 'false',
        disableInvite: 'false',
        disableShare: 'false',
        disableTitlebar: 'false',
        viewOptions: 'true',
        noAudio: 'false',
        noDisconnectAudio: 'false');

    zoom.initZoom(zoomOptions).then((results) {
      if (results[0] == 0) {
        zoom.onMeetingStatus().listen((status) {
          if (kDebugMode) {
            print('[Meeting Status Stream] : ' + status[0] + ' - ' + status[1]);
          }
          if (_isMeetingEnded(status[0])) {
            if (kDebugMode) {
              print('[Meeting Status] :- Ended');
            }
            timer.cancel();
          }
          if (status[0] == 'MEETING_STATUS_INMEETING') {
            zoom.meetingDetails().then((meetingDetailsResult) {
              if (kDebugMode) {
                print('[MeetingDetailsResult] :- ' +
                    meetingDetailsResult.toString());
              }
            });
          }
        });
        zoom.startMeeting(meetingOptions).then((loginResult) {
          if (kDebugMode) {
            print(
                '[LoginResult] :- ' + loginResult[0] + ' - ' + loginResult[1]);
          }
          if (loginResult[0] == 'SDK ERROR') {
            //SDK INIT FAILED
            if (kDebugMode) {
              print((loginResult[1]).toString());
            }
            return;
          } else if (loginResult[0] == 'LOGIN ERROR') {
            //LOGIN FAILED - WITH ERROR CODES
            if (kDebugMode) {
              if (loginResult[1] ==
                  ZoomError.ZOOM_AUTH_ERROR_WRONG_ACCOUNTLOCKED) {
                print('Multiple Failed Login Attempts');
              }
              print((loginResult[1]).toString());
            }
            return;
          } else {
            //LOGIN SUCCESS & MEETING STARTED - WITH SUCCESS CODE 200
            if (kDebugMode) {
              print((loginResult[0]).toString());
            }
          }
        }).catchError((error) {
          if (kDebugMode) {
            print('[Error Generated] : ' + error);
          }
        });
      }
    }).catchError((error) {
      if (kDebugMode) {
        print('[Error Generated] : ' + error);
      }
    });
  }

  //Start Meeting With Custom Meeting ID ----- Emila & Password For Zoom is required.
  void startMeetingNormal(BuildContext context) {
    bool _isMeetingEnded(String status) {
      var result = false;

      if (Platform.isAndroid) {
        result = status == 'MEETING_STATUS_DISCONNECTING' ||
            status == 'MEETING_STATUS_FAILED';
      } else {
        result = status == 'MEETING_STATUS_IDLE';
      }

      return result;
    }

    ZoomOptions zoomOptions = const ZoomOptions(
      domain: 'zoom.us',
      appKey: sdkKey, //API KEY FROM ZOOM -- SDK KEY
      appSecret: sdkSecret, //API SECRET FROM ZOOM -- SDK SECRET
    );
    var meetingOptions = ZoomMeetingOptions(
        userId: 'user@gmail.com', //pass host email for zoom
        userPassword: 'userPsw', //pass host password for zoom
        meetingId: meetingIdController.text,
        disableDialIn: 'false',
        disableDrive: 'false',
        disableInvite: 'false',
        disableShare: 'false',
        disableTitlebar: 'false',
        viewOptions: 'false',
        noAudio: 'false',
        noDisconnectAudio: 'false');

    zoom.initZoom(zoomOptions).then((results) {
      if (results[0] == 0) {
        zoom.onMeetingStatus().listen((status) {
          if (kDebugMode) {
            print('[Meeting Status Stream] : ' + status[0] + ' - ' + status[1]);
          }
          if (_isMeetingEnded(status[0])) {
            if (kDebugMode) {
              print('[Meeting Status] :- Ended');
            }
            timer.cancel();
          }
          if (status[0] == 'MEETING_STATUS_INMEETING') {
            zoom.meetingDetails().then((meetingDetailsResult) {
              if (kDebugMode) {
                print('[MeetingDetailsResult] :- ' +
                    meetingDetailsResult.toString());
              }
            });
          }
        });
        zoom.startMeetingNormal(meetingOptions).then((loginResult) {
          if (kDebugMode) {
            print('[LoginResult] :- ' + loginResult.toString());
          }
          if (loginResult[0] == 'SDK ERROR') {
            //SDK INIT FAILED
            if (kDebugMode) {
              print((loginResult[1]).toString());
            }
          } else if (loginResult[0] == 'LOGIN ERROR') {
            //LOGIN FAILED - WITH ERROR CODES
            if (kDebugMode) {
              print((loginResult[1]).toString());
            }
          } else {
            //LOGIN SUCCESS & MEETING STARTED - WITH SUCCESS CODE 200
            if (kDebugMode) {
              print((loginResult[0]).toString());
            }
          }
        });
      }
    }).catchError((error) {
      if (kDebugMode) {
        print('[Error Generated] : ' + error);
      }
    });
  }

  Future<void> openZoomActivity(BuildContext context) async {
    await zoom.openZoomActivity();
  }
}
