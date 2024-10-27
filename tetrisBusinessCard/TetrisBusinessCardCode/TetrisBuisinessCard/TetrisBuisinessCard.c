#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
const int GPIO_PIN_COUNT = 27;

void GPIO_Init(){
    int i; 
    for(i = 0; i < 28; i++){
        gpio_init(i);
    }
}

int main() {
    stdio_init_all();
    //turn on all pins needed
    GPIO_Init();
    
    gpio_set_dir_all_bits(GPIO_IN);

    int i;
    for(i = 0; i < 28; i++){
        gpio_set_dir(i,GPIO_OUT);
        gpio_put(i, true);
    }
    
   
    // while (!stdio_usb_connected()) {
    //     sleep_ms(100);
    // }
    printf("Connected\n");
    while (true) {
         sleep_ms(1000);
         printf("Running from RAM\n");
    }
}