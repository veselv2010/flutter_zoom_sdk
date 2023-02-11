#ifndef FLUTTER_PLUGIN_FLUTTER_ZOOM_SDK_PLUGIN_H_
#define FLUTTER_PLUGIN_FLUTTER_ZOOM_SDK_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace flutter_zoom_sdk {

class FlutterZoomSdkPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  FlutterZoomSdkPlugin();

  virtual ~FlutterZoomSdkPlugin();

  // Disallow copy and assign.
  FlutterZoomSdkPlugin(const FlutterZoomSdkPlugin&) = delete;
  FlutterZoomSdkPlugin& operator=(const FlutterZoomSdkPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  void AuthorizeSDK();
};

}  // namespace flutter_zoom_sdk

#endif  // FLUTTER_PLUGIN_FLUTTER_ZOOM_SDK_PLUGIN_H_
