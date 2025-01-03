#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"

class charlieplexDriver
{
    private:
        uint32_t GPIO_PIN_COUNT = 27;
        uint32_t LED_PIN_COUNT = 22;
        uint32_t LEDMask = 0x3FFFFF;
        uint32_t GPIOMask =  0x2FFFFFF;

        //need to add index as param for inserting low bit
        uint32_t expandRow(uint32_t row, uint32_t rowIndex){
            uint32_t shift = (row << 1);
            uint32_t rightMask = ~(1 << rowIndex);
            uint32_t leftMask = ~((~0<<rowIndex));
            return ((shift & rightMask)& ~leftMask)|(leftMask & row);
        }
        
    public:
        charlieplexDriver()
        {
            stdio_init_all();
            gpio_init_mask(GPIOMask);
        }

        void writeFrame(const std::vector<uint32_t> &frames){
            uint32_t framebuff;
            for(int i = 0; i <= 21; i++){
                framebuff = expandRow(frames[i], i);
                printf("Expanded Frame: %x \n Original Frame %x \n");
                gpio_set_dir_in_masked(LEDMask);
                gpio_set_dir_out_masked(framebuff | 1<<i);

                //double check what put masked needs to be here
                gpio_put_masked(framebuff | 1<<i, framebuff);
            }
        }
};