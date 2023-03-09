import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_zoom_sdk/flutter_zoom_sdk.dart';
import 'package:flutter_zoom_sdk/flutter_zoom_sdk_platform_interface.dart';
import 'package:flutter_zoom_sdk/flutter_zoom_sdk_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockFlutterZoomSdkPlatform
    with MockPlatformInterfaceMixin
    implements FlutterZoomSdkPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final FlutterZoomSdkPlatform initialPlatform = FlutterZoomSdkPlatform.instance;

  test('$MethodChannelFlutterZoomSdk is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelFlutterZoomSdk>());
  });

  test('getPlatformVersion', () async {
    FlutterZoomSdk flutterZoomSdkPlugin = FlutterZoomSdk();
    MockFlutterZoomSdkPlatform fakePlatform = MockFlutterZoomSdkPlatform();
    FlutterZoomSdkPlatform.instance = fakePlatform;

    expect(await flutterZoomSdkPlugin.getPlatformVersion(), '42');
  });
}
