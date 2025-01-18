#include <stdio.h>
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
int main() {
    charlieplexDriver driver;
    uartInit();
    bool test[21][20] = {false};
    test[0][0] = true;
    test[20][19] = true;
    test[0][19] = true;
    test[20][0] = true;
    while(true){
    driver.writeFrame(test);
    }
    // uint32_t LEDTargetBit = 0b1;
    // uint32_t targetMask;
    // for(uint32_t i = 0; i<LED_PIN_COUNT; i++){
    //     for(uint32_t j = 0; j<LED_PIN_COUNT; j++){
    //         if (i==j){GDB never responded to an interrupt request. Trying to end session anyways
    //             j++;
    //         }
    //         targetMask = (LEDTargetBit<<i)|(LEDTargetBit<<j);
    //         gpio_set_dir_in_masked(LEDMask);
    //         gpio_set_dir_out_masked(targetMask);
    //         gpio_put_masked(targetMask, LEDTargetBit<<j);
    //         printf("target mask: %x, HiVal: %x\n",targetMask,LEDTargetBit<<j);
    //     },
    // }
}
