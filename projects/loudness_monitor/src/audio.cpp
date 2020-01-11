#include "audio.h"

//////////////////////////////////////////////////////
// Audio configuration

// The higher, the more sensitive the microphone is.
const float Audio::sensitivity_levels_[] = {0.5, 1.0, 2.0};
constexpr uint8_t default_sensitivity_idx = 1;

// If true it will log stuff. Useful for debugging.
// TODO: add debug levels?
constexpr bool DEBUGGING_LOGS = true;

//////////////////////////////////////////////////////

template <typename T, typename ResType = T>
ResType get_array_sum(const T arr[], uint8_t arr_size) {
  ResType sum = 0;
  for (auto i = 0; i < arr_size; ++i) {
    sum += arr[i];
  }
  return sum;
}

// Like map but for floats.
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//////////////////////////////////////////////////////

Audio::Audio(uint8_t analog_pin, uint16_t sample_window_ms,
             uint16_t loudness_window_ms)
    : analog_pin_(analog_pin),
      sample_window_ms_(sample_window_ms),
      sensitivity_idx_(default_sensitivity_idx),
      loudness_window_size_(loudness_window_ms / sample_window_ms),
      loudness_window_idx_(0) {
  loudness_window_ = new float[loudness_window_size_];
}

void Audio::setup() { clear_loudness_window(); }

void Audio::clear_loudness_window() {
  memset(loudness_window_, 0, sizeof(float) * loudness_window_size_);
}

float Audio::get_loudness_running_average() {
  auto volume = get_volume();
  return get_loudness_running_average(volume);
}

float Audio::get_sensitivity_level() const {
  return sensitivity_levels_[sensitivity_idx_];
}

float Audio::get_mapped_sensitivity() const {
  uint8_t num_lvls = sizeof(sensitivity_levels_) / sizeof(float);
  return mapf(get_sensitivity_level(),                                    //
              sensitivity_levels_[0], sensitivity_levels_[num_lvls - 1],  //
              0.1, 1.0);
}

void Audio::next_sensitivity() {
  uint8_t num_lvls = sizeof(sensitivity_levels_) / sizeof(float);
  sensitivity_idx_ = (sensitivity_idx_ + 1) % num_lvls;

  clear_loudness_window();

  Serial.print("New sensitivity level [");
  Serial.print(sensitivity_idx_);
  Serial.print("]: ");
  Serial.println(get_sensitivity_level());
}

void Audio::print_loudness_window() const {
  Serial.print("Loudness: [");
  for (uint8_t i = 0; i < loudness_window_size_; ++i) {
    Serial.print(loudness_window_[i]);
    Serial.print(",");
  }
  float tot = get_array_sum(loudness_window_, loudness_window_size_);

  Serial.print("]. Tot: ");
  Serial.print(tot);
  Serial.print(", avg: ");
  Serial.println(tot / loudness_window_size_);
}

//////////////////////////////////////////////////////

float Audio::get_volume() const {
  auto startMillis = millis();  // start of sample window
  int peakToPeak = 0;  // peak-to-peak level

  int signalMax = 0;
  int signalMin = 1024;
  // collect data for sample_window ms.
  while (millis() - startMillis < sample_window_ms_) {
    int sample = analogRead(analog_pin_);
    sample = constrain(sample, 0, 1024);  // avoid spurious reads

    if (sample > signalMax)
      signalMax = sample;  // save just the max levels
    else if (sample < signalMin)
      signalMin = sample;  // save just the min levels
  }

  peakToPeak = signalMax - signalMin;       // max - min = peak-peak amplitude
  float volts = (peakToPeak * 3.3) / 1024;  // convert to volts

  // Update volts with sensitivity level and constraint between 0 and 1.
  float volume = constrain(volts * get_sensitivity_level(), 0, 1);
  if constexpr (DEBUGGING_LOGS) {
    if (volume > 0.05) {
      Serial.print("volume: ");
      Serial.print(volume);
      Serial.print(" - ");
      for (auto j = 0; j < int(volume * 20 + 0.5); ++j) {
        Serial.print("*");
      }
      Serial.println();
    }
  }

  return volume;
}

// Returns the sum of the values inside the window.
float Audio::get_loudness_running_average(float volume) {
  loudness_window_idx_ = (loudness_window_idx_ + 1) % loudness_window_size_;
  loudness_window_[loudness_window_idx_] = volume;
  return get_array_sum(loudness_window_, loudness_window_size_) /
         loudness_window_size_;
}
