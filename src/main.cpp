// UNDER DEVELOPMENT
// NOT FINALIZED

// Last updated: 01-08-2024 06:53 PM
// Author: ANOOF CHAPPANGATHIL
// DEADLINE : 04-08-2024 11:59 PM


#define DEBUG 1

#define RED 255,0,0
#define GREEN 0,255,0
#define BLUE 0,0,255
#define WHITE 255,255,255
#define YELLOW 255,255,0
#define CYAN 0,255,255
#define ORANGE 255,125,0
#define PINK 255,0,125
#define TURQUOISE 0,255,125
#define LIME 125,255,0
#define MAGENTA 255,0,255
#define INDIGO 0,125,255


#define SENSOR_PIN      A0
#define LED_PIN         D4
#define SENS_THRESHOLD  600

#define LED_COUNT 74
#define LED_RUN_DELAY_ms 2000000

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

struct RGB {
    int r, g, b;
};


RGB colors[] = {
        {RED},
        {GREEN},
        {BLUE},
        {WHITE},
        {YELLOW},
        {CYAN},
        {ORANGE},
        {PINK},
        {TURQUOISE},
        {LIME},
        {MAGENTA},
        {INDIGO}
    };

Adafruit_NeoPixel led_strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

uint16_t sensorValue = 0;
uint32_t lastUpdateTime = 0;
uint8_t colorIndex;


// void neopixel (uint8_t r, uint8_t g, uint8_t b) {
//   for (int i = 0; i < led_strip.numPixels(); i++) {
//     if (micros() - lastUpdateTime < LED_RUN_DELAY_ms) {
//       return;
//     }
//     int8_t j = i+1;
//     int8_t k = i-2;
//     led_strip.setPixelColor(i, r, g, b);
//     led_strip.show();
//   }
  
// }

#define COMET_SIZE 5
#define DELAY_MS   50

uint32_t current_led_number_1 = 0;

uint32_t last_updated_time_for_strip_1 = 0;

#define NUMBER_OF_ON_LED_FOR_STRIP_1 4
#define NUMBER_OF_OFF_LED_FOR_STRIP_1 8
#define TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_1_IN_us 70000

void sun_run_led(uint8_t r, uint8_t g, uint8_t b)
{
  if (micros() - last_updated_time_for_strip_1 < TIME_DELAY_FOR_RUNNING_SPEED_FOR_STRIP_1_IN_us) {
    return;
  }
  if (current_led_number_1 < (LED_COUNT + NUMBER_OF_ON_LED_FOR_STRIP_1)) {
    current_led_number_1 += 1;
  } else {
    current_led_number_1 = (LED_COUNT + NUMBER_OF_ON_LED_FOR_STRIP_1) - (NUMBER_OF_ON_LED_FOR_STRIP_1 + NUMBER_OF_OFF_LED_FOR_STRIP_1 - 1);
  }
  for (uint16_t k = 1; k <= current_led_number_1; k++) {
    led_strip.setPixelColor(k, led_strip.Color(0, 0, 0));
  }
  for (int32_t j = current_led_number_1; j > 0; j -= (NUMBER_OF_ON_LED_FOR_STRIP_1 + NUMBER_OF_OFF_LED_FOR_STRIP_1))
  {
    for (int32_t i = j; i > (j - NUMBER_OF_ON_LED_FOR_STRIP_1); i--)
    {
      uint16_t m = i; if (m < 1) {
        m = 1;
      }
            uint8_t red = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),r,0);
            uint8_t green = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),g,0);
            uint8_t blue = map(m,j,(j- NUMBER_OF_ON_LED_FOR_STRIP_1),b,0);
      led_strip.setPixelColor(m, led_strip.Color(r, g, b));
    }
  }
  led_strip.show();
  last_updated_time_for_strip_1 = micros();
}


void cometEffect(uint8_t r, uint8_t g, uint8_t b) {
  if (micros() - lastUpdateTime < LED_RUN_DELAY_ms) {
      return;
    }
  lastUpdateTime = micros();
  for (int i = 0; i < led_strip.numPixels() + COMET_SIZE; i++) {
    
    
      // Clear all pixels
      for (int j = 0; j < led_strip.numPixels(); j++) {
          led_strip.setPixelColor(j, 0, 0, 0);
      }

      // Set comet pixels
      for (int j = 0; j < COMET_SIZE; j++) {
          if ((i - j) >= 0 && (i - j) < led_strip.numPixels()) {
              led_strip.setPixelColor(i - j, r, g, b);
              delay(DELAY_MS);
          }
      }

      // Show the updated strip
      led_strip.show();
      
  }
}

void selectColor() {
  colorIndex = random(0, sizeof(colors) / sizeof(colors[0]));
  if (DEBUG) { Serial.println("Color index: " + String(colorIndex));}
}

void readSensor() {
  sensorValue = analogRead(SENSOR_PIN);
  if (DEBUG) { Serial.println("Sensor value: " + String(sensorValue));}
  if (sensorValue > SENS_THRESHOLD) {
    selectColor();
    // neopixel(colors[colorIndex].r, colors[colorIndex].g, colors[colorIndex].b);
    // cometEffect(colors[colorIndex].r, colors[colorIndex].g, colors[colorIndex].b);
    sun_run_led(colors[colorIndex].r, colors[colorIndex].g, colors[colorIndex].b);
    // to do
  } 
}


void setup() {
  Serial.begin(9600);
  led_strip.begin();
  led_strip.clear();
  led_strip.show();

}

void loop() {
  readSensor();
  if (DEBUG) { Serial.println("Looping...");}

}
