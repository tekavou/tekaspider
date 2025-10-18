/*
   Prompt: Spider web LED animation with blink at completion
   Behavior:
   - Starts all purple
   - A moving LED of the *next color* sweeps across and fills the strip
   - Once full, that color blinks 5 times before transitioning
   - Color cycle: Purple → Orange → Blue → Green → (repeat)
   Hardware:
   - Seeed Studio XIAO
   - WS2812 LED strip, 43 LEDs, data on D5
*/

#include <Adafruit_NeoPixel.h>

#define LED_PIN    D5
#define LED_COUNT  43

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t colors[] = {
  Adafruit_NeoPixel::Color(80, 0, 80),    // purple
  Adafruit_NeoPixel::Color(255, 50, 0),   // orange
  Adafruit_NeoPixel::Color(0, 0, 150),    // blue
  Adafruit_NeoPixel::Color(0, 150, 0)     // green
};
int numColors = 4;

int currentBase = 0;   // current background color index
int nextColor = 1;     // moving color index
int spiderPos = 0;     // current moving LED position
int lockedCount = 0;   // how many LEDs have been filled with nextColor

void setup() {
  strip.begin();
  strip.setBrightness(100);
  resetStrip();
}

void loop() {
  strip.clear();

  uint32_t baseColor = colors[currentBase];
  uint32_t moveColor = colors[nextColor];

  // Draw base color
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, baseColor);
  }

  // Locked LEDs at the END
  for (int i = 0; i < lockedCount; i++) {
    strip.setPixelColor(LED_COUNT - 1 - i, moveColor);
  }

  // Moving LED
  if (spiderPos < LED_COUNT - lockedCount) {
    strip.setPixelColor(spiderPos, moveColor);
  }

  strip.show();
  delay(30);
  spiderPos++;

  // Reached end of section?
  if (spiderPos >= LED_COUNT - lockedCount) {
    lockedCount++;
    spiderPos = 0;
    delay(300);

    // Whole strip done?
    if (lockedCount >= LED_COUNT) {
      blinkColor(moveColor, 5, 150);  // blink 5 times before next color

      // Next cycle setup
      currentBase = nextColor;
      nextColor = (nextColor + 1) % numColors;
      lockedCount = 0;
      spiderPos = 0;
      resetStrip();
      delay(1000);
    }
  }
}

// Blink the entire strip in a color a given number of times
void blinkColor(uint32_t color, int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    // Turn off
    strip.clear();
    strip.show();
    delay(delayMs);

    // Turn on with color
    for (int j = 0; j < LED_COUNT; j++) {
      strip.setPixelColor(j, color);
    }
    strip.show();
    delay(delayMs);
  }
}

void resetStrip() {
  strip.clear();
  uint32_t baseColor = colors[currentBase];
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, baseColor);
  }
  strip.show();
}