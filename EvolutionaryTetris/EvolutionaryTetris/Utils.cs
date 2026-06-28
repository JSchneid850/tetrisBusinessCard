namespace EvolutionaryTetris;

public class Utils
{
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