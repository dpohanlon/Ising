#ifndef ATOM_H
#define ATOM_H

class Atom
{
public:
	Atom(){};
	Atom(int spin);
	int getSpin(){ return spin; }
	void setSpin(int spin) { this->spin = spin; }

private:
	int spin;
};

#endif
