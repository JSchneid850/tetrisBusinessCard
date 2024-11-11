#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
const uint32_t GPIO_PIN_COUNT = 27;
const uint32_t LED_PIN_COUNT = 22;
const uint32_t LEDMask = 0x4FFFFF;
const uint32_t GPIOMask =  0x2FFFFFF;

uint calculate_hi_res_mask(uint lowMask, uint hiMask){
    return((lowMask|hiMask)^LEDMask);
}



int main() {
    stdio_init_all();
    //turn on all pins needed
    gpio_init_mask(GPIOMask);

    uint32_t LEDHiMask = 0b1;
    uint32_t LEDLowMask = 0b10;

        // gpio_set_dir_out_masked((LEDHiMask<<i)&(LEDLowMask<<j));
        // gpio_set_dir_in_masked(calculate_hi_res_mask(LEDHiMask<<i, LEDLowMask<<j));
        // gpio_put(LEDHiMask<<i, true);
        // gpio_put(LEDLowMask<<(i+j), false);

        // if(i>LED_PIN_COUNT){
        //     i=0;
        //     j++;
        //     sleep_ms(10000);
        // }
        // if(j>LED_PIN_COUNT){
        //     j=0;
        // }
        // i++;
        // printf("i:%d j:%d\n", i, j);
        /// Set all as inputs for high impedance mode
        // gpio_set_dir_all_bits(GPIO_IN);
        // gpio_set_dir(0, GPIO_OUT);
        // gpio_set_dir(1, GPIO_OUT);
        // gpio_put(0, true);
        // gpio_put(1, false);
    while(true){
         for(uint32_t i = 0; i<LED_PIN_COUNT; i++){
             for(uint32_t j = 0; j<LED_PIN_COUNT; j++){
                 if (i==j){
                     j++;
                 }
                 gpio_set_dir_in_masked(LEDMask);
                 gpio_set_dir_out_masked((LEDHiMask<<i)&(LEDLowMask<<j));
                 gpio_put_masked(LEDHiMask<<i, LEDHiMask<<i);
                 gpio_put_masked(LEDLowMask<<j, ~(LEDLowMask<<j));
                 printf("hiMask: %x, lowMask: %x, hiResMask: %x \n",LEDHiMask<<i,LEDLowMask<<j,calculate_hi_res_mask(LEDHiMask<<i, LEDLowMask<<j));
                sleep_ms(100);
             }
         }
    }
}