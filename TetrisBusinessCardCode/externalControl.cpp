#include "pico/stdlib.h"
#include "playfield.cpp"
#include "button.cpp"
#include <string_view>
#include "pico/stdio_uart.h"


class externalControl
{
    public:  
      std::string static createMessage(Playfield* playfield, int currentPiece, int heldPiece, int nextPiece)
        {
            std::array<std::array<bool,10>,21> field = playfield->getPlayfield();
            std::string message = "";
            for(int i=0; i < 21; ++i){
                for(int j=0; j < 10; ++j){ 
                    message += field[i][j];
                }
                message += ",";
            }
            message += ",curr:" + std::to_string(currentPiece) + ",held:" + std::to_string(heldPiece) + ",next:" + std::to_string(nextPiece) + "\n";
        }

        void static sendMessage(char* message, uart_inst_t* uart){
            uart_puts(uart, message);
        }


        Action static decodeAction(std::string_view action){
            if (action == "UP") return Action::UP;
            if (action == "DOWN") return Action::DOWN;
            if (action == "LEFT") return Action::LEFT;
            if (action == "RIGHT") return Action::RIGHT;
            if (action == "A") return Action::A;
            if (action == "B") return Action::B;
        }
};
