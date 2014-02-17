#include <cstdio>
#include <cmath>
#include "chromosome.h"

double* fitness(Chromosome* pop, int popSize)
{
	double* ret = new double[popSize];
	for(int i = 0; i < popSize; i++)
	{
		Chromosome contender1 = pop[i];
		Chromosome contender2 = pop[++i];
		double fitness1 = 0;
		double fitness2 = 0;

		if(!contender1 && !contender2)
		{
			fitness1 = 1;
			fitness2 = 1;
		}
		else if(contender1 && !contender2)
		{
			fitness2 = 5;
		}
		else if(contender1 && contender2)
		{
			fitness1 = 3;
			fitness2 = 3;
		}
		else if(!contender1 && contender2)
		{
			fitness1 = 5;
		}

		ret[i - 1]	= 1.0 / (fitness1 + 1);
		ret[i]		= 1.0 / (fitness2 + 1);
	}
	return ret;
}

int main(int argc, char **argv)
{
	Population dilemma(32, 1, 0.01, 0.4, fitness);
	dilemma.run(10);
}