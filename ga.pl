use strict;
use warnings;
use List::Util qw( sum shuffle );
use List::Util::WeightedChoice qw( choose_weighted );

###################################################################
###################################################################

my $entropy = 1;
my $pc = 0;
my $pm = 0.01;

###################################################################
###################################################################

# fitness($chromosome)
sub fitness {
	my $chromosome = shift(@_);
	return 1.0 / (1+abs($chromosome-255));
}

# 0 First move
# 1 Opponent First Move C
# 2 Opponent First Move D
# 3 Opponent First Moves CC
# 4 Opponent First Moves CD
# 5 Opponent First Moves DC
# 6 Opponent First Moves DD
# 7-70 Iterations up to max
# 1 is defect, 0 is cooperate

# evaluateFitness(@population)
sub evaluateFitness {
	my @population = @_;
	my @fitness = ();
	@population = shuffle @population;
	for(my $i = 0; $i < scalar(@population); $i++)
	{
		my $contender1 = $population[$i];
		my $contender2 = $population[++$i];

		my $fitness1 = 0;
		my $fitness2 = 0;

		#Move 1 block
		
		my @moves = ($contender1 & 0b1, $contender2 & 0b1);

		if($moves[0] == 0 && $moves[1] == 0)
		{
			$fitness1 += 1;
			$fitness2 += 1;
		}
		elsif($moves[0] == 1 && $moves[1] == 0)
		{
			$fitness2 += 5;
		}
		elsif($moves[0] == 1 && $moves[1] == 1)
		{
			$fitness1 += 3;
			$fitness2 += 3;
		}
		elsif($moves[0] == 0 && $moves[1] == 1)
		{
			$fitness1 += 5;
		}

		push(@fitness, 1.0 / ($fitness1 + 1));
		push(@fitness, 1.0 / ($fitness2 + 1));
	}
	return @fitness;
}

###################################################################
###################################################################

# initPopulation($populationSize)
sub initPopulation {
	my @population = ();
	my $populationSize = shift(@_);
	for(my $i = 0; $i < $populationSize; $i++)
	{
		push(@population, int(rand(2**$entropy)));
	}
	return @population;
}

# evolve(@population)
sub evolve {
	my @population = @_;
	my @fitness = ();

	@fitness = evaluateFitness(@population);

	my @intermediate = ();
	for(my $i = 0; $i < scalar(@population); $i++)
	{
		push(@intermediate, choose_weighted(\@population, \@fitness));
	}

	@population = shuffle @population;

	for(my $i = 0; $i < scalar(@intermediate); $i++)
	{
		if(rand() <= $pc && ($i != scalar(@intermediate) - 1))
		{
			my $c = int(rand($entropy - 1));
			my $cut1 = $intermediate[$i] & (2**($c+1)-1);
			my $cut2 = $intermediate[$i + 1] & (2**($c+1)-1);
			$intermediate[$i] += ($cut2 - $cut1);
			$intermediate[++$i] += ($cut1 - $cut2);

		}
		if(rand() <= $pm)
		{
			$intermediate[$i] ^= 2**int(rand($entropy));
		}
	}

	return @intermediate;
}

###################################################################
###################################################################

my @pop = initPopulation(32);
print "@pop\n\n";
for(my $i = 0; $i < 2; $i++)
{
	@pop = evolve(@pop);
	print "@pop\n\n";
}
