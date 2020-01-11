#include "button.h"

void Button::setup() const { pinMode(pin_, INPUT); }

bool Button::loop(CallbackFn on_press, CallbackFn on_release) {
  auto button_state = digitalRead(pin_);

  if (button_state == 1 && button_down_ == 0) {
    // first press, triggering callback
    button_down_ = 1;
    if (on_press) {
      // Serial.println("Button pressed");
      on_press();
      return true;
    }
  } else if (button_state == 0 && button_down_ == 1) {
    button_down_ = 0;
    if (on_release) {
      // Serial.println("Button released");
      on_release();
      return true;
    }
  }

  return false;
}
