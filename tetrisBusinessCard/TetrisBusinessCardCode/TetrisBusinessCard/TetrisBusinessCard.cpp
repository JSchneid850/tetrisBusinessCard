#include <stdio.h>
#include "pico/stdlib.h"

const uint32_t GPIO_PIN_COUNT = 27;
const uint32_t LED_PIN_COUNT = 22;
const uint32_t LEDMask = 0x3FFFFF;
const uint32_t GPIOMask =  0x2FFFFFF;



int main() {

    uint32_t LEDTargetBit = 0b1;
    uint32_t targetMask;
    while(true){
         for(uint32_t i = 0; i<LED_PIN_COUNT; i++){
             for(uint32_t j = 0; j<LED_PIN_COUNT; j++){
                 if (i==j){
                     j++;
                 }
                 targetMask = (LEDTargetBit<<i)|(LEDTargetBit<<j);
                 gpio_set_dir_in_masked(LEDMask);
                 gpio_set_dir_out_masked(targetMask);
                 gpio_put_masked(targetMask, LEDTargetBit<<j);
                printf("target mask: %x, HiVal: %x\n",targetMask,LEDTargetBit<<j);
             }
         }
    }
}
