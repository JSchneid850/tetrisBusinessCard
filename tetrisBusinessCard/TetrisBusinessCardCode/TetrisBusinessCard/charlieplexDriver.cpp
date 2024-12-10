#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"

const uint32_t GPIO_PIN_COUNT = 27;
const uint32_t LED_PIN_COUNT = 22;
const uint32_t LEDMask = 0x3FFFFF;
const uint32_t GPIOMask =  0x2FFFFFF;

void init(){
    stdio_init_all();
    gpio_init_mask(GPIOMask);
}

int writeFrame(const std::vector<uint32_t> &frames){
    uint32_t framebuff;
    for(uint32_t frame : frames){
        framebuff = expandRow(frame);
        printf("Expanded Frame: %x \n Original Frame %x \n");
        gpio_set_dir_in_masked(LEDMask);
        gpio_set_dir_out_masked(framebuff);

        //double check what put masked needs to be here
        gpio_put_masked(framebuff, framebuff | /*row integer*/);
    }
}

//need to add index as param for inserting low bit
uint32_t expandRow(uint32_t row){

}