#include <array>
#include "shape.cpp"
#include "playfield.cpp"

class Matrix {
    public:
        void mapPlayfield(Playfield* playfield){
            std::array<std::array<bool,10>,21> field = playfield->getPlayfield();
            for(int i=0; i < 21; ++i){
                for(int j=0; j < 10; ++j){
                    if(field[i][j]){
                        matrix[i][j] = true;
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
                        //this might be backwards
                        matrix[pos.second + i][pos.first + j] = true;
                    }
                }
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