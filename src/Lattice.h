#ifndef LATTICE_H
#define LATTICE_H

#include <vector>

#include "Atom.h"
#include "mt.h"

using namespace std;

class Lattice
{
public:
	Lattice(int N);
	int getSpin(int r, int c);
	void setSpin(int r, int c, int s){ atoms[r][c].setSpin(s); }
	int getSize(){ return N; }

private:
	int N;
	vector<vector<Atom> > atoms;
};

#endif
