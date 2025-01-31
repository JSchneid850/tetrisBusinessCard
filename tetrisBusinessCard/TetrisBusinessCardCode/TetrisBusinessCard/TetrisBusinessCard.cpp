#include <stdio.h>
#include <cstring>
#include <queue>
#include "pico/stdlib.h"
#include "charlieplexDriver.cpp"
#include "shape.cpp"
#include "playfield.cpp"
#include "matrix.cpp"
#include "button.cpp"
#include "action.cpp"

const uint32_t GPIO_PIN_COUNT = 27;
const uint32_t LED_PIN_COUNT = 22;
const uint32_t LEDMask = 0x3FFFFF;
const uint32_t GPIOMask =  0x2FFFFFF;
const uint32_t targetFrameMs = 33; 

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

Shape startingShape;
Playfield field(&startingShape);
std::queue<Action> actionQueue;

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

bool checkCollision(Playfield& playfield, Shape& shape){
    auto field = playfield.getPlayfield();
    auto tetromino = shape.getShape();
    int x = shape.x;
    int y = shape.y;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (tetromino[i][j]) {
                int newX = x + i;
                int newY = y + j;

                // Check boundaries
                if (newX < 0 || newX >= 10 || newY < 0 || newY >= 21) {
                    return true;
                }

                // Check for existing blocks
                if (field[newX][newY]) {
                    return true;
                }
            }
        }
    }
    return false;

}

void lockShape(Playfield* playfield, Shape& shape){
    auto tetromino = shape.getShape();
    int x = shape.x;
    int y = shape.y;
    auto field = playfield ->getPlayfield();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (tetromino[i][j]) {
                field[y + i][x + j] = true;
            }
        }
    }
    printGrid<21, 10>(field, "updated field");
    playfield->setPlayfield(field);
}

void stepGame(Shape* shape, Playfield* playfield) {
    while (!actionQueue.empty()){
        Action action = actionQueue.front();
        actionQueue.pop(); 

        int oldX = shape->x;
        int oldY = shape->y;

        switch (action)
        {
        case LEFT:
            shape->x++;
            break;
        case RIGHT:
            shape->x--;
            break;
        case A://rotate
            /* code */
            break;
        case DOWN:
            shape->y++;
            break;
        default:
            break;
        }

        if (checkCollision(*playfield, *shape)){
            shape->x = oldX;
            shape->y = oldY;

            if(action == DOWN){
                lockShape(playfield, *shape);
                *shape = Shape();
            }
        }
    }
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

    upButton.onPress([]() { 
        actionQueue.push(UP); 
    });
    leftButton.onPress([]() { 
        actionQueue.push(LEFT); 
    });
    downButton.onPress([]() { 
        actionQueue.push(DOWN); 
    });
    rightButton.onPress([]() { 
        actionQueue.push(RIGHT); 
    });
    aButton.onPress([]() { 
        actionQueue.push(A); 
    });
    bButton.onPress([]() { 
        actionQueue.push(B); 
    });

    Shape tetromino;
    Playfield playfield(&tetromino);

    Matrix matrix;
    matrix.mapPlayfield(&playfield);
    matrix.mapShape(&tetromino);

    while(true){
        uint32_t frameStart = to_ms_since_boot(get_absolute_time());

        stepGame(&tetromino, &playfield);

        matrix.reset();
        matrix.mapPlayfield(&playfield);
        matrix.mapShape(&tetromino);

        auto frame = matrix.getMatrix();
        driver.writeFrame(frame);

        uint32_t frameTime = to_ms_since_boot(get_absolute_time()) - frameStart;

    }
}
