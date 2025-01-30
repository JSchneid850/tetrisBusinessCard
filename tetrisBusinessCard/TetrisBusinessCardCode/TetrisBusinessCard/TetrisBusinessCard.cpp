#include <stdio.h>
#include <cstring>
#include "pico/stdlib.h"
#include "charlieplexDriver.cpp"
#include "shape.cpp"
#include "playfield.cpp"
#include "matrix.cpp"
#include "button.cpp"

const uint32_t GPIO_PIN_COUNT = 27;
const uint32_t LED_PIN_COUNT = 22;
const uint32_t LEDMask = 0x3FFFFF;
const uint32_t GPIOMask =  0x2FFFFFF;

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

Shape startingShape;
Playfield field(&startingShape);

void uartInit(){
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
    uart_init(UART_ID, BAUD_RATE);}

template <size_t Rows, size_t Cols>
void printGrid(const std::array<std::array<bool, Cols>, Rows>& grid, const std::string& title) {
    std::cout << title << ":" << std::endl;
    for (size_t i = 0; i < Rows; ++i) {
        for (size_t j = 0; j < Cols; ++j) {
            std::cout << (grid[i][j] ? "X" : ".") << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void testPatterns(charlieplexDriver &driver) {
    // Pattern 1: All Off
    static std::array<std::array<bool, 21>, 22> allOff = {false};

    // Pattern 2: All On
    static std::array<std::array<bool, 21>, 22> allOn = {{true}};

    // Pattern 3: Single LED
    static 
    std::array<std::array<bool, 21>, 22> singleLed = {false};
    singleLed[0][0] = true;

    // Pattern 4: Last LED
    static std::array<std::array<bool, 21>, 22> lastLed = {false};
    lastLed[21][20] = true;

    // Pattern 5: First Row On
    static std::array<std::array<bool, 21>, 22> firstRowOn = {false};
    for (int j = 0; j < 21; ++j) {
        firstRowOn[0][j] = true;
    }

    // Pattern 6: First Column On
    static std::array<std::array<bool, 21>, 22> firstColumnOn = {false};
    for (int i = 0; i < 22; ++i) {
        firstColumnOn[i][0] = true;
    }

    // Pattern 7: Checkerboard
    static std::array<std::array<bool, 21>, 22> checkerboard = {false};
    for (int i = 0; i < 22; ++i) {
        for (int j = 0; j < 21; ++j) {
            checkerboard[i][j] = (i + j) % 2 == 0;
        }
    }

    // Pattern 8: Border Only
    static std::array<std::array<bool, 21>, 22> borderOnly = {false};
    for (int j = 0; j < 21; ++j) {
        borderOnly[0][j] = true;       // Top border
        borderOnly[21][j] = true;     // Bottom border
    }
    for (int i = 0; i < 22; ++i) {
        borderOnly[i][0] = true;      // Left border
        borderOnly[i][20] = true;     // Right border
    }

    static std::array<std::array<bool, 21>, 22> diagonal = {false};
    for(int i = 0; i < 22; ++i){
        for(int j = 0; j < 21; ++j){
            if (i == j){
                diagonal[i][j]=true;
            }
        }
    }

    // Array of test patterns
    std::array<std::array<bool, 21>, 22> (*patterns[]) = {&allOff, &allOn, &singleLed, &lastLed, &firstRowOn, &firstColumnOn, &checkerboard, &borderOnly, &diagonal};
    const std::string patternNames[] = {
        "All Off",
        "All On",
        "Single LED",
        "Last LED",
        "First Row On",
        "First Column On",
        "Checkerboard",
        "Border Only",
        "Diagonal"};

    // Display each pattern for 2 seconds
    for (int i = 0; i < 9; ++i) {
        driver.writeFrame(*patterns[i]);
        printGrid<22,21>(*patterns[i], patternNames[i]);
        sleep_ms(2000); // Display for 2 seconds
    }
}

void stepGame() {

}

int main() {
    charlieplexDriver driver;
    uartInit();
    //testPatterns(driver);
    Button upButton(24);
    Button leftButton(25);
    Button downButton(26);
    Button rightButton(27);
    Button aButton(28);
    Button bButton(29);

    upButton.onPress([]() { std::cout << "upPressed" << std::endl; });
    leftButton.onPress([]() { std::cout << "leftPressed" << std::endl; });
    downButton.onPress([]() { std::cout << "downPressed" << std::endl; });
    rightButton.onPress([]() { std::cout << "rightPressed" << std::endl; });
    aButton.onPress([]() { std::cout << "aPressed" << std::endl; });
    bButton.onPress([]() { std::cout << "bPressed" << std::endl; });

    while(true){

    }

    Shape tetromino;
    printGrid<4, 4>(tetromino.getShape(), "Initial Tetromino Shape");

    Playfield playfield(&tetromino);
    printGrid<21, 10>(playfield.getPlayfield(), "Initial Playfield");

    Matrix matrix;
    matrix.mapPlayfield(&playfield);
    matrix.mapShape(&tetromino);
    printGrid<22, 21>(matrix.getMatrix(), "Final Matrix (Playfield + Tetromino)");

    auto frame = matrix.getMatrix();
    
    driver.writeFrame(frame);

}
