#include <cstdio>
#include <cmath>
#include "chromosome.h"

double* fitness(Chromosome* pop, int popSize)
{
	double* ret = new double[popSize];
	for(int i = 0; i < popSize; i++)
	{
		ret[i] = pow(2, -pop[i]);
	}
	return ret;
}

int main(int argc, char **argv)
{
	Population dilemma(64, 8, 0.01, 0.4, fitness);
	dilemma.run(10, 1);
}