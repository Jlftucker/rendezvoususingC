#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>

// Define the type for our genetic algorithm's vectors
using Chromosome = std::vector<float>;



// Define the fitness function - for now this is a placeholder
float fitnessFunction(const Chromosome& chromosome) {
    // Example fitness function: sum of the chromosome's values
    return std::accumulate(chromosome.begin(), chromosome.end(), 0.0f);
}

// Function to initialize a population of floats

/*
initialize_population - A function that initializes a population of vectors containing floats between 1 and 0

INPUT

populationSize -  INT - The total amount of individuals in this generation
chromosomeLength - INT - The length of the chromosome of an individual

OUTPUT

population - vector<Chromosome> - a 2 dimensional vector containing vectors that are the population of
                                   the current generation

*/

std::vector<Chromosome> initializePopulation(int populationSize, int chromosomeLength) {
    std::vector<Chromosome> population(populationSize, Chromosome(chromosomeLength));//declare the 2D array of a population size vectors all with length chromosome length
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);//allows for generation of numbers between 1 and 0

    for (auto& chromosome : population) {//create a new chromosome for each in the population
        for (auto& gene : chromosome) {//While accessing that chromosome generate a gene
            gene = dis(gen); // Random float between 0 and 1
        }
    }

    return population;
}


/*
Function - fitness_values - Evaluate the population of fitness functions and store the results in an vector

INPUT

population - std::vector<Chromosome> - a 2d vector that contains the genomes of the population


*/


std::vector<float> fitness_values(const std::vector<Chromosome>& population){
    std::vector<float> fitness_value_table;//Declare fitness value holder
    //std::cout << "ping inside"; Debug statement
    for(int i = 0; i < population.size(); ++i){//for i in the range of the size of the population
        fitness_value_table.push_back(fitnessFunction(population[i]));//Evaluate the genome using the fitness function
    }
    return fitness_value_table;
}


// Function to perform crossover between two chromosomes

/*
Function - crossover - performs crossover between 2 parents genes

INPUT

parent 1 - Chromosome(1D vector float) pointer - points to the memory address of parent 1 (solution 1)
parent 2 - Chromosome(1D vector float) pointer - points to the memory address of parent 2 (solution 2)

OUTPUT

offspring - Chromosome(1D vector float) - the offspring of two individuals ( a new solution chromosome)
*/


Chromosome crossover(const Chromosome& parent1, const Chromosome& parent2) {
    std::random_device rd; //random gen
    std::mt19937 gen(rd());//set seed for randomness
    std::uniform_real_distribution<> dis(0.0, 1.0);//set distribution between 0 and 1

    Chromosome offspring(parent1.size());//We create a vector that represents the offspring using our chromosone type that is a 1D vector
    for (size_t i = 0; i < parent1.size(); ++i) {
        // Use random value to decide whether to take from parent1 or parent2
        offspring[i] = dis(gen) < 0.5 ? parent1[i] : parent2[i];//this is a conditional whether to pull from parent 1 or parent 2
    }

    return offspring;//return the offspring
}


/*
Function - mutate - performs a mutation on a chromosome

INPUT

chromosome - Chromosome(1D vector float) pointer - Points to the memory address of the individual chromosome
mutationRate - float - the number that decides if we mutate a gene of an individual or not

*/


void mutate(Chromosome& chromosome, float mutationRate) {
    std::random_device rd;//random device generator
    std::mt19937 gen(rd());//generate random
    std::uniform_real_distribution<> dis(0.0, 1.0);//set distribution between 0 and 1

    for (auto& gene : chromosome) {// for each gene (element of an individual chromosome)
        // Apply mutation with a given probability
        if (dis(gen) < mutationRate) {//if the random number generated is below the mutation rate
            gene = dis(gen); // mutate the gene and then move onto the next one in the chromosome
        }
    }
}



// Function to perform selection using tournament selection

/*
Function - tournamentSelection - A function that takes the input population and runs a tournament using the "winner stays on method"

INPUT
population     - vector<Chromosome>& - A pointer to the memory address of the 2D population vector
fitness_values - vector<float>& - A pointer to the memory address of the 1D vector that holds all of the
                                  fitness values for the chromosomes

tournamentSize - int - The number of rounds of the tournament that wish to be held to decide an optimal parent

OUTPUT

bestChromosome - Chromosome(1D vector float) - A 1d vector list containing the winning gene decided to be the parent gene

*/


Chromosome tournamentSelection(const std::vector<Chromosome>& population, const std::vector<float>& fitnessValues, int tournamentSize) {
    std::random_device rd;//random_device for generation of random
    std::mt19937 gen(rd());//random generator
    std::uniform_int_distribution<> dis(0, population.size() - 1);//picks an integer between 0 and the max population number(-1)

    // Randomly select chromosomes for the tournament
    size_t bestIndex = dis(gen);//randomly pick one
    float bestFitness = fitnessValues[bestIndex];;//pull out the fitness of the randomly picked individual

    //iterate through the population which is the tournament size
    for (int i = 1; i < tournamentSize; ++i) {//for i in tournament size (set via external parameter)
        size_t candidateIndex = dis(gen);//randomly pick a candidate from the population size
        float candidateFitness = fitnessValues[candidateIndex];//pull out this candidates fitness from the fitness value table
        if (candidateFitness > bestFitness) {//if the fitness of the new candidate is better than the old one
            bestIndex = candidateIndex;//best candidate is the new one
            bestFitness = candidateFitness;//best fitness is now the new ones fitness
        }
    }

    return population[bestIndex];//return the optimal chromosome from the tournament
}



int main() {
    //std::cout<<"ping here"; Debug
    const int populationSize = 1000;
    const int chromosomeLength = 10;
    const int generations = 1000;
    const float mutationRate = 0.05;
    const int tournamentSize = 50;
   // std::cout<<"ping here 1"; Debug
    // Initialize the first population - two dimensional vector containing our geneomes
    std::vector<Chromosome> population = initializePopulation(populationSize, chromosomeLength);
    //std::cout<<"ping here 2"; Debug
    //Determine the fitness of our initial population
    std::vector<float> population_fitness = fitness_values(population);
    //std::cout<<"ping here 3"; Debug
    //For a set amount of generations
    for (int generation = 0; generation < generations; ++generation) {
        std::vector<Chromosome> newPopulation;//Create an empty 2D vector to hold our new genome population
        // Create new population
        while (newPopulation.size() < populationSize) {//While the new population is less that the old population
            Chromosome parent1 = tournamentSelection(population,population_fitness,tournamentSize);//Decide Parent 1 through tournament selection
            Chromosome parent2 = tournamentSelection(population,population_fitness, tournamentSize);//Decide Parent 2 through tournament selection


            Chromosome offspring = crossover(parent1, parent2);//Create offspring through crossover

            mutate(offspring, mutationRate);//Apply mutation to the offspring

            newPopulation.push_back(offspring);//Push the new offspring into the new population list
        }

        population = std::move(newPopulation);//After the new generation has been fully created, replace it with out
        population_fitness = fitness_values(population);//We can now re-evaluate the populations fitness


        //Now we pick the best one

        int counter = 0;
        float best_fitness =0.0;

        for(int j =0;j<population_fitness.size();++j){
            float fit = population_fitness[j];//pull out the fitness

            if(fit>best_fitness){
                best_fitness = fit;
                counter = j;
            }
        }


        std::cout << "Generation " << generation + 1 << ": Best Fitness = " << best_fitness << "\n";
        std::cout << "found at location" << counter << "\n";
    }

    return 0;
}





