#include "Lattice.h"

Lattice::Lattice(int N)
: N(N), T(0), e(0), e2(0), m(0), m2(0)
{
	MersenneTwister mt;

	for (int i = 0; i < N; ++i){
		atoms.push_back(vector<int>(N));
	}

	for (int i 	= 0; i < N; ++i){
		for (int j = 0; j < N; ++j){
			// atoms[i][j].setSpin(mt.random() > 0.5 ? 1 : -1);
			atoms[i][j] = 1;
		}	
	}
}

int Lattice::getSpin(int r, int c)
{
	return atoms[(N + r) % N][(N + c) % N]; // Periodic boundary
}
