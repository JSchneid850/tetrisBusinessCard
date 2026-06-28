using EvolutionaryTetris;
using static EvolutionaryTetris.Utils;

namespace EvolutionaryTetris
{
    public class TetrisChromosome
    {
        static Random Random { get; } = new Random();
        private const double MutationStep = .2;

        //determines how much each chromosome cares about a certain factor of how to play the game
        public Guid Guid { get; }
        public double RowsCleared { get; set; }
        public double WeightedHeight { get; set; }
        public double RelativeHeight { get; set; }
        public double Holes { get; set; }
        public double Roughness { get; set; }

        //the fitness of said chromosome
        public double Fitness { get; set; } = -1;


        public TetrisChromosome()
        {
            Guid = Guid.NewGuid();
            RowsCleared = Random.NextDouble();
            WeightedHeight = Random.NextDouble();
            RelativeHeight = Random.NextDouble();
            Holes = Random.NextDouble();
            Roughness = Random.NextDouble();
        }

        public TetrisChromosome(TetrisChromosome parent1, TetrisChromosome parent2, double mutationRate)
        {
            RowsCleared = Utils.RandomChoice<double>(parent1.RowsCleared, parent2.RowsCleared);
            WeightedHeight = Utils.RandomChoice<double>(parent1.WeightedHeight, parent2.WeightedHeight);
            RelativeHeight = Utils.RandomChoice<double>(parent1.RelativeHeight, parent2.RelativeHeight);
            Holes = Utils.RandomChoice<double>(parent1.Holes, parent2.Holes);
            Roughness = Utils.RandomChoice<double>(parent1.Roughness, parent2.Roughness);
            if (Random.NextDouble() > mutationRate)
            {
                RowsCleared = RowsCleared + Random.NextDouble() * MutationStep * 2 - MutationStep;
            }

            if (Random.NextDouble() > mutationRate)
            {
                WeightedHeight = WeightedHeight + Random.NextDouble() * MutationStep * 2 - MutationStep;
            }

            if (Random.NextDouble() > mutationRate)
            {
                RelativeHeight = RelativeHeight + Random.NextDouble() * MutationStep * 2 - MutationStep;
            }

            if (Random.NextDouble() > mutationRate)
            {
                Holes = Holes + Random.NextDouble() * MutationStep * 2 - MutationStep;
            }

            if (Random.NextDouble() > mutationRate)
            {
                Roughness = Roughness + Random.NextDouble() * MutationStep * 2 - MutationStep;
            }
        }

        public Move GetHighestRatedMove(List<Move> moves)
        {
            Double maxRating = Int64.MinValue;
            Move maxMove = new Move(Moveset.Down, Int64.MinValue);
            var ties = new List<Move>();

            foreach (var move in moves)
            {
                if (move.Rating > maxRating)
                {
                    maxRating = move.Rating;
                    maxMove = move;
                }
                else if (move.Rating == maxRating)
                {
                    ties.Add(move);
                }
            }

            if (ties.Count != 0)
            {
                maxMove =  ties[0];
            }

            return maxMove;
        }
    }
}