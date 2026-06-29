namespace EvolutionaryTetris;

public class Utils
{
    public static bool CheckCollision(Playfield field, Shape tetromino)
    {
        int x = tetromino.X;
        int y = tetromino.Y;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; i++)
            {
                if (Shape.Tetromino[(int)tetromino.SelectedTetromino][i,j])
                {
                    int newX = x + j;
                    int newY = y + i;

                    if (newX < 0 || newX >= 10 || newY < 0 || newY >= 21 || field.field[newY, newX])
                    {
                        return true;
                    }
                    
                }
            }
        }

        return false;
    }
    public static T RandomChoice<T>(T itemOne, T itemTwo)
    {
        if (Math.Round(Random.Shared.NextDouble()) == 0)
        {
            return itemOne;
        }
        else
        {
            return itemTwo;
        }
    }
}