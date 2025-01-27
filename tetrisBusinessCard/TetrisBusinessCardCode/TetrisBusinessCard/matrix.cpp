#include <array>
#include "shape.cpp"
#include "playfield.cpp"

class Matrix {
    public:
        void mapPlayfieldp(Playfield* playfield){
            std::array<std::array<bool,21>,10> field = playfield->getPlayfield();
            for(int i=0; i < 21; ++i){
                for(int j=0; j < 10; ++j){
                    if(field[i][j]){
                        matrix[i][j] = true;
                    }
                }
            }0
        }

        void mapShape(Shape* shape){
            std::pair<int,int> pos = shape->getPos();
            std::array<std::array<bool,4>, 4> tetromino = shape->getShape();
            for(int i=0; i <4; ++i){
                for(int j=0; j<4; ++j){
                    if(tetromino[i][j]){
                        //this might be backwards
                        matrix[i+pos.first][j+pos.second]
                    }
                }
            }

        }

        std::array<std::array<bool,21>,20> getMatrix(){
            return matrix;
        }

    private:
        std::array<std::array<bool,22>,21> matrix = {{false}};
}