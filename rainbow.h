#include <FastLED.h>
  
  CRGBPalette16 rainbowPalette;
  TBlendType    rainbowBlending;

  int colorIndex = 0; 
  
  void setupRainbow() {
    rainbowPalette = RainbowColors_p;
    rainbowBlending = LINEARBLEND;
  }

  void loopRainbow(CRGB* leds, int num) {
    int startColor = colorIndex;
    for( int i = 0; i < num; i++) {
      leds[i] = ColorFromPalette( rainbowPalette, startColor, 255, rainbowBlending);
      startColor += 2;
    }
    colorIndex++;
  }
