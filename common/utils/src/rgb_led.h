// A simple class to simplify access to an RGB LED.
//
// Example usage:
// static RgbLed rgb_led(RgbLed::LT_ANODE,  //  type of LED
//                       /*red_pin=*/3,
//                       /*green_pin=*/5,
//                       /*blue_pin=*/6);
// void setup() {
//   rgb_led.setup();
// }
// void loop() {
//   rgb_led.set(/*red=*/255, /*green=*/0, /*blue=*/0);
//   delay(1000);
//   rgb_led.set(/*red=*/0, /*green=*/255, /*blue=*/0);
//   delay(1000);
//   rgb_led.set(/*red=*/0, /*green=*/0, /*blue=*/255);
//   delay(1000);
//   rgb_led.clear();
//   delay(3000);
// }

#ifndef _RGB_LED_h
#define _RGB_LED_h

#include <Arduino.h>

class RgbLed {
 public:
  // The type of LED. Cathode is the one in the Arduino Kit (I think).
  enum eLedType {
    LT_CATHODE,  // Cathode LED: the long pin is GND.
    LT_ANODE,    // Anode LED: the long pin is +V.
  };

  // Note: the pins which allow brightness are: 3, 5, 6, 9, 10, 11
  RgbLed(eLedType type, uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin);

  // To call in main's "void setup()".
  void setup() const;

  // Set the value of the RGB LED.
  // All color arguments are bound between 0 and 255. If all of them are
  // set to 0 the LED is off. The brightness argument is bound between 0 and 1.
  void set(uint8_t red, uint8_t green, uint8_t blue,
           float brightness = 1.f) const;

  // Set everything to 0.
  void clear() const { set(0, 0, 0); }

 protected:
  // I could turn this class into pure template and save those
  // variables, but it feels a bit like an overkill.
  eLedType type_;

  uint8_t red_pin_;
  uint8_t green_pin_;
  uint8_t blue_pin_;
};

#endif  // _RGB_LED_h
