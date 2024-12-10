#include <stdio.h>
#include "pico/stdlib.h"

const uint32_t GPIO_PIN_COUNT = 27;
const uint32_t LED_PIN_COUNT = 22;
const uint32_t LEDMask = 0x3FFFFF;
const uint32_t GPIOMask =  0x2FFFFFF;

void init(){
    stdio_init_all();
    gpio_init_mask(GPIOMask);
}

int writeFrame(uint32_t frame[]){

}

uint32_t expandRow(uint32_t row){

}