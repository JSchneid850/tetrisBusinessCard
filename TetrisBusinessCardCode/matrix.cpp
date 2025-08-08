#pragma GCC optimize ("O0")
#include <array>
#include "shape.cpp"
#include "playfield.cpp"
#include "Score.cpp"
#include "Scorecard.cpp"

const int playfieldAndShapeXOffset = 11;
const int heldAndNextShapexOffset = 4; 
const int heldShapeYOffset = 6;
const int scoreYOffset = 12;  
const int scoreXOffset = 2;   


class Matrix {
    public:
        void mapPlayfield(Playfield* playfield){
            std::array<std::array<bool,10>,21> field = playfield->getPlayfield();
            for(int i=0; i < 21; ++i){
                for(int j=0; j < 10; ++j){
                    if(field[i][j]){
                        matrix[i][j + playfieldAndShapeXOffset] = true;
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
                        matrix[pos.second + i ][pos.first + j + playfieldAndShapeXOffset] = true;
                    }
                }
            }
        }

        void mapHeldShape(Shape* shape){
            if(shape){
                std::array<std::array<bool,4>, 4> tetromino = shape->getShape();
                for(int i=0; i <4; ++i){
                    for(int j=0; j<4; ++j){
                        if(tetromino[i][j]){
                            matrix[i + heldShapeYOffset][j +heldAndNextShapexOffset] = true;
                        }
                    }
                }
            }
        }

        void mapNextShape(Shape* shape){
            if(shape){
                std::array<std::array<bool,4>, 4> tetromino = shape->getShape();
                for(int row=0; row <4; ++row){
                    for(int col=0; col<4; ++col){
                        if(tetromino[row][col]){
                            matrix[row][col+heldAndNextShapexOffset] = true;
                        }
                    }
                }
            }
        }

        void mapScore(Score* score) {
            std::array<std::array<bool, 7>, 5> scoreGrid = score->getGrid();
            for (int row = 0; row < 5; ++row) {
                for (int col = 0; col < 7; ++col) {
                    if (scoreGrid[row][col]) {
                        matrix[row + scoreYOffset][col + scoreXOffset] = true;
                    }
                }
            }
        }

        void mapCaret(int xOffset, int yOffset){
            int topYOffset = 5;
            int bottomYOffset =17;
            const auto& caret= Alphabet::caret;

            for(int row = 0; row < 5; ++row){
                for(int col = 0; col < 5; ++col){
                    if(caret[row][col]){
                        matrix[yOffset + topYOffset + row][xOffset  + col] = true;
                    }
                    if(caret[4-row][col]){
                        matrix[(yOffset + bottomYOffset + row)][xOffset  + col] = true;
                    }
                }
            }
        }


        void mapScorecard(Scorecard scorecard, int scorecardXOffset, int scorecardYOffset, int scrollStep) {
            int width = scorecard.calculateWidth();
            int initialXOffset = scorecardXOffset + (width-20);
            if ((width > 20)){
                auto initials = scorecard.getInitials();
                int currentXOffset = scorecardXOffset;
                int score = scorecard.getScore();
                mapScore(score, scorecardYOffset,  currentXOffset + scrollStep - initialXOffset);
                currentXOffset += scorecard.calculateScoreWidth() + 1;
                mapInitials(initials,  currentXOffset + scrollStep -initialXOffset, scorecardYOffset, true);
            }else {
                auto initials = scorecard.getInitials();
                int currentXOffset = scorecardXOffset;
                int score = scorecard.getScore();
                mapScore(score, scorecardYOffset, currentXOffset-initialXOffset);
                currentXOffset += scorecard.calculateScoreWidth() + 1;
                mapInitials(initials, currentXOffset-initialXOffset, scorecardYOffset, true);
            }
            
        }
        
        void mapInitials(std::array<char,3> initials, int initialsXOffset, int initialsYOffset, bool dynamicSpacing){
            int currentXOffset = initialsXOffset;
            auto letters = Alphabet::getBinaryInitials(initials, dynamicSpacing);
            for (int l = 0; l < 3; ++l) {
                const auto& grid = letters[2-l];
                char upperChar = std::toupper(initials[2-l]);
                int width = dynamicSpacing ? Alphabet::letterWidth[upperChar - 'A'] : 5;
                

                for (int i = 0; i < 5; ++i) {
                    for (int j = 0; j < 5; ++j) {
                        if (currentXOffset + j > 20 || currentXOffset + j < 0) continue;
                        if (width -1- j < 0) continue;
                        if (grid[i][width -1- j]) {
                            matrix[initialsYOffset + i][currentXOffset + j] = true;
                        }
                    }
                }
                
                currentXOffset += width + 1; // 1 column of spacing
            }
        }

        void mapScore(int score, int scoreY, int scoreX){
            int digit1 = score / 10; 
            int digit2 = score % 10;  
            int scoreXOffset = scoreX;
            int width1 = Numbers::numberWidth[digit1];
            int width2 = Numbers::numberWidth[digit2];
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < width2; ++j) {
                    if (scoreXOffset + j > 20 || scoreXOffset + j < 0) continue;
                    matrix[scoreY + i][scoreXOffset + j] = Numbers::number[digit2][i][2-j];
                }
            }
            scoreXOffset += width2 + 1;

            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < width1; ++j) {
                    if (scoreXOffset + j > 20 || scoreXOffset + j < 0) continue;
                    matrix[scoreY + i][scoreXOffset + j] = Numbers::number[digit1][i][2-j];
                }
            }

        }

        void mapPlayFieldIndicator(){
            for(int i = 0; i< 10; ++i){
                matrix[21][i+playfieldAndShapeXOffset] = true;
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