#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <array>
#include "shape.cpp"

class Playfield {
    public:
        Playfield(Shape* startingShape){
            field = {{false}};
            inPlay = startingShape;
            nextShape = new Shape();
        }

        std::array<std::array<bool,10>,21> getPlayfield(){
            return field;
        }

        void setPlayfield(std::array<std::array<bool, 10>, 21> playfield){
            this->field= playfield;
        }

        Shape* swapShape(Shape* shape) {
            Shape* tempShape = heldShape;
            heldShape = new Shape(*shape);
            heldShape->x = shape->x;
            heldShape->y = shape->y;

            if (tempShape) {
                return new Shape(*tempShape);
            } else {
                return new Shape();
            }
        }

        bool hasHeldShape(){
            return heldShape != nullptr;
        }

        Shape* getHeldShape(){
            return heldShape;
        }

        void setNextShape(Shape* shape){
            nextShape = shape;
        }

        Shape* getNextShape(){
            return nextShape;
        }

        void createNextShape(){
            delete nextShape;
            nextShape = new Shape();
        }
        
    private:
        Shape* inPlay = nullptr;
        Shape* nextShape = nullptr;
        Shape* heldShape = nullptr;
        std::array<std::array<bool,10>,21> field;
};
#endif 