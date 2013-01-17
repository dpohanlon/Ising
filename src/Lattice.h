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
    int getSize(void){ return N; }
	void setSpin(int r, int c, int s){ atoms[r][c] = s; }
    void addE(double e) { this->e += e; this->e2 += e*e; }
    void addM(double m) { this->m += m; this->m2 += m*m; }
    void setT(double T){ this->T = T; }
    double getE(void){ return e; }
    double getE2(void){ return e2; }
    double getM(void){ return m; }
    double getM2(void){ return m2; }
    double getT(void){ return T; }

private:
	int N;
    double T;
    double e;
    double e2;
    double m;
    double m2;

	vector<vector<int> > atoms;
};

#endif
