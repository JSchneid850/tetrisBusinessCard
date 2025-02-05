#include <array>
#include "shape.cpp"
#include "playfield.cpp"
#include "scoreboard.cpp"

const int playfieldAndShapeXOffset = 11;
const int heldAndNextShapexOffset = 4; 
const int heldShapeYOffset = 6;
const int scoreYOffset = 12;  
const int scoreXOffset = 2;   


class Matrix {
    public:
        void mapPlayfield(Playfield* playfield){
            std::array<std::array<bool,10>,21> field = playfield->getPlayfield();
            for(int i=0; i < 21; ++i){
                for(int j=0; j < 10; ++j){
                    if(field[i][j]){
                        matrix[i][j + playfieldAndShapeXOffset] = true;
                    }
                }
            }
        }

        void mapShape(Shape* shape){
            std::pair<int,int> pos {shape->x, shape->y};
            std::array<std::array<bool,4>, 4> tetromino = shape->getShape();
            for(int i=0; i <4; ++i){
                for(int j=0; j<4; ++j){
                    if(tetromino[i][j]){
                        matrix[pos.second + i ][pos.first + j + playfieldAndShapeXOffset] = true;
                    }
                }
            }
        }

        void mapHeldShape(Shape* shape){
            if(shape){
                std::array<std::array<bool,4>, 4> tetromino = shape->getShape();
                for(int i=0; i <4; ++i){
                    for(int j=0; j<4; ++j){
                        if(tetromino[i][j]){
                            matrix[i + heldShapeYOffset][j +heldAndNextShapexOffset] = true;
                        }
                    }
                }
            }
        }

        void mapNextShape(Shape* shape){
            if(shape){
                std::array<std::array<bool,4>, 4> tetromino = shape->getShape();
                for(int i=0; i <4; ++i){
                    for(int j=0; j<4; ++j){
                        if(tetromino[i][j]){
                            matrix[i][j+heldAndNextShapexOffset] = true;
                        }
                    }
                }
            }
        }

        void mapScoreboard(Scoreboard* score) {
            std::array<std::array<bool, 7>, 5> scoreGrid = score->getGrid();
            for (int row = 0; row < 5; ++row) {
                for (int col = 0; col < 7; ++col) {
                    if (scoreGrid[row][col]) {
                        matrix[row + scoreYOffset][col + scoreXOffset] = true;
                    }
                }
            }
        }

        void mapPlayFieldIndicator(){
            for(int i = 0; i< 10; ++i){
                matrix[21][i+playfieldAndShapeXOffset] = true;
            }
        }

        void reset(){
            matrix = {{false}};
        }

        std::array<std::array<bool,21>,22> getMatrix(){
            return matrix;
        }

    private:
        std::array<std::array<bool,21>,22> matrix = {{false}};
};