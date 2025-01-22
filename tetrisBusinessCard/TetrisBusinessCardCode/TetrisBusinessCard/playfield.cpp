#include <array>

class Playfield {
    public:
        Playfield(){
            field = {{false}};
        }

        std::array<std::array<bool,21>,10> GetPlayfield(){
            return field;
        }

    private:
        std::array<std::array<bool,21>,10> field;
};