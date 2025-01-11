#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"

class charlieplexDriver
{
    private:
        uint32_t GPIO_PIN_COUNT = 27;
        uint32_t LED_PIN_COUNT = 22;
        uint32_t LEDMask = 0xFFFFFF;
        uint32_t GPIOMask =  0xFFFFFFF;

        //need to add index as param for inserting low bit
        uint32_t expandRow(uint32_t row, uint32_t rowIndex){
            uint32_t shift = (row << 1u);
            uint32_t rightMask = ~(1u << rowIndex);
            uint32_t leftMask = ~((~0u<<rowIndex));
            uint32_t result = ((shift & rightMask)& ~leftMask)|(leftMask & row);
            return result;
        }

        //once rev 0.3 is done and I've flipped the led matrix to scan vertically this code will be removed and subsequently burned 
        int rotateFrameArray(std::vector<uint32_t> &frames){
            int size = frames.size();
            std::vector<uint32_t> result(size, 0);
            
            for(int i = 0; i < size; i++){
                for(int j=0; j < size; j++){
                    if(frames[i] & (1<<j)){// if the j'th bit is set in our original value
                        result[size - j - 1] + 1u<<j;
                    }
                }
            } 
            frames = result;
            return 1;
        }
        
    public:
        charlieplexDriver()
        {
            stdio_init_all();
            gpio_init_mask(GPIOMask);
        }

        void writeFrame(std::vector<uint32_t> &frames){
            uint32_t framebuff;
            rotateFrameArray(frames);
            for(uint32_t i = 0u; i < 21u; i++){
                framebuff = expandRow(frames[i]<<2, i);// shift framebuff to account for pins 1 and 2 in rev0.2 being used for uart debugging

                gpio_set_dir_in_masked(~(framebuff | 1u<<i)); 
                gpio_set_dir_out_masked(framebuff | 1u<<i);

                //double check what put masked needs to be here
                gpio_put_masked(framebuff | 1u<<i, framebuff);
                framebuff = 0;
            }
        }
}; 