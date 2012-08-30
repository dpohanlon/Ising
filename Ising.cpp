#include <iostream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <math.h>

#include "Atom.h"
#include "Lattice.h"
#include "mt.h"

using namespace std;

bool accept(double dE, double T, double randN);

double calculateH(Lattice l);

int main(int argc, char const *argv[])
{
	MersenneTwister mt;

	ofstream outfile;

	outfile.open("isingM_3_6.dat");

	for(int Ti = 30; Ti < 60; ++Ti){
		double T = 0.1*Ti;
		//double T = 4.0;
		// double T = 1000;
		Lattice l(20);
		double E = 0;
		double E2 = 0;
		double m = 0;
		double m2 = 0;

		// INIT
		for (int i = 0; i < 20; ++i){
			for (int j = 0; j < 20; ++j){
				m+= l.getSpin(i, j);
				// E
			}
		}

		// MC STEPS
		for(int s = 0; s < 100000; ++s){
		
			for (int i = 0; i < 20; ++i){
				for (int j = 0; j < 20; ++j){

					int x = mt.random()*20.;
					int y = mt.random()*20.;

					double dE = 0;
					dE = 2 * l.getSpin(x, y) * 
							(l.getSpin(x - 1, y) +
							l.getSpin(x + 1, y) +
							l.getSpin(x, y - 1) +
							l.getSpin(x, y + 1) );

					if(accept(dE, T, mt.random())){
						m += 2 * -l.getSpin(x, y);
						m2 += (2 * -l.getSpin(x, y)) * (2 * -l.getSpin(x, y));
						E += dE;
						E2 += dE*dE;
						l.setSpin(x, y, -l.getSpin(x, y));
					}


				}
			}
		}
		double Cv = (1./(T*T))*(E2/(pow(20,2)) - E*E/(pow(20,4))); // Heat capacity
		double X = (1./T)*(m2/(pow(20,2)) - m*m/(pow(20,4))); // Magnetic susceptibility

		outfile << T << "\t" << fabs(m/(20*20)) << "\t"
				<< Cv << "\t"
				<< X << endl;
	}

	outfile.close();

	return 0;
}

bool accept(double dE, double T, double randN)
{
	double k = 1.0;

	if (dE <= 0){
		return true;
	}
	else if(exp(-dE / (k*T)) >= randN){
		return true;
	}
	else{
		return false;
	}
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