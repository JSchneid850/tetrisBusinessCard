#include <array>
#include "shape.cpp"
class Playfield {
    public:
        Playfield(Shape* startingShape){
            field = {{false}};
            inPlay = startingShape;
        }

        std::array<std::array<bool,21>,10> getPlayfield(){
            return field;
        }

        Shape swapShape(Shape* shape){

            Shape* tempShape = this->heldShape;
            this->heldShape = shape;
            return *tempShape;
        }
        
    private:
        Shape* inPlay = NULL;
        Shape* heldShape = NULL;
        std::array<std::array<bool,21>,10> field;
};