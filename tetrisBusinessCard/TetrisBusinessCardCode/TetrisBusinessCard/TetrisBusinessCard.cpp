#include <stdio.h>
#include <cstring>
#include "pico/stdlib.h"
#include "charlieplexDriver.cpp"

const uint32_t GPIO_PIN_COUNT = 27;
const uint32_t LED_PIN_COUNT = 22;
const uint32_t LEDMask = 0x3FFFFF;
const uint32_t GPIOMask =  0x2FFFFFF;

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1
void uartInit(){
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
    uart_init(UART_ID, BAUD_RATE);
}

void testPatterns(charlieplexDriver &driver) {
    // Pattern 1: All Off
    static bool allOff[22][21] = {false};

    // Pattern 2: All On
    static bool allOn[22][21] = {true};

    // Pattern 3: Single LED
    static bool singleLed[22][21] = {false};
    singleLed[0][0] = true;

    // Pattern 4: Last LED
    static bool lastLed[22][21] = {false};
    lastLed[21][20] = true;

    // Pattern 5: First Row On
    static bool firstRowOn[22][21] = {false};
    for (int j = 0; j < 21; ++j) {
        firstRowOn[0][j] = true;
    }

    // Pattern 6: First Column On
    static bool firstColumnOn[22][21] = {false};
    for (int i = 0; i < 22; ++i) {
        firstColumnOn[i][0] = true;
    }

    // Pattern 7: Checkerboard
    static bool checkerboard[22][21] = {false};
    for (int i = 0; i < 22; ++i) {
        for (int j = 0; j < 21; ++j) {
            checkerboard[i][j] = (i + j) % 2 == 0;
        }
    }

    // Pattern 8: Border Only
    static bool borderOnly[22][21] = {false};
    for (int j = 0; j < 21; ++j) {
        borderOnly[0][j] = true;       // Top border
        borderOnly[21][j] = true;     // Bottom border
    }
    for (int i = 0; i < 22; ++i) {
        borderOnly[i][0] = true;      // Left border
        borderOnly[i][20] = true;     // Right border
    }

    // Array of test patterns
      bool (*patterns[])[22][21] = {&allOff, &allOn, &singleLed, &lastLed, &firstRowOn, &firstColumnOn, &checkerboard, &borderOnly};
    const char *patternNames[] = {
        "All Off",
        "All On",
        "Single LED",
        "Last LED",
        "First Row On",
        "First Column On",
        "Checkerboard",
        "Border Only"};

    // Display each pattern for 2 seconds
    for (int i = 0; i < 8; ++i) {
        printf("Displaying Pattern: %s\n", patternNames[i]);
        driver.writeFrame(*patterns[i]);
        sleep_ms(2000); // Display for 2 seconds
    }
}

int main() {
    charlieplexDriver driver;
    uartInit();
    testPatterns(driver);

}
