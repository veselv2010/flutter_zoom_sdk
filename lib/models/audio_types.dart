// ignore_for_file: constant_identifier_names

enum ZmAudioType {
  AUDIO_SOURCE_UNKNOWN(-2),
  AUDIO_SOURCE_NONE(-1),
  AUDIO_SOURCE_SPEAKER_PHONE(0),
  AUDIO_SOURCE_EAR_PHONE(1),
  AUDIO_SOURCE_WIRED(2),
  AUDIO_SOURCE_BLUETOOTH(3);

  final int code;

  const ZmAudioType(this.code);

  factory ZmAudioType.fromCode(int code) {
    return values[code];
  }
}

/// https://zoom.github.io/zoom-sdk-android/us/zoom/sdk/InMeetingServiceListener.AudioStatus.html
enum ZmAudioStatus {
  Audio_None,
  Audio_Muted,
  Audio_UnMuted,
  Audio_Muted_ByHost,
  Audio_UnMuted_ByHost,
  Audio_MutedAll_ByHost,
  Audio_UnMutedAll_ByHost;
}
