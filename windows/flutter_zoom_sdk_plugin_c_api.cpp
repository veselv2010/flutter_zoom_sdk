#include "include/flutter_zoom_sdk/flutter_zoom_sdk_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "flutter_zoom_sdk_plugin.h"

void FlutterZoomSdkPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  flutter_zoom_sdk::FlutterZoomSdkPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
