#include "running_avg_led.h"

/////////////////////////////////////////////////////////////
// Configuration settings

// The list of thresholds and their colors once they trigger.
// This represent the thresholds for the running average of weights
// above which a specific color will trigger: purple, red, orange, yellow.
// TODO: also add flashing stuff?.
const RunningAvgRgbLed::sReaction RunningAvgRgbLed::thresholds_[] = {
    {0.4f, 255, 50, 50, 1.f},   // purple
    {0.2f, 255, 0, 0, 1.0f},    // red
    {0.15f, 255, 50, 0, 0.7f},  // orange
    {0.1f, 100, 100, 0, 0.2f},  // yellowish
};

void RunningAvgRgbLed::set_from_running_average(float running_average) {
  char num_threholds = sizeof(RunningAvgRgbLed::thresholds_) /
                       sizeof(RunningAvgRgbLed::sReaction);

  for (unsigned char i = 0; i < num_threholds; ++i) {
    const auto& react = thresholds_[i];
    if (running_average > react.threshold) {
      set(react.red, react.green, react.blue, react.intensity);
      return;
    }
  }

  clear();
}
