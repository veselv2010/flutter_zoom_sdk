import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_zoom_sdk/flutter_zoom_sdk_method_channel.dart';

void main() {
  MethodChannelFlutterZoomSdk platform = MethodChannelFlutterZoomSdk();
  const MethodChannel channel = MethodChannel('flutter_zoom_sdk');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await platform.getPlatformVersion(), '42');
  });
}
