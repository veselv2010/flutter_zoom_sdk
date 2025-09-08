@JS()
library;

import 'dart:js_interop';

/// Calls invoke JavaScript `JSON.stringify(obj)`.
@JS('JSON.stringify')
external JSString stringify(JSAny obj);
