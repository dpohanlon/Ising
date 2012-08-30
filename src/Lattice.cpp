#include "Lattice.h"

Lattice::Lattice(int N) : N(N)
{
	MersenneTwister mt;

	for (int i = 0; i < N; ++i){
		atoms.push_back(vector<Atom>(N));
	}

	for (int i 	= 0; i < N; ++i){
		for (int j = 0; j < N; ++j){
			// atoms[i][j].setSpin(mt.random() > 0.5 ? 1 : -1);
			atoms[i][j].setSpin(1);
		}	
	}
}

int Lattice::getSpin(int r, int c)
{
	return atoms[(N + r) % N][(N + c) % N].getSpin(); // Periodic boundary
}
