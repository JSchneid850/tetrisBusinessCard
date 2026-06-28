namespace EvolutionaryTetris;


public class TetrisPopulation
{
    private const int PopulationSize = 50;
    List<TetrisChromosome> Chromosomes = new List<TetrisChromosome>();

    public TetrisPopulation()
    {
        for (int i = 0; i < PopulationSize; i++)
        {
            Chromosomes.Add(new TetrisChromosome());
        }
    }

    public void Evolve()
    {
        
    }
}