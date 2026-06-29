namespace EvolutionaryTetris;

public class Playfield
{
    public bool[,] field = new bool[21,10];
    public Shape held = new();
    public Shape current = new();

}
