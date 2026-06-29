#include <stdio.h>
#include <cstring>
#include <queue>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "charlieplexDriver.cpp"
#include "shape.cpp"
#include "playfield.cpp"
#include "matrix.cpp"
#include "button.cpp"
#include "externalControl.cpp"

const uint32_t targetFrameUs = 1000;
const bool selfLearning = false; //use this if hooked up to a Pi running the genetic algorithm code
int frameCount = 0;
int scrollStep = 0;
bool playing = true;

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

static std::queue<Action> actionQueue;
std::array<char, 3> initials;
std::array<Scorecard, 3> savedScores = {
    Scorecard({'A', 'A', 'A'}, 0),
    Scorecard({'A', 'A', 'A'}, 0),
    Scorecard({'A', 'A', 'A'}, 0)};
bool scoresLoaded = true;
Button upButton(24);
Button leftButton(25);
Button downButton(26);
Button rightButton(27);
Button aButton(28);
Button bButton(29);

void uartInit()
{
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
    uart_init(UART_ID, BAUD_RATE);

    if(selfLearning){
        // //the next bit of setup is taken from uart_advanced.c demo code

        // // Set UART flow control CTS/RTS, we don't want these, so turn them off
        // uart_set_hw_flow(UART_ID, false, false);

        // // Set our data format
        // uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

        // // Turn off FIFO's - we want to do this character by character
        // uart_set_fifo_enabled(UART_ID, false);

        // // Set up a RX interrupt
        // // We need to set up the handler first
        // // Select correct interrupt for the UART we are using
        // int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
        // //TODO:Write interrupt handler
        // // And set up and enable the interrupt handlers
        // irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
        // irq_set_enabled(UART_IRQ, true);

        // // Now enable the UART to send interrupts - RX only
        // uart_set_irq_enables(UART_ID, true, false);

        // //now we should be able to use uart_puts(UART_ID, message) to communicate with the PI
    }
}

void setCoreSpeed()
{
    set_sys_clock_khz(18000, true);
}

template <size_t Rows, size_t Cols>
void printGrid(const std::array<std::array<bool, Cols>, Rows> &grid, const std::string &title)
{
    std::cout << title << ":" << std::endl;
    for (size_t i = 0; i < Rows; ++i)
    {
        for (size_t j = 0; j < Cols; ++j)
        {
            std::cout << (grid[i][j] ? "X" : ".") << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void testPatterns(charlieplexDriver &driver)
{
    // Pattern 1: All Off
    static std::array<std::array<bool, 21>, 22> allOff = {false};

    // Pattern 2: All On
    static std::array<std::array<bool, 21>, 22> allOn = {{true}};

    // Pattern 3: Single LED
    static std::array<std::array<bool, 21>, 22> singleLed = {false};
    singleLed[0][0] = true;

    // Pattern 4: Last LED
    static std::array<std::array<bool, 21>, 22> lastLed = {false};
    lastLed[21][20] = true;

    // Pattern 5: First Row On
    static std::array<std::array<bool, 21>, 22> firstRowOn = {false};
    for (int j = 0; j < 21; ++j)
    {
        firstRowOn[0][j] = true;
    }

    // Pattern 6: First Column On
    static std::array<std::array<bool, 21>, 22> firstColumnOn = {false};
    for (int i = 0; i < 22; ++i)
    {
        firstColumnOn[i][0] = true;
    }

    // Pattern 7: Checkerboard
    static std::array<std::array<bool, 21>, 22> checkerboard = {false};
    for (int i = 0; i < 22; ++i)
    {
        for (int j = 0; j < 21; ++j)
        {
            checkerboard[i][j] = (i + j) % 2 == 0;
        }
    }

    // Pattern 8: Border Only
    static std::array<std::array<bool, 21>, 22> borderOnly = {false};
    for (int j = 0; j < 21; ++j)
    {
        borderOnly[0][j] = true;  // Top border
        borderOnly[21][j] = true; // Bottom border
    }
    for (int i = 0; i < 22; ++i)
    {
        borderOnly[i][0] = true;  // Left border
        borderOnly[i][20] = true; // Right border
    }

    static std::array<std::array<bool, 21>, 22> diagonal = {false};
    for (int i = 0; i < 22; ++i)
    {
        for (int j = 0; j < 21; ++j)
        {
            if (i == j)
            {
                diagonal[i][j] = true;
            }
        }
    }

    // Array of test patterns
    std::array<std::array<bool, 21>, 22>(*patterns[]) = {&allOff, &allOn, &singleLed, &lastLed, &firstRowOn, &firstColumnOn, &checkerboard, &borderOnly, &diagonal};
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
    for (int i = 0; i < 9; ++i)
    {
        driver.writeFrame(*patterns[i]);
        printGrid<22, 21>(*patterns[i], patternNames[i]);
        sleep_ms(2000); // Display for 2 seconds
    }
}

bool checkCollision(Playfield &playfield, Shape &shape)
{
    auto field = playfield.getPlayfield();
    auto tetromino = shape.getShape();
    int x = shape.x;
    int y = shape.y;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (tetromino[i][j])
            {
                int newX = x + j;
                int newY = y + i;

                // Check boundaries
                if (newX < 0 || newX >= 10 || newY < 0 || newY >= 21)
                {
                    return true;
                }

                // Check for existing blocks
                if (field[newY][newX])
                {
                    return true;
                }
            }
        }
    }
    return false;
}

int clearLines(Playfield *playfield)
{
    auto field = playfield->getPlayfield();
    int rowsRemoved = 0;
    const int ROWS = 21;
    const int COLS = 10;
    int currentRow = ROWS - 1;

    while (currentRow >= 0)
    {
        bool rowIsFull = true;
        for (int col = 0; col < COLS; ++col)
        {
            if (!field[currentRow][col])
            {
                rowIsFull = false;
                break;
            }
        }

        if (rowIsFull)
        {
            for (int row = currentRow; row > 0; --row)
            {
                for (int col = 0; col < COLS; ++col)
                {
                    field[row][col] = field[row - 1][col];
                }
            }
            for (int col = 0; col < COLS; ++col)
            {
                field[0][col] = false;
            }

            rowsRemoved++;
        }
        else
        {
            currentRow--;
        }
    }
    playfield->setPlayfield(field);

    if (rowsRemoved == 4)
    {
        rowsRemoved += 2;
    }

    return rowsRemoved;
}

int lockShape(Playfield *playfield, Shape &shape)
{
    auto tetromino = shape.getShape();
    int x = shape.x;
    int y = shape.y;
    auto field = playfield->getPlayfield();
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (tetromino[i][j])
            {
                int row = y + i;
                int col = x + j;

                if (row >= 0 && row < 21 && col >= 0 && col < 10)
                {
                    field[row][col] = true;
                }
            }
        }
    }
    printGrid<21, 10>(field, "updated field");
    playfield->setPlayfield(field);
    return clearLines(playfield);
}

