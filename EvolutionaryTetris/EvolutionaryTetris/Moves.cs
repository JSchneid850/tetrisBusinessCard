namespace EvolutionaryTetris;

public class Move
{
 public Moveset Moveset { get; set; }
 public double Rating  { get; set; }

 public Move(Moveset moveset, double rating)
 {
     Moveset = moveset;
     Rating = rating;
 }
}

public enum Moveset
{
    Up,
    Down,
    Left,
    Right,
    Swap
}