#include <FastLED.h>
#include <IRremote.h>

#include "rainbow.h"
#include "noise.h"
#include "waze_img.h"
#include "wade.h"

#define IR_POWER    0xFFA25D
#define IR_VOLPLUS  0xFF629D
#define IR_VOLMIN   0xFFA857
#define IR_0        0xFF6897
#define IR_1        0xFF30CF
#define IR_2        0xFF18E7
#define IR_3        0xFF7A85
#define IR_4        0xFF10EF
#define IR_5        0xFF38C7

#define LED_PIN     3
#define IR_PIN      5

#define NUM_LEDS    240
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define MAX_BRIGHTNESS 200

//CRGB leds[NUM_LEDS];
CRGB real[NUM_LEDS];

IRrecv irrecv(IR_PIN);

volatile byte mode = 0;
volatile byte brightness = 100;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();

  delay(2000);
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(real, NUM_LEDS);
  FastLED.setBrightness(brightness);
  FastLED.setCorrection(TypicalLEDStrip);

  setupRainbow();
  setupNoise(SetupHalloweenPalette(), 10, 15);
  setupWazePic();
  setupWade();
  
}

void readIR() {
  decode_results  results;

  if (irrecv.decode(&results)) {
    Serial.println(String("value: ") + results.value);
    irrecv.resume();
  } else {
    return;
  }

  switch(results.value){
    case IR_0: mode = 0; FastLED.clear(); break;
    case IR_1: mode = 1; FastLED.clear(); break;
    case IR_2: mode = 2; FastLED.clear(); break;
    case IR_3: mode = 3; FastLED.clear(); break;
    case IR_4: mode = 4; FastLED.clear(); break;
    case IR_5: mode = 5; FastLED.clear(); break;
    case IR_POWER:
      if (brightness > 0) brightness = 0;
      else brightness = 100;
      FastLED.setBrightness(brightness);
      break;
    case IR_VOLPLUS:
      if (brightness < MAX_BRIGHTNESS) {
        brightness += 10;
        FastLED.setBrightness(brightness);
      }
      break;
    case IR_VOLMIN:
      if (brightness > 0) {
        brightness -= 10;
        FastLED.setBrightness(brightness);
      }
      break;
  }

  return -1;
}

void loop() {
  //Read IR
  readIR();
 
  switch(mode){
    case 0: loopRainbow(real, NUM_LEDS); break;
    case 1: loopNoise(real, NUM_LEDS); break;
    case 2: loopWazePic(real, NUM_LEDS); break;
    case 3: loopWade(real, NUM_LEDS); break;
  }

  adjustLeds();
  FastLED.show();
  FastLED.delay(5);
}




void adjustLeds() {
  int flipcolumns[] = {1,2,4,5,6,7,12,14,15};
  //memcpy(real, leds, sizeof(CRGB)*NUM_LEDS);
  flip(flipcolumns);
}

void flip(int columns[]) {
  for (int i = 0; i < 9; i++) flip(columns[i]);
}

void flip(int column) {
  flip(15*column, 15 * (column + 1) -1);
}

void flip(int start, int end) {
  CRGB tmp;
  for (int i = 0; start + i < end - i; i++) {
    tmp = real[start + i];
    real[start + i] = real[end - i];
    real[end - i] = tmp;
  }
}
