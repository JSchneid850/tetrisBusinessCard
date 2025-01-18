#include <stdio.h>
#include <vector>
#include <iostream>
#include "pico/stdlib.h"

class charlieplexDriver
{
    private:
        uint32_t GPIO_PIN_COUNT = 27;
        uint32_t LED_PIN_COUNT = 22;
        uint32_t LEDMask =   0xFFFFFF4;
        uint32_t GPIOMask =  0xFFFFFFFF;
        //bool rotateResult[21][21] = {false};
        uint32_t frameArray[21]= {0}; // a frame with bitmasks equal to vertical bitmask (Will be horizontal bitmask in hardware rev 0.3)

        uint32_t expandRow(uint32_t row, uint32_t rowIndex){
            uint32_t shift = (row << 1u);
            uint32_t rightMask = ~(1u << rowIndex);
            uint32_t leftMask = ~((~0u<<rowIndex));
            uint32_t result = ((shift & rightMask)& ~leftMask)|(leftMask & row);
            return result;
        }

        // //once rev 0.3 is done and I've flipped the led matrix to scan vertically this code will be removed and subsequently burned 
        // void rotateFrameArray(bool (&frame)[21][21]){
        //     for(int i = 0; i < 21; i++){
        //         for(int j=0; j < 21; j++){
        //             rotateResult[20-j][i]= frame[i][j];
        //         }
        //     } 
        // }

        //funnily enough we can deal with bool's up to this point, should make it easier to write games just by dealing with an array of on's and off's
        void compressFrameArray(bool (&frame)[21][20]){
            for(int i = 0; i <= 21; i++){
                frameArray[i] = 0u;//reset frame array from last call
                for(uint32_t j = 0; j <= 20; j++){
                    if (frame[i][j]){
                        frameArray[i] = frameArray[i] + 1u << j;
                    }
                }
            }
        }

        void printArrayState(bool (&frame)[21][20]){
            for (int i = 0; i <= 21; ++i) {
                for (int j = 0; j <= 20; ++j) {
                std::cout << frame[i][j] << " ";
            }
            std::cout << std::endl;
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

        void writeFrame(bool (&frames)[21][20]){
            //std::cout << "Pre-Rotation" << std::endl;
            //printArrayState(frames);
            //rotateFrameArray(frames);
            //std::cout << "Post-rotation" << std::endl;
            //printArrayState(rotateResult);
            compressFrameArray(frames);
            //std::cout << "frameArray" <<std::endl;
            //printFrameArray();
            for(uint32_t i = 0u; i <= 21u; i++){
                uint32_t framebuff = expandRow(frameArray[i]<<2, i<<2);// shift framebuff to account for pins 1 and 2 in rev0.2+ being used for uart debugging
                //std::cout << "framebuff:" << framebuff << std::endl;
                gpio_set_dir_in_masked(LEDMask);
                gpio_set_dir_out_masked((framebuff | 1u<<i<<2));

                //double check what put masked needs to be here 
                //1/14/25: this should be fine but need to check row logic since row 0 and 1 may be flipped
                gpio_put_masked(LEDMask, (~framebuff | 1u<<i<<2));
                framebuff = 0;
            }
        }
}; 