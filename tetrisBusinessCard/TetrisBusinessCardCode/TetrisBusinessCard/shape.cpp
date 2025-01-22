#include <array>
#include <iostream>
#include <random>

class Shape {
    public:
        static const std::array<std::array<std::array<bool, 4>, 4>, 7> tetrominoes;

        Shape(int shapeChoice) {
            shape = tetrominoes.at(shapeChoice);
        }

        void rotateClockwise(){
            std::array<std::array<bool,4>,4> rotatedC;
            for(int i = 0; i< 4; ++i){
                for(int j = 0; j<4; ++j){
                    rotatedC[j][3-i]= shape[i][j];
                }
            }

            shape = rotatedC;
        }

        void rotateCounterClockwise(){
            std::array<std::array<bool,4>,4> rotatedCC;
            for(int i = 0; i< 4; ++i){
                for(int j = 0; j<4; ++j){
                    rotatedCC[3-j][i] = shape[i][j];
                }
            }
            shape = rotatedCC;
        }

        std::array<std::array<bool,4>, 4> getShape(){
            return shape;
        }
        
    private:
        std::array<std::array<bool, 4>, 4> shape;

};

const std::array<std::array<std::array<bool, 4>, 4>, 7> Shape::tetrominoes = {{
    // I Shape
    {{
        {false, false, false, false},
        {true,  true,  true,  true},
        {false, false, false, false},
        {false, false, false, false}
    }},
    // O Shape
    {{
        {false, false, false, false},
        {false, true,  true,  false},
        {false, true,  true,  false},
        {false, false, false, false}
    }},
    // T Shape
    {{
        {false, false, false, false},
        {false, true,  true,  true },
        {false, false, true,  false},
        {false, false, false, false}
    }},
    // S Shape
    {{
        {false, false, false, false},
        {false, false, true,  true },
        {false, true,  true,  false},
        {false, false, false, false}
    }},
    // Z Shape
    {{
        {false, false, false, false},
        {false, true,  true,  false},
        {false, false, true,  true },
        {false, false, false, false}
    }},
    // J Shape
    {{
        {false, false, false, false},
        {false, true,  false, false},
        {false, true,  true,  true },
        {false, false, false, false}
    }},
    // L Shape
    {{
        {false, false, false, false},
        {false, false, false, true },
        {false, true,  true,  true },
        {false, false, false, false}
    }},
}};
