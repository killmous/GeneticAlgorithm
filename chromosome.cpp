#include "chromosome.h"

#include <random>

//
//Population 
//

Population::Population(	int populationSize,
						int entropy,
						double pm,
						double pc,
						double* (*fitnessFunc)(Chromosome*, int))
{
	_populationSize = populationSize;
	_entropy = entropy;
	_population = new Chromosome[_populationSize];
	_pmutation = pm;
	_pcrossover = pc;
	_fitnessFunc = fitnessFunc;

	std::random_device rd;
	for(int i = 0; i < _populationSize; ++i)
	{
		_population[i] = int(rd()/double(rd.max())*(1 << entropy));
	}
}

Population::~Population(void)
{
	delete[] _population;
}

void Population::run(int iterations, bool debug)
{
	printf("Initial population: ");
	this->print();
	for(int i = 0; i < iterations; ++i)
	{
		this->evolve();
		if(debug) this->print();
	}
	printf("Final population: ");
	this->print();
}

void Population::print(void)
{
	for(int i = 0; i < _populationSize; ++i)
	{
		printf("%d ", _population[i]);
	} printf("\n");
}

void Population::evolve(void)
{
	double* fitness = _fitnessFunc(_population, _populationSize);

	Chromosome* intermediate = new Chromosome[_populationSize];
	for(int i = 0; i < _populationSize; ++i)
	{
		Chromosome chromo = chooseWeighted(_population, fitness, _populationSize);
		intermediate[i] = chromo;
	}

	std::random_device rd;
	for(int i = 0; i < _populationSize; ++i)
	{
		if(rd()/double(rd.max()) <= _pcrossover && i != _populationSize - 1)
		{
			int crossoverPoint = int(rd()/double(rd.max())*(_entropy - 1));
			Chromosome cut1 = intermediate[i] & ((1 << (crossoverPoint + 1)) - 1);
			Chromosome cut2 = intermediate[i + 1] & ((1 << (crossoverPoint + 1)) - 1);
			intermediate[i] += (cut2 - cut1);
			intermediate[++i] += (cut1 - cut2);
		}

		if(rd()/double(rd.max()) <= _pmutation)
		{
			intermediate[i] ^= (1 << int(rd()/double(rd.max())*_entropy));
		}
	}

	_population = intermediate;
}

Chromosome chooseWeighted(Chromosome* pop, double* fitness, int size)
{
	double sumOfWeights = 0;
	for(int i = 0; i < size; ++i)
	{
		sumOfWeights += fitness[i];
	}

	std::random_device rd;
	double rnd = rd()/double(rd.max())*sumOfWeights;
	for (int i = 0; i < size; ++i)
	{
		if (rnd < fitness[i])
		{
			return pop[i];
		}
		rnd -= fitness[i];
	}
	//If it gets here, we're boned
}
