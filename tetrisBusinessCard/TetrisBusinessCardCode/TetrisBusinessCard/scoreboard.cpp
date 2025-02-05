class scoreboard
{
    public:
        scoreboard(){
            score = 0;
        }
        void addScore(int increase){
            score += increase;
        }

        void resetScore(){
            score = 0;
        }
    private:
        int score = 0
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
        }}

}