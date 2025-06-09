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
                
                Shape* swappedShape = new Shape(*nextShape);
                delete nextShape;  
                nextShape = new Shape();
                return swappedShape;
            }
            else {          
                Shape* temp = new Shape(*heldShape);
                heldShape = new Shape(*shape);
                heldShape->x = shape->x;
                heldShape->y = shape->y;
                return temp;
            }
        }

        bool hasHeldShape(){
            return heldShape != nullptr;
        }

        Shape* getHeldShape(){
            return heldShape;
        }

        void setHeldShape(Shape* shape){
            heldShape = shape;
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

        void reset(Shape* startingShape, Shape* nextShape){
            this->inPlay = startingShape;
            this->nextShape = nextShape;
            this->field.fill({false});
        }
        
    private:
        Shape* inPlay = nullptr;
        Shape* nextShape = nullptr;
        Shape* heldShape = nullptr;
        std::array<std::array<bool,10>,21> field;
};
#endif 