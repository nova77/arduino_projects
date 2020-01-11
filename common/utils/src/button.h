// A simple button handling class.
//
// Example usage:
//
// static Button button(/*pin=*/7);
// void setup() {
//   button.setup();
// }
// void loop() {
//   button.loop(/*on_press=*/[] {
//     Serial.println("The button has been pressed!");
//     // do other stuff
//   }
// }

#ifndef _BUTTON_h
#define _BUTTON_h

#include <Arduino.h>

class Button {
 public:
  typedef void (*CallbackFn)();  // the callback function is just void f();

 public:
  // Set up button. Any digital pin is accepted.
  explicit Button(uint8_t pin) : pin_(pin), button_down_(0) {}

  // To call in main's "void setup()".
  void setup() const;

  // Run the loop for the button. Will run the callback
  // during on_press and on_release events. Both are optional.
  // Will return true if any of the events have been triggered.
  bool loop(CallbackFn on_press = nullptr, CallbackFn on_release = nullptr);

 private:
  uint8_t pin_;
  uint8_t button_down_;
};

#endif  // _BUTTON_h
