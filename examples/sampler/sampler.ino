#include <FastLED.h>
#include <LEDControl.h>

#define NUM_LEDS  16
#define DATA_PIN  10

CRGB ledsOne[NUM_LEDS];

LEDControl stripOne(NUM_LEDS,ledsOne);

void setup() {

  Serial.begin(115200);

  // Initialize the LED strip  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(ledsOne,NUM_LEDS);

  // Simple startup test -- simulate a progress indicator
  for(int i=0;i<=100;i+=5) {
    stripOne.setProgress(CRGB::Blue,i);
    stripOne.update();
    FastLED.show();
    delay(100);
  }
  delay(1000);  // Leave initial display up 1 second
}

unsigned int counter = 0;

void loop() {

  int i, level_leds, progress;
  unsigned long bitmap;

  // Cycle through patterns, with 64 updates for each
  if( (counter % 64) == 0) {
    switch((counter/64)%7) {
      case 0: stripOne.setRunFwd(CRGB::Purple);    break;
      case 1: stripOne.setRunRev(CRGB::Red);       break;
      case 2: stripOne.setRainbowRev();            break;
      case 3: stripOne.setCylon(CRGB::Red);        break;
      case 4: 
        bitmap = 0b1100110011001100;  // marquee pattern
        stripOne.setMarquee(CRGB::Yellow,bitmap); break;
      case 5: stripOne.setOneColor(CRGB::Purple);  break;
      case 6: stripOne.setBreathe(CRGB::Orange);  break;
        
      default: stripOne.setOneColor(CRGB::Orange); break;  // should never see this
    }
  }
  counter++;
  
  stripOne.update();
  FastLED.show();
  delay(100);
  
}
