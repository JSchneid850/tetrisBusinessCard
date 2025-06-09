#include <array>
class Scoreboard
{
    public:
        Scoreboard(){
            score = 0;
            clearGrid();
        }
        void addScore(int increase){
            score += increase;
        }

        int getScore(){
            return score;
        }

        void updateScoreGrid() {
            int digit1 = score / 10; 
            int digit2 = score % 10;  

            clearGrid();

            for (int row = 0; row < 5; ++row) {
                for (int col = 0; col < 3; ++col) {
                    scoreGrid[row][col + 4] = numbers[digit1][row][2-col];
                }
            }

            for (int row = 0; row < 5; ++row) {
                for (int col = 0; col < 3; ++col) {
                    scoreGrid[row][col] = numbers[digit2][row][2-col];
                }
            }
        }

        std::array<std::array<bool,7>,5> getGrid(){
            return scoreGrid;
        }

        void resetScore(){
            score = 0;
            updateScoreGrid();
        }
    private:
        int score = 0;
        std::array<std::array<bool,7>,5> scoreGrid;

        void clearGrid(){
            for(auto &row:scoreGrid){
                row.fill(false);
            }
        }
        static constexpr std::array<std::array<std::array<bool, 3>, 5>, 10> numbers = {{
            //0
            {{
                {true, true, true},
                {true, false, true},
                {true, false, true},
                {true, false, true},
                {true, true, true},
            }},
            //1
            {{
                {false, false, true},
                {false, false, true},
                {false, false, true},
                {false, false, true},
                {false, false, true},
            }},
            //2
            {{
                {true, true, true},
                {false, false, true},
                {true, true, true},
                {true, false, false},
                {true, true, true},
            }},
            //3
            {{
                {true, true, true},
                {false, false, true},
                {true, true, true},
                {false, false, true},
                {true, true, true},
            }},
            //4
            {{
                {true, false, true},
                {true, false, true},
                {true, true, true},
                {false, false, true},
                {false, false, true},
            }},
            //5
            {{
                {true, true, true},
                {true, false, false},
                {true, true, true},
                {false, false, true},
                {true, true, true},
            }},
            //6
            {{
                {true, true, true},
                {true, false, false},
                {true, true, true},
                {true, false, true},
                {true, true, true},
            }},
            //7
            {{
                {true, true, true},
                {false, false, true},
                {false, false, true},
                {false, false, true},
                {false, false, true},
            }},
            //8
            {{
                {true, true, true},
                {true, false, true},
                {true, true, true},
                {true, false, true},
                {true, true, true},
            }},
            //9
            {{
                {true, true, true},
                {true, false, true},
                {true, true, true},
                {false, false, true},
                {true, true, true},
            }},
        }};
};