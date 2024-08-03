// Created: 01-08-2024 12:00 PM
// Project: LED Comet with Sound Sensor
// Description: A program to create a comet effect on an LED strip with a sound sensor
// Last updated: 02-08-2024 7:00 PM
// Author: ANOOF CHAPPANGATHIL
// DEADLINE : 04-08-2024 11:59 PM


#define DEBUG 0
// Define the LED strip parameters
#define LED_PIN    D4       // Digital pin for the LED strip
#define LED_COUNT  80       // Number of LEDs in the strip
#define COMET_SIZE 8        // Number of pixels in a comet
#define BR_FACTOR  3.0      // Brightness factor for comets
#define DELAY_MS   20       // Delay between updates
#define MAX_COMETS 15        // Maximum number of comets
#define SENSOR_PIN A0       // Analog pin for the sensor
#define SENS_THRESHOLD 500  // Threshold for sensor activation
#define DEBOUNCE_DELAY 100  // Delay between sensor activations


#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

Adafruit_NeoPixel led_strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);  // Initialize the LED strip
uint32_t lastUpdateTime = 0;        // Last time the strip was updated
uint32_t lastUpdateTimeDelay = 0;   // Last time the sensor was read

// Define the comet structure
struct Comet {
    int position;       // Position of the comet
    uint8_t r, g, b;    // Color of the comet
    bool active;        // Whether the comet is active
};

Comet comets[MAX_COMETS];   // Array of comets

// Read the sensor and activate a comet if the sensor value is above the threshold
void readSensor() {     
    int sensorValue = analogRead(SENSOR_PIN);   // Read the sensor value
    if (DEBUG) { Serial.println("Sensor value: " + String(sensorValue)); } // if debug is enabled, print the sensor value
    if (sensorValue > SENS_THRESHOLD) {         // If the sensor value is above the threshold
        if (millis() - lastUpdateTime < DEBOUNCE_DELAY) {   // Check for debounce delay
            return;                                         // If debounce delay is not over, return
        }
        lastUpdateTime = millis();                          // Update the last update time
        for (int i = 0; i < MAX_COMETS; i++) {              // Loop through the comets array
            if (!comets[i].active) {                        // Find an inactive comet
                comets[i].position = 0;                     // Set the position of the comet
                comets[i].r = random(0, 256);               // Set the color of the comet
                comets[i].g = random(0, 256);               // Set the color of the comet
                comets[i].b = random(0, 256);               // Set the color of the comet
                comets[i].active = true;                    // Activate the comet
                break;                                      // Exit the loop
            }
            
        }
    }
}

void updateComets() {
    if (millis() - lastUpdateTimeDelay < DELAY_MS) {        // Check for delay between updates
        return;                                             // If delay is not over, return
    }
    lastUpdateTimeDelay = millis();                         // Update the last update time

    // Clear all pixels
    for (int i = 0; i < LED_COUNT; i++) {                   // Loop through all pixels
        led_strip.setPixelColor(i, 0, 0, 0);                // Set the pixel color to black
    }

    // Update each active comet
    for (int i = 0; i < MAX_COMETS; i++) {                  // Loop through the comets array
        if (comets[i].active) {                             // Check if the comet is active
            for (int j = 0; j < COMET_SIZE; j++) {          // Loop through the comet size
                int pos = comets[i].position - j;           // Calculate the position of the comet
                if (pos >= 0 && pos < LED_COUNT) {          // Check if the position is within the LED strip
                    // Calculate brightness factor
                    float brightnessFactor = pow(1.0 - static_cast<float>(j) / COMET_SIZE, BR_FACTOR);   // Calculate the brightness factor
                    uint8_t r = comets[i].r * brightnessFactor;
                    uint8_t g = comets[i].g * brightnessFactor;
                    uint8_t b = comets[i].b * brightnessFactor;
                    led_strip.setPixelColor(pos, r, g, b);  // Set the pixel color with adjusted brightness
                }
            }
            comets[i].position++;                                   // Update the position of the comet
            if (comets[i].position - COMET_SIZE >= LED_COUNT) {     // Check if the comet has reached the end of the strip
                comets[i].active = false;                           // Deactivate the comet
            }
        }
    }
    
    led_strip.show();                                               // Show the updated strip
}
// READ SERIAL 
void readSerial (){
    if 
}


void setup() {
    Serial.begin(9600);                         // Initialize serial communication
    
    led_strip.begin();                          // Initialize the LED strip
    led_strip.show();                           // Initialize all pixels to 'off'
    // randomSeed(analogRead(A0)); // Seed the random number generator

    // Initialize comets
    for (int i = 0; i < MAX_COMETS; i++) {          // Loop through the comets array
        comets[i].active = false;                   // Set the comet as inactive
    }

}

void loop() {
    if (millis() - lastUpdateTimeDelay < DELAY_MS) {        // Check for delay between updates
        return;                                             // If delay is not over, return
    }
    readSensor();                                           // Read the sensor
    updateComets();                                         // Update the comets
    lastUpdateTimeDelay = millis();                         // Update the last update time
    // delay(DELAY_MS);                                         
}

