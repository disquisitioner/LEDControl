#include <FastLED.h>
#include <LEDControl.h>

#define NUM_LEDS  16
#define DATA_PIN_1  10
#define DATA_PIN_2  11

CRGB ledsOne[NUM_LEDS];
CRGB ledsTwo[NUM_LEDS];

LEDControl stripOne(NUM_LEDS,ledsOne);
LEDControl stripTwo(NUM_LEDS,ledsTwo);

void setup() {

  Serial.begin(115200);

  // Initialize the LED strip  
  FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(ledsOne,NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN_2, GRB>(ledsTwo,NUM_LEDS);


  // Simple startup test -- simulate a progress indicator
  for(int i=0;i<=100;i+=5) {
    stripOne.setProgress(CRGB::Blue,i);
    stripTwo.setProgress(CRGB::Green,100-i);
    stripOne.update();
    stripTwo.update();
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
      case 0:
        stripOne.setRunFwd(CRGB::Green);
        stripTwo.setRunFwd(CRGB::Orange); 
        break;
        
      case 1:
        stripOne.setRunRev(CRGB::Orange);       
        stripTwo.setRunRev(CRGB::Green);
        break;
        
      case 2:
        stripOne.setRainbowFwd();
        stripTwo.setRainbowRev();
        break;

      case 3:
        stripOne.setCylon(CRGB::Red);
        stripTwo.setCylon(CRGB::Red);
        break;
 
      case 4:
        bitmap = 0b1100110011001100;  // marquee 

        stripOne.setMarquee(CRGB::Yellow,bitmap); 
        stripTwo.setMarquee(CRGB::Blue,bitmap);
        break;
   
      case 5: // Side-by-side comparison of breathing...
        stripOne.setBreathe(CRGB::Purple);
        stripTwo.setOneColor(CRGB::Purple);
        break;
        
      case 6: // ...for two cycles, just for emphasis
        stripOne.setBreathe(CRGB::Purple);  
        stripTwo.setOneColor(CRGB::Purple);
        break;
        
      default: stripOne.setOneColor(CRGB::Orange); break;  // should never see this
    }
  }
  counter++;
  
  stripOne.update();
  stripTwo.update();
  FastLED.show();
  delay(100);
  
}
