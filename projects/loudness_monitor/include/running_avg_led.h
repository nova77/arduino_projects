#ifndef _RUNNING_AVG_RGB_LED_h
#define _RUNNING_AVG_RGB_LED_h

#include <Arduino.h>
#include "rgb_led.h"

class RunningAvgRgbLed : public RgbLed {
 public:
  RunningAvgRgbLed(eLedType type, uint8_t red_pin, uint8_t green_pin,
                   uint8_t blue_pin)
      : RgbLed(type, red_pin, green_pin, blue_pin) {}

  void set_from_running_average(float running_average);

 private:
  struct sReaction {
    float threshold;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    float intensity;
  };

  static sReaction const thresholds_[];
};

#endif  // _RUNNING_AVG_RGB_LED_h
