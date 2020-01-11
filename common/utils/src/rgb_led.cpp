#include "rgb_led.h"

// Note: the pins which allow brightness are: 3, 5, 6, 9, 10, 11
RgbLed::RgbLed(eLedType type, uint8_t red_pin, uint8_t green_pin,
               uint8_t blue_pin)
    : type_(type),
      red_pin_(red_pin),
      green_pin_(green_pin),
      blue_pin_(blue_pin) {}

void RgbLed::setup() const {
  pinMode(red_pin_, OUTPUT);
  pinMode(green_pin_, OUTPUT);
  pinMode(blue_pin_, OUTPUT);
  clear();  // initially set as off
}

void RgbLed::set(uint8_t red, uint8_t green, uint8_t blue,
                 float brightness) const {
  if (brightness < 1) {
    red *= brightness;
    green *= brightness;
    blue *= brightness;
  }

  // Flip around if it's anode type.
  if (type_ == LT_ANODE) {
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  }

  analogWrite(red_pin_, red);
  analogWrite(green_pin_, green);
  analogWrite(blue_pin_, blue);
}
