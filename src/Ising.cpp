#include <iostream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

#include <boost/thread.hpp>

#include "ThreadPool.h"

#include "Atom.h"
#include "Lattice.h"
#include "mt.h"

using namespace std;

bool accept(int dE, double T, double randN);

double calculateH(Lattice l);

int deltaE(Lattice & l, int x, int y);

typedef void (*latFunc)(Lattice & l, int x, int y);

void tryFlip(Lattice & l, int x, int y);

void mapLattice(Lattice & l, latFunc f);

double myExp(double ex);

void runIsing(double T);

const int N = 128;
ofstream outfile;

int main(int argc, char const *argv[])
{
	outfile.open("ising.dat");
	ThreadPool threads(2);

	double T = 1.0;

	while (T < 5.0){
		threads.addJob(runIsing, T);
		T += 0.1;
	}

	threads.join();

	outfile.close();

	return 0;
}

void runIsing(double T)
{
	Lattice l(N);
	l.setT(T);

	// INIT
	for (int i = 0; i < N; ++i){
		for (int j = 0; j < N; ++j){
			l.addM(l.getSpin(i, j));
			// E
		}
	}

	// MC STEPS
	int step = 0;
	while (step < 10000){
		mapLattice(l, tryFlip);
		step++;
	}

	double Cv = (1./(T*T))*(l.getE2()/(pow(N,2.)) - l.getE()*l.getE()/(pow(N,4.))); // Heat capacity
	double X = (1./T)*(l.getM2()/(pow(N,2.)) - l.getM()*l.getM()/(pow(N,4.))); // Magnetic susceptibility

	outfile << T << "\t" << fabs(l.getM()/(N*N)) << "\t"
			<< Cv << "\t"
			<< X << endl;

	cout << "Done: " << T << endl;
}

void tryFlip(Lattice & l, int x, int y)
{
	static MersenneTwister mt;
	int dE = deltaE(l, x, y);
	if(accept(dE, l.getT(), mt.random())){
		l.addM(2 * -l.getSpin(x, y));
		l.addE(dE);
		l.setSpin(x, y, -l.getSpin(x, y));
	}
}

void mapLattice(Lattice & l, latFunc f)
{
	for (int x = 0; x < l.getSize(); ++x){
		for (int y = 0; y < l.getSize(); ++y){
			f(l, x, y);
		}
	}
}

bool accept(int dE, double T, double randN)
{
	double k = 1.0;

	if (dE <= 0){
		return true;
	}
	else if(myExp(-dE / (k*T)) >= randN){
		return true;
	}
	else{
		return false;
	}
}

double myExp(double ex) // Memoized
{
	static map<double, double> expM;

	map<double, double>::iterator loc;
	loc = expM.find(ex);

	if (loc != expM.end()){
		return loc->second;
	}
	else{
		double result = exp(ex);
		expM.insert(make_pair(ex, result));
		return result;
	}
}

int deltaE(Lattice & l, int x, int y)
{
	int dE = 2 * l.getSpin(x, y) * (
			 	 l.getSpin(x - 1, y) +
			 	 l.getSpin(x + 1, y) +
			 	 l.getSpin(x, y - 1) +
			 	 l.getSpin(x, y + 1) );

	return dE;
}

double calculateH(Lattice l)
{
	double J = 0.5;
	double first_term = 0;
	double second_term = 0;

	// first term for all r, c pairs

	// loop over r, c
	for (int r = 0; r < l.getSize(); ++r){
		for (int c = 0; c < l.getSize(); ++c){
			first_term += l.getSpin(r, c) * 
				l.getSpin(r - 1, c);

			first_term += l.getSpin(r, c) * 
				l.getSpin(r + 1, c);
				
			first_term += l.getSpin(r, c) * 
				l.getSpin(r, c - 1);
				
			first_term += l.getSpin(r, c) * 
				l.getSpin(r, c + 1);		
		}
	}

	first_term *= -J;

	for (int r = 0; r < l.getSize(); ++r){
		for (int c = 0; c < l.getSize(); ++c){
			second_term += l.getSpin(r, c);
		}
	}

	second_term *= -5.0; // -uH

	return first_term + second_term;
}
