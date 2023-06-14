package com.evilratt.flutter_zoom_sdk.utils;

import java.util.Map;

public class Utils {
    public static boolean parseBoolean(Map<String, String> options, String property) {
        return options.get(property) != null && Boolean.parseBoolean(options.get(property));
    }
}
