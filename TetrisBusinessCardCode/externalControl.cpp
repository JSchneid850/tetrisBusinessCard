#include "pico/stdlib.h"
#include "playfield.cpp"
#include "button.cpp"


static class externalControl{
    public:  
      std::string createMessage(Playfield* field, int currentPiece, int heldPiece, int nextPiece)
        {
            std::array<std::array<bool,10>,21> field = playfield->getPlayfield();
            std::string message = "";
            for(int i=0; i < 21; ++i){
                for(int j=0; j < 10; ++j){ 
                    message += field[i][j];
                }
                message += ",";
            }
            message += ",curr:" + currentPiece + ",held:" + heldPiece + ",next:" + nextPiece + "\n";
        }

        void sendMessage(std::string message){
            uart_puts(UART_ID, message);
        }

        Action decodeMessage(std::string message){
            switch (message)
            {
                case "UP":
                    return Action::UP
                    break;
                case "DOWN":
                    return Action::DOWN
                    break;
                case "LEFT":
                    return Action::LEFT
                    break;
                case "RIGHT":
                    return Action::RIGHT
                    break;
                case "A":
                    return Action::A
                    break;
                case "B":
                    return Action::B
                    break;                    
            }
        }

