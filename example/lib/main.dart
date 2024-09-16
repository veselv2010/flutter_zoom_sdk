import 'package:flutter/material.dart';
import 'main_page.dart';
import 'meeting_screen.dart';

// for complete example see https://github.com/evilrat/flutter_zoom_sdk/tree/master/example

void main() => runApp(const ExampleApp());

class ExampleApp extends StatelessWidget {
  const ExampleApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Example Zoom SDK',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      navigatorObservers: const [],
      debugShowCheckedModeBanner: false,
      initialRoute: '/',
      routes: {
        '/': (context) => const MainPage(),
        '/meeting': (context) => const MeetingWidget(),
      },
    );
  }
}
