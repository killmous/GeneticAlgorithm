#include <cstdio>
#include <cmath>
#include "chromosome.h"

bool bitAt(int offset, Chromosome chromo)
{
	return (chromo >> (offset - 1)) & 0b1;
}

int population = 64;
int iterations = 500;
int entropy = 5;
double pm = 0.01;
double pc = 0.4;
double* fitness(Chromosome* pop, int popSize)
{
	int tournamentLength = 32;

	double* ret = new double[popSize];
	for(int i = 0; i < popSize; i++)
	{
		Chromosome contender1 = pop[i];
		Chromosome contender2 = pop[++i];
		double fitness1 = 0;
		double fitness2 = 0;

		int move1 = bitAt(1, contender1);
		int move2 = bitAt(1, contender2);

		for(int j = 0; j < tournamentLength; ++j)
		{
			if(!move1 && !move2)
			{
				fitness1 += 1;
				fitness2 += 1;
			}
			else if(move1 && !move2)
			{
				fitness2 += 5;
			}
			else if(move1 && move2)
			{
				fitness1 += 3;
				fitness2 += 3;
			}
			else if(!move1 && move2)
			{
				fitness1 += 5;
			}

			if(j != tournamentLength - 1)
			{
				int temp1 = move1;
				int temp2 = move2;

				if(temp1 && temp2) //If opponent defect and i defect
				{
					move1 = bitAt(2, contender1);
					move2 = bitAt(2, contender2);
				}
				else if(temp1 && !temp2) //If i defect and he cooperate
				{
					move1 = bitAt(4, contender1);
					move2 = bitAt(3, contender2);
				}
				else if(!temp1 && !temp1)
				{
					move1 = bitAt(5, contender1);
					move2 = bitAt(5, contender2);
				}
				else if(!temp1 && temp2)
				{
					move1 = bitAt(3, contender1);
					move2 = bitAt(4, contender2);
				}
			}
		}

		ret[i - 1]	= 1.0 / (fitness1 + 1);
		ret[i]		= 1.0 / (fitness2 + 1);
	}
	return ret;
}

// bit 1 -> first move
// bit 2 -> move if both defect
// bit 3 -> move if he defect i cooperate
// bit 4 -> move if i defect he cooperate
// bit 5 -> nove if both cooperate

int main(int argc, char **argv)
{
	Population dilemma(population, entropy, pm, pc, fitness);
	dilemma.run(iterations, 1);
}