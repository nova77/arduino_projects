// Handle Audio stuff, mostly about collecting the volume values
// in a window.

#ifndef _AUDIO_h
#define _AUDIO_h

#include <Arduino.h>

//////////////////////////////////////////////////////

class Audio {
 public:
  // Creates the Audio Object.
  // Args:
  //   analog_pin: the analog pin where to read the value from
  //     microphone.
  //   sample_window_ms: sample window width in ms (50 ms = 20Hz)
  //   loudness_window_ms: the total size of the window (in ms)
  //     which will holds the volume values.
  explicit Audio(uint8_t analog_pin, uint16_t sample_window_ms = 50,
                 uint16_t loudness_window_ms = 4000);

  Audio(const Audio&) = delete;
  Audio& operator=(const Audio&) = delete;

  void setup();

  // Clears the running window with the volume values.
  void clear_loudness_window();

  // Returns the running average ov the volume values while
  // updating the window with the latest volume read.
  float get_loudness_running_average();

  // Returns the current sensitivity level of the microphone.
  float get_sensitivity_level() const;
  // Updates the sensitivity level to the next one (or loop around).
  void next_sensitivity();
  // Returns the sensitivity as mapped between 0.1 and 1.0.
  float get_mapped_sensitivity() const;

  // Print the volume values in the loudness window.
  // Used for debugging.
  void print_loudness_window() const;

 private:
  // Returns the volume of the microphone, between 0 and 1.
  float get_volume() const;

  // Returns the running average ov the volume values while
  // updating the window with the given volume value.
  float get_loudness_running_average(float volume);

 private:
  const uint8_t analog_pin_;
  const uint16_t sample_window_ms_;

  // The levels of sensitivity which are selected with
  // next_sensitivity.
  static const float sensitivity_levels_[];
  uint8_t sensitivity_idx_;

  // The window looks something like this:
  //    [0.0, 0.1, 0.0, 0.2, 0.8, 0.8, ...]
  // where the value is the volume, eg 1.0 represent a *very* loud noise.
  float* loudness_window_;
  uint8_t loudness_window_size_;
  uint8_t loudness_window_idx_;
};

#endif  // _AUDIO_h
