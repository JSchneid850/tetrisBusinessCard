#include <stdio.h>
#include <vector>
#include <iostream>
#include <array>
#include "pico/stdlib.h"

class charlieplexDriver
{
    private:
        uint32_t GPIO_PIN_COUNT = 27;
        uint32_t LED_PIN_COUNT = 22;
        uint32_t LEDMask =   0xFFFFFFC;
        uint32_t GPIOMask =  0xFFFFFFFF;
        uint32_t frameArray[22]= {0}; // a frame with bitmasks equal to vertical bitmask (Will be horizontal bitmask in hardware rev 0.3)

        uint32_t expandRow(uint32_t row, uint32_t rowIndex){
            uint32_t shift = (row << 1u);
            uint32_t rightMask = ~(1u << rowIndex);
            uint32_t leftMask = ~((~0u<<rowIndex));
            uint32_t result = ((shift & rightMask)& ~leftMask)|(leftMask & row);
            return result;
        }

        void compressFrameArray(std::array<std::array<bool, 21>, 22>& frame) {
            for (size_t i = 0; i < 22; i++) {
                frameArray[i] = 0u; 
                for (size_t j = 0; j < 21; j++) {
                    if (frame[i][j]) {
                        frameArray[i] |= (1u << j);
                    }
                }
            }
        }

        void printFrameArray(){
            for(int i = 0; i < 21; ++i){
                std::cout<< frameArray[i]<<std::endl;
            }
        }
        
    public:
        charlieplexDriver()
        {
            stdio_init_all();
            gpio_init_mask(GPIOMask);
        }

        void writeFrame(std::array<std::array<bool, 21>, 22>& frames){
            
            //TODO: clear gpio pin out writes
            compressFrameArray(frames);
            for(uint32_t i = 0u; i < 22u; i++){
                int shift = 2;// shift framebuff to account for pins 1 and 2 in rev0.2+ being used for uart debugging
                uint32_t framebuff = expandRow(frameArray[i]<<2, i+shift);//expand row in frame to be 21 bits long
                gpio_set_dir_in_masked(~(framebuff | 1u<<i<<shift));//set high-z pins
                gpio_set_dir_out_masked((framebuff | 1u<<i<<shift));//set pins for IO

                gpio_put_masked(LEDMask, (~framebuff | 1u<<i<<shift));//set IO pins for each row
                sleep_us(10);
            }
            gpio_set_dir_all_bits(GPIO_IN);//clear last frame
        }
}; 