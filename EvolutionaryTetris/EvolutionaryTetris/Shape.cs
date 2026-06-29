namespace EvolutionaryTetris
{
    public class Shape
    {
        public int X; 
        public int Y;
        public TetrominoEnum SelectedTetromino;
        // 3D array: [shape, row, col]
        public static readonly bool[][,] Tetromino =
        [
            // I Shape
            new bool[,]
            {   { false, false, false, false },
                { true,  true,  true,  true  },
                { false, false, false, false },
                { false, false, false, false }
            },
            // O Shape
            new bool[,]
            {   { false, false, false, false },
                { false, true,  true,  false },
                { false, true,  true,  false },
                { false, false, false, false }
            },
            // T Shape
            new bool[,]
            {   { false, false, false, false },
                { false, true,  true,  true  },
                { false, false, true,  false },
                { false, false, false, false }
            },
            // S Shape
            new bool[,]
            {   { false, false, false, false },
                { false, false, true,  true  },
                { false, true,  true,  false },
                { false, false, false, false }
            },
            // Z Shape
            new bool[,]
            {   { false, false, false, false },
                { false, true,  true,  false },
                { false, false, true,  true  },
                { false, false, false, false }
            },
            // J Shape
            new bool[,]
            {   { false, false, false, false },
                { false, true,  false, false },
                { false, true,  true,  true  },
                { false, false, false, false }
            },
            // L Shape
            new bool[,]
            {   { false, false, false, false },
                { false, false, false, true  },
                { false, true,  true,  true  },
                { false, false, false, false }
            }
        ];
    }

    public enum TetrominoEnum
    {
        I,
        O,
        T,
        S,
        Z,
        J,
        L
    }
}
