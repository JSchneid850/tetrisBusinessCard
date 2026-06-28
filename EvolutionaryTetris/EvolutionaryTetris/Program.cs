using GeneticSharp;

namespace EvolutionaryTetris;

class Program
{
    static void Main(string[] args)
    
    {
        var selection = new EliteSelection();
        var crossover = new OrderedCrossover();
        var mutation = new ReverseSequenceMutation();
        var fitness = new TetrisFitness();
        var chromosome = new TetrisChromosome();
        var population = new Population (50, 70, chromosome);

        var ga = new GeneticAlgorithm(population, fitness, selection, crossover, mutation);
        ga.Termination = new GenerationNumberTermination(100);

        Console.WriteLine("GA running...");
        ga.Start();

        Console.WriteLine("Best solution found has {0} fitness.", ga.BestChromosome.Fitness);
    }
}