#include <Arduino.h>

#include "audio.h"
#include "button.h"
#include "running_avg_led.h"

/////////////////////////////////////////////////////////////
// Components settings

static Audio audio(/*analog_pin=*/A0);
static RunningAvgRgbLed rgb_led(RgbLed::LT_ANODE,  //
                                /*red_pin=*/9, /*green_pin=*/10,
                                /*blue_pin=*/11);
static Button button(/*pin=*/2);

void setup() {
  Serial.begin(9600);  // just for debugging
  delay(5);

  Serial.println("******* Setup *******");
  pinMode(LED_BUILTIN, OUTPUT);  
  audio.setup();
  rgb_led.setup();
  button.setup();
}

void loop() {
  // What to do when the button is pressed.
  button.loop(/*on_press=*/[] {
    // audio.print_loudness_window();  // this is just for debugging.
    audio.next_sensitivity();

    // Flick the LED to give indication of the current intensity.
    auto intensity = audio.get_mapped_sensitivity();
    rgb_led.set(0, 255, 0, intensity * .8);
    // Same with the builtin led as additional feedback.
    digitalWrite(LED_BUILTIN, HIGH);

    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    rgb_led.clear();
  });

  // Get the running average of the audio window.
  auto running_average = audio.get_loudness_running_average();
  // Set the LED color using the running average.
  rgb_led.set_from_running_average(running_average);
}