void resetGame(Playfield &field)
{
    frameCount = 0;
    Shape *newInPlay = new Shape();
    Shape *nextShape = new Shape();
    field.reset(newInPlay, nextShape);
    playing = true;
}

void swapShapeAndCheckCollision(Playfield *playfield, Shape *shape, bool &retFlag)
{
    retFlag = true;
    Shape *swappedShape = playfield->swapShape(shape);

    swappedShape->x = shape->x;
    swappedShape->y = shape->y;

    if (checkCollision(*playfield, *swappedShape))
    {
        bool moved = false;

        for (int i = 1; i <= 2; i++)
        {
            swappedShape->x = shape->x - i;
            if (!checkCollision(*playfield, *swappedShape))
            {
                moved = true;
                break;
            }
        }

        if (!moved)
        {
            swappedShape->x = shape->x;
        }

        if (!moved)
        {
            for (int i = 1; i <= 2; i++)
            {
                swappedShape->x = shape->x + i;
                if (!checkCollision(*playfield, *swappedShape))
                {
                    moved = true;
                    break;
                }
            }
        }

        if (!moved)
        {
            delete swappedShape;
            return;
        }
    }

    *shape = *swappedShape;
    delete swappedShape;
    retFlag = false;
}

bool isHardDropping;
void hardDrop(Playfield *playfield, Shape *shape)
{
    if (isHardDropping)
    {
        actionQueue.push(DOWN);
        if (checkCollision(*playfield, *shape))
        {
            isHardDropping = false;
        }
    }
}

void stepGame(Shape *shape, Playfield *playfield, Score *scoreGrid)
{
    while (!actionQueue.empty())
    {
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
        case UP:
            bool retFlag;
            swapShapeAndCheckCollision(playfield, shape, retFlag);
            if (retFlag)
                return;
            break;
        case DOWN:
            shape->y++;
            break;
        case A: // rotate
            shape->rotateClockwise();
            break;
        case B:
            isHardDropping = true;
            break;
        default:
            break;
        }
        if (isHardDropping)
        {
            hardDrop(playfield, shape);
        }

        if (checkCollision(*playfield, *shape))
        {
            if (action == DOWN)
            {
                shape->y--;
                scoreGrid->addScore(lockShape(playfield, *shape));
                scoreGrid->updateScoreGrid();
                *shape = *playfield->getNextShape();
                playfield->createNextShape();

                if (checkCollision(*playfield, *shape))
                {
                    playing = false;
                }
            }
            else if (action == A)
            {
                shape->rotateCounterClockwise();
            }
            else if (action == B)
            {
                shape->rotateClockwise();
            }
            else
            {
                shape->x = oldX;
                shape->y = oldY;
            }
        }




        
    }
}

