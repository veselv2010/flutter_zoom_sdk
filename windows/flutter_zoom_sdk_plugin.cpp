#include "flutter_zoom_sdk_plugin.h"
#include <include/zoom_sdk.h>

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

namespace flutter_zoom_sdk {

    using flutter::EncodableList;
    using flutter::EncodableMap;
    using flutter::EncodableValue;
    using namespace std;

// static
void FlutterZoomSdkPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "flutter_zoom_sdk",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<FlutterZoomSdkPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

FlutterZoomSdkPlugin::FlutterZoomSdkPlugin() {}

FlutterZoomSdkPlugin::~FlutterZoomSdkPlugin() {}

void FlutterZoomSdkPlugin::HandleMethodCall(
    const flutter::MethodCall<EncodableValue> &method_call,
    unique_ptr<flutter::MethodResult<EncodableValue>> result) {
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    result->Success(EncodableValue(version_stream.str()));
  } else if (method_call.method_name().compare("init") == 0)
        {
            EncodableList results;
            results.push_back(EncodableValue("init windows zoom sdk"));
            AuthorizeSDK();
            result->Success(results);
        }
  else {
    result->NotImplemented();
  }
}

void AuthorizeSDK()
{
    // Initialize SDK with InitParam object
    ZOOM_SDK_NAMESPACE::InitParam initParam;
    ZOOM_SDK_NAMESPACE::SDKError initReturnVal(ZOOM_SDK_NAMESPACE::SDKERR_UNINITIALIZE);

    // Set web domain to zoom.us
    initParam.strWebDomain = L"https://zoom.us";
    initReturnVal = ZOOM_SDK_NAMESPACE::InitSDK(initParam);

    // Check if InitSDK call succeeded
    if (initReturnVal == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS)
    {
        cout << "Initialized";
    }
}

}  // namespace flutter_zoom_sdk
