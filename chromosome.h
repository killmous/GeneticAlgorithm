#ifndef __CHROMOSOME_H__
#define __CHROMOSOME_H__

typedef unsigned long Chromosome;

class Population
{
public:
	Population(	int populationSize,
				int entropy,
				double pm,
				double pc,
				double* (*fitnessFunc)(Chromosome*, int));
	~Population(void);

	void run(int iterations, bool debug = false);
	void print(void);
private:
	int _populationSize;
	int _entropy;
	Chromosome* _population;
	double _pmutation;
	double _pcrossover;
	double* (*_fitnessFunc)(Chromosome*, int);

	void evolve(void);
};

Chromosome chooseWeighted(Chromosome* pop, double* fitness, int size);

#endif