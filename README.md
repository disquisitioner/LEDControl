LED Control
===============
LED Control is a utility library designed to make it easy to  generate patterns and animations on LED strips, making use of the excellent FastLED library (https://fastled.io) to handle low level command and control of a wide variety of LEDs.

## Basic Use
All patterns and animations supported by LEDControl are based on a simple state machine where patterns are assigned to each attached LED strip and a per-strip update function is called each clock cycle to carry out the desired animation effect. Strips can be put in any supported animation mode as desired, at any time, knowing that they will properly updated as part of each clock cycle.  In the simplest case animations are assigned to each strip at the start of the program and then the event loop runs to carry out those animations indefinitely.  More likely, though, animations will change based on some overall state of the executing program -- commands received, transitions in operating mode perhaps based on an external sensor or microphone, or just to inform the user of system state such as network connection lost or battery power low.

Each strip used must be defined properly using the FastLED library, as LEDControl animations will use FastLED functions to carry out the desired lighting effects.  FastLED library functions can be intermixed with LEDControl functions assuming reasonable knowledge of how FastLED works, for example as part of setting overall LED brightness, doing color math, etc.  (See the excellent FastLED documention for more insight on this.)

A very simple example program using a strip of sixteen WS2812B LEDs would look like this:

```
#include "Arduino.h"
#include <FastLED.h>
#include <LEDControl.h>

#define NUM_LEDS 16    // How many LEDs are in the attached strip?
#define DATA_PIN 10    // To which digital data pin is the LED strip connected?

CRGB leds[NUM_LEDS];   // Creates the array of LEDs to be manipulated by the FastLED library
LEDControl ledstrip(NUM_LEDS,leds);  // Create the LEDControl library object for set of LEDs

void setup() {
    FastLED.addLeds<WS2812B, DATA_PIN>(leds,NUM_LEDS);  // Register the LED strip via FastLED library
    ledstrip.setRunFwd(CRGB::Blue);                     // Set the strip animation to a forward run
}

// Simple main loop just updates the LED strip every clock tick, will fun forever
void loop() {
    ledstrip.update();   // Update display state of the LED strip per the assigned animation
    FastLED.show();      // Causes FastLED to drive the LEDs and display them as required
    delay(100);          // 10Hz clock for driving animations. (Try some others for fun... )
}
```

## Supported Animations
The following animation effects are provided by LEDControl:
* __One Color__ -- Sets all LEDs in the strip to a single specified CRGB `color`.  (CRGB colors are defined by the FastLED library.)
* __Run Forward__ -- Cycles a specified CRGB `color` forward along the strip lighting each LED one-by-one.  Forward here means starting at LED #0 and progressing to the last LED in the strip.
* __Run Reverse__ -- Cycles a specified CRGB `color` in reverse along the strip, one-by-one, starting at the last LED and sequencing back towards LED #0
* __Cylon__ -- Cycles a specified CRGB `color` forward along the strip from LED #0 to the end, and then in reverse back to LED #0.  (Named after the eye of the Cylons from 'Battlestar Galactica'.)
* __Rainbow Forward__ -- Fills the LED strip with a rainbow of color from hue value 0 at LED #0 to hue value 255 at the end, using the HSV color space as supported by the FastLED library.  Once the strip is filled, it cycles forward one LED per clock cycle, making the rainbow color sequnce progress through the LEDs on the strip.
* __Rainbow Reverse__ -- Fills the LED strip with the same rainbow of color as Rainbow Forward, but then cycles the rainbow color sequence in reverse through the LEDs on the strip.
* __Pattern__ -- Takes a specified `bitmap` and lights LEDs in the strip with a specified CRGB `color` wherever 1s apperar in the bitmap.  Useful on its own for displaying simple static patterns or progress bars, and as the basis for creating all sorts of basic patterns and animations within the controlling program.  You need to reset the pattern bitmap whenever you want the pattern displayed to change, so there's no active animation in this effect. 
* __Marquee__ -- Generates the cycling lighting effect often seen on theater marquees where a pattern of lights appears to run around the marquee.  The desired pattern is spefied as a `bitmap` (as in Pattern mode), along with the CRGB `color` to be used.  That pattern will shift forward one LED each clock cycle, creating a chase effect along the strip.

All animations are designed to repeat indefinitely, so even though some represent a pattern that repeats periodically based on the number of LEDs in the strip the effect will work properly if left to run for any arbitrary period of time (or forever).  There is no need to keep track of pattern cycles, and patterns can be changed on any LED strip at any time -- even in mid cycle.

While pattern effects can be assigned anywhere and anywhere, somewhere in your application will be an internal clock loop that will call the `update()` function for every LEDstrip to be displayed and then call `FastLED.show()` one time to cause those updates to happen and LEDs to light up.  You can drive that clock function however works best for your application -- through simple delays (as in the example above), with more precise timing loops, with hardware timers, using external interrupts, etc.

The `examples` folder included in the LEDControl library contains sampler programs showcasing the various patterns as well as how they might be used.

## API Reference
* `void setOneColor(CRGB color)` -- sets all LEDs in the strip to the specified `color`
* `void setRunFwd(CRGB color)` -- lights one LED at time, in sequence from the first LED (#0) to the last, using the specified `color`.  Will take as many clock ticks as their are LEDs in the strip to complete the run.
* `void setRunRev(CRGB color)` -- lights one LED at a time, in sequence from the last LED to the first (#0), using the specified `color`.  Will take as many clock ticks as their are LEDs in the strip to complete the run.
* `void setCylon(CRGB color)` -- lights a single LED in the specified color and runs it along the strip forwards from the first LED (#0) to the last, then back to the beginning in reverse order.  The first and last LEDs will be lit twice, once as part of each run.  Will take twice as many clock ticks as there are LEDs in the strip to complete the full Cylon run.
* `void setRainbowFwd()` -- initializes the LED strip with a full rainbow of colors using the HSV color space, starting from a hue of 0 degrees (red) at the first strip, and running through a hugh of 360 degrees (also red) at the end of the strip.  Uses the HSV color system provided by the FastLED library.  Once the strip is initialized and all LEDS are lit, cycles each color forward as a run so the rainbow is seen to flow through the strip.  As colors flow off the end of the strip they reappear at the beginning.
* `void setRainbowRev()` -- the counterpart to `setRainbowFwd()`, but the colors cycle in the reverse direction.
* `void setPattern(CRGB color, unsigned long bitmap)` -- uses the specified `bitmap` to determine which LEDs in the strip should be lit, as zeros in the bitmap will correspond to 'off' LEDs and ones will be illuminated in the specified `color`.  Will be limited to at most 32 LEDs in a strip (given the length limitation of `unsigned long`).  The pattern does not change dynamically, though bitmap-based animcations can be easily created through calls with different bitmaps over successive clock cycles. 
* `void setProgress(CRGB color, int percent)` -- treats the LED strip as a progress bar and illuminates however many LEDs correspond to the stated percentage factor from zero to one hundred, using the specified `color`.
* `void setMarquee(CRGB color, unsigned long bitmap)` -- creates a cycling "marquee"-style effect as seen on many classic movie theaters.  The LEDs are illuminated according to the specified `bitmap` and `color` as is in the `setPattern()` mode, but once set the pattern will be cycled forward one LED per clock cycle.
* `void udpate()` -- the per-strip function called each clock cycle to advance that strip's animation effect to the next state, whatever that may happen to be.   Even though the strip may be displaying something static it's fine to call the update function as it won't change any displayed state.
