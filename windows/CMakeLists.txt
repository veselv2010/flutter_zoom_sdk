# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "flutter_zoom_sdk")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "${PROJECT_NAME}_plugin")

# Any new source files that you add to the plugin should be added here.
# Define the plugin library target. Its name must not be changed (see comment
# on PLUGIN_NAME above).
add_library(${PLUGIN_NAME} SHARED
  "include/flutter_zoom_sdk/flutter_zoom_sdk_plugin_c_api.h"
  "flutter_zoom_sdk_plugin_c_api.cpp"
  "flutter_zoom_sdk_plugin.cpp"
  "flutter_zoom_sdk_plugin.h"
)

# Symbols are hidden by default to reduce the chance of accidental conflicts
# between plugins. This should not be removed; any symbols that should be
# exported should be explicitly exported with the FLUTTER_PLUGIN_EXPORT macro.
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_features(${PLUGIN_NAME} PRIVATE cxx_std_17)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)

# Source include directories and library dependencies. Add any plugin-specific
# dependencies here.
target_include_directories(${PLUGIN_NAME} PUBLIC
  "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE
  flutter flutter_wrapper_plugin shell32)

find_library(SDK_LOCATION NAMES sdk
  PATHS "${PROJECT_SOURCE_DIR}/lib" NO_DEFAULT_PATH)
if(NOT SDK_LOCATION)
  message(FATAL_ERROR "Zoom SDK import library not found in windows/lib")
endif()

add_executable(flutter_zoom_host WIN32 "ZoomHost.cpp")
set_target_properties(flutter_zoom_host PROPERTIES OUTPUT_NAME "ZoomHost")
target_compile_features(flutter_zoom_host PRIVATE cxx_std_17)
target_include_directories(flutter_zoom_host PRIVATE
  "${PROJECT_SOURCE_DIR}/h")
target_link_libraries(flutter_zoom_host PRIVATE
  "${SDK_LOCATION}" shell32 propsys ole32 uuid)

if(MSVC)
  target_compile_options(${PLUGIN_NAME} PRIVATE /EHsc)
  target_compile_options(flutter_zoom_host PRIVATE /EHsc)
endif()

set(ZOOM_HOST_INSTALL_DIR "$<TARGET_FILE_DIR:${BINARY_NAME}>/zoom_host")
install(TARGETS flutter_zoom_host RUNTIME
  DESTINATION "${ZOOM_HOST_INSTALL_DIR}" COMPONENT Runtime)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/zoom_bin/"
  DESTINATION "${ZOOM_HOST_INSTALL_DIR}" COMPONENT Runtime)

# List of absolute paths to libraries that should be bundled with the plugin.
# This list could contain prebuilt libraries, or libraries created by an
# external build triggered from this build file.
set(flutter_zoom_sdk_bundled_libraries "" PARENT_SCOPE)
