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
        if (!heldShape) {
                heldShape = new Shape(*shape);
                heldShape->x = shape->x; 
                heldShape->y = shape->y;

                Shape* newShape = new Shape(*nextShape);
                newShape->x = shape->x;
                newShape->y = shape->y;

                return newShape;
            } else {
                Shape* tempShape = heldShape;
                heldShape = new Shape(*shape);
                heldShape->x = shape->x;
                heldShape->y = shape->y;

                tempShape->x = shape->x; 
                tempShape->y = shape->y;

                return tempShape;
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