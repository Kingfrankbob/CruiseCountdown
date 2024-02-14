#include <Adafruit_NeoPixel.h>
#include "RTClib.h"

#define LED_PIN 6
#define LED_COUNT 172
#define BRIGHTNESS 135

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
RTC_PCF8523 rtc;

DateTime previousTime = DateTime();

uint32_t firstPixelHue = 1;
bool binaryCounter[8] = {0};
unsigned long timeCounter = 0;

void setup() {
  strip.begin();
  rtc.begin();
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
  strip.setBrightness(BRIGHTNESS);
  setBinaryCounter();
  displayStrip();
  Serial.begin(9600);
}

void loop() {
  DateTime now = rtc.now();

  if (now.day() != previousTime.day() || now.month() != previousTime.month()) {
    setBinaryCounter();
  }

  if (millis() - timeCounter > 10) {
    timeCounter = millis();
    displayStrip();
  }
}

void setBinaryCounter() {
  DateTime now = rtc.now();
  DateTime targetDate = DateTime(2024, 5, 19, 0, 0, 0);
  int daysUntilTarget = (targetDate - now).days();

  int index = 0;
  while (daysUntilTarget > 0 && index < 8) {
    if (daysUntilTarget % 2 == 1) {
      binaryCounter[index] = 1;
    }
    daysUntilTarget >>= 1; // Right shift to divide by 2
    index++;
  }
}

void displayStrip()
{
  int section = 0;
  int everyOther = 0;
  int lightCounter = 0;

  for (int i = 0; i < LED_COUNT / 2; i++) {
    int reversedIndex = LED_COUNT - 1 - i;
    if (binaryCounter[section]) {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      strip.setPixelColor(reversedIndex, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    else {
      strip.setPixelColor(i, 0, 0, 0);
      strip.setPixelColor(reversedIndex, 0, 0, 0);
    }
    lightCounter++;
    everyOther++;
    if (everyOther % 20 == 0)
    {
      lightCounter--;
    }
    if (lightCounter % 10 == 0)
    {
      section++;
    }
    if (lightCounter % 10 == 1)
    {
      strip.setPixelColor(i, 255, 255, 255);
      strip.setPixelColor(reversedIndex, 255, 255, 255);
    }
  }
  strip.show();
  firstPixelHue += 300;
}
