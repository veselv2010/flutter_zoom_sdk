import 'package:flutter/material.dart';

class MainPage extends StatelessWidget {
  const MainPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Main page'),
      ),
      body: Center(
        child: Padding(
          padding: const EdgeInsets.symmetric(
            vertical: 8.0,
            horizontal: 32.0,
          ),
          child: Padding(
            padding: const EdgeInsets.all(16.0),
            child: Builder(
              builder: (context) {
                // The basic Material Design action button.
                return ElevatedButton(
                  style: ElevatedButton.styleFrom(
                    foregroundColor: Colors.white,
                    backgroundColor: Colors.blue, // foreground
                  ),
                  onPressed: () {
                    Navigator.of(context).pushNamed('/meeting');
                  },
                  child: const Text('To meeting page'),
                );
              },
            ),
          ),
        ),
      ),
    );
  }
}