void showScoreCards(Matrix &matrix, charlieplexDriver &driver, std::array<Scorecard, 3> &Scorecards)
{
    if (frameCount % 240 == 0)
    {
        scrollStep++;
    }
    matrix.reset();
    matrix.mapScorecard(Scorecards[0], 0, 2, scrollStep);
    matrix.mapScorecard(Scorecards[1], 0, 8, scrollStep);
    matrix.mapScorecard(Scorecards[2], 0, 14, scrollStep);
    auto frame = matrix.getMatrix();
    driver.writeFrame(frame);
    frameCount++;
    if (scrollStep > 10)
    {
        scrollStep = 0;
    }
}

void setInitials(Matrix &matrix, charlieplexDriver &driver)
{
    initials = {'A', 'A', 'A'};
    int index = 0;
    while (true)
    {
        while (!actionQueue.empty())
        {
            Action action = actionQueue.front();
            actionQueue.pop();
            switch (action)
            {
            case LEFT:
                if (index > 0)
                {
                    index--;
                }
                break;
            case RIGHT:
                if (index < 2)
                {
                    index++;
                }
                else if (index == 2)
                {
                    return;
                }
                break;
            case UP:
                if (initials[index] < 'Z')
                {
                    initials[index]++;
                }
                break;
            case DOWN:
                if (initials[index] > 'A')
                {
                    initials[index]--;
                }
                break;
            case A:
            case B:
                if (index == 2)
                {
                    return;
                }
            default:
                break;
            }
        }
        matrix.reset();
        matrix.mapCaret(14 - 6 * index, -4);
        matrix.mapInitials(initials, 2, 7, false);
        auto frame = matrix.getMatrix();
        driver.writeFrame(frame);
    }
}

void autoStepGame(Score &scoreGrid)
{
    if (frameCount > (150 - (scoreGrid.getScore() * 1.5)))
    {
        if (actionQueue.front() != DOWN || actionQueue.empty())
        {
            actionQueue.push(DOWN);
        }
        frameCount = 0;
    }
}

void playTetris(Shape &tetromino, Playfield &playfield, Score &scoreGrid, Matrix &matrix, charlieplexDriver &driver)
{

    while (playing)
    {
        if(selfLearning && uart_is_readable(UART_ID)){
            //TODO: read and process UART message
            // actionQueue.push(externalControl::decodeMessage());
        }
        stepGame(&tetromino, &playfield, &scoreGrid);
        matrix.reset();
        matrix.mapPlayfield(&playfield);
        matrix.mapShape(&tetromino);
        matrix.mapHeldShape(playfield.getHeldShape());
        matrix.mapNextShape(playfield.getNextShape());
        matrix.mapPlayFieldIndicator();
        matrix.mapScore(&scoreGrid);
        auto frame = matrix.getMatrix();
        driver.writeFrame(frame);
        frameCount++;

        if(!selfLearning){
            autoStepGame(scoreGrid);
        }else {
            int heldShape = playfield.hasHeldShape() ? playfield.getHeldShape()->shapeChoice : -1;
            int nextShape = playfield.getNextShape()->shapeChoice;
            int currentShape = playfield.getCurentShape()->shapeChoice;
            // std::string message = externalControl::createMessage(&playfield, currentShape, heldShape, nextShape);
            // externalControl::sendMessage(message);
        }
    }

    bool waitForNextGame = true;

    if(selfLearning){
        // externalControl::sendMessage("GAME_OVER");
    }
    else {
        //Scorecard::ClearFlashScores();
        savedScores = Scorecard::readFromFlash();
        // scoreGrid.addScore(15);
        if (scoreGrid.getScore() > Scorecard::getLowestScore(savedScores))
        {
            setInitials(matrix, driver);
            Scorecard newCard(initials, scoreGrid.getScore());
            Scorecard::insertScore(savedScores, newCard);
            Scorecard::saveToFlash(savedScores);
        }
        savedScores = Scorecard::readFromFlash();
    }

    while (waitForNextGame)
    {
        if(!selfLearning){
            showScoreCards(matrix, driver, savedScores);
        }

        if (!actionQueue.empty())
        {
            actionQueue.pop();
            resetGame(playfield);
            scoresLoaded = false;
            scoreGrid.resetScore();
            waitForNextGame = false;
        }
    }
}

void initButtons()
{

    upButton.onPress([]()
                    { actionQueue.push(UP); });
    leftButton.onPress([]()
                    { actionQueue.push(LEFT); });
    downButton.onPress([]()
                    { actionQueue.push(DOWN); });
    rightButton.onPress([]()
                    { actionQueue.push(RIGHT); });
    aButton.onPress([]()
                    { actionQueue.push(A); });
    bButton.onPress([]()
                    { actionQueue.push(B); });
}

int main()
{
    charlieplexDriver driver;
    if (selfLearning) {
        uartInit();
    }else {
        setCoreSpeed();
        // testPatterns(driver);
        initButtons();
    }

    Shape tetromino;
    Playfield playfield(&tetromino);
    Score scoreGrid;
    Matrix matrix;
    scoreGrid.updateScoreGrid();
    playfield.createNextShape();
    while(true){
        playTetris(tetromino, playfield, scoreGrid, matrix, driver);
    }
}
