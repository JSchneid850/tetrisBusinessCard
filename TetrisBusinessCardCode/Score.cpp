#include <array>
#include "Numbers.cpp"
class Score
{
    public:
        Score(){
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
                    scoreGrid[row][col + 4] = Numbers::number[digit1][row][2-col];
                }
            }

            for (int row = 0; row < 5; ++row) {
                for (int col = 0; col < 3; ++col) {
                    scoreGrid[row][col] = Numbers::number[digit2][row][2-col];
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
        
};