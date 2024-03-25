#include <FastLED.h>

#define WIDTH 9
#define HEIGHT 9
#define NUM_LEDS (WIDTH * HEIGHT)
#define DATA_PIN 16
#define MAX_BRIGHTNESS 100
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
char buffer[100];

#define ATOPILE_ORANGE 0xFFFF00

const bool imagePattern[HEIGHT][WIDTH] = {
    {false, true, false, true, true, false, true, true, true},
    {false, false, false, false, true, false, false, false, true},
    {false, true, true, false, true, true, true, false, true},
    {false, false, true, true, false, false, true, false, false},
    {true, true, false, true, true, false, true, true, true},
    {false, true, false, false, true, false, false, false, true},
    {false, true, true, false, true, true, true, false, true},
    {false, false, true, true, false, false, false, false, true},
    {true, true, false, true, true, true, true, true, true}};

uint16_t XY(uint8_t x, uint8_t y)
{
    return (y * WIDTH) + x;
}

void displayImage()
{
    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            leds[XY(col, row)] = imagePattern[row][col] ? ATOPILE_ORANGE : false;
        }
    }
    FastLED.show();
}

void setup()
{
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(MAX_BRIGHTNESS);
    displayImage();
    Serial.begin(115200);
    pinMode(0, INPUT);
}

void wavePulse()
{
    static long startTime = millis();
    long currentTime = millis();
    float waveSpeed = 0.25; // controls the speed of the wave

    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            if (imagePattern[row][col])
            {
                // Calculate the distance from the center of the wave
                float distance = sqrt(sq(row - HEIGHT / 2.0) + sq(col - WIDTH / 2.0));
                // Calculate the wave pattern based on the distance
                float wavePattern = sin(distance * waveSpeed - (currentTime - startTime) * 0.002);
                // Map the wave pattern to a brightness value
                uint8_t brightness = map((wavePattern + 1) * 127.5, 0, 255, 0, MAX_BRIGHTNESS);
                // Set the LED color with the new brightness value
                leds[XY(col, row)] = CHSV(23, 255, brightness);
                // sprintf(buffer, "row: %d, col: %d, distance: %f, wavePattern: %f, brightness: %d\n", row, col, distance, wavePattern, brightness);
            }
            else
            {
                leds[XY(col, row)] = false;
                // sprintf(buffer, "row: %d, col: %d\n", row, col);
            }
            // Serial.print(buffer);
        }
        // Serial.println();
    }
    // Serial.println();
    // Serial.println();
    // Serial.println();
    FastLED.show();
    delay(15); // You can adjust this for a faster or slower wave
}

void loop()
{
    wavePulse();
    if (millis() > 3600 * 1000)
    {
        FastLED.setBrightness(0);
    }
}
