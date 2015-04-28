/* Federico Poli Dicembre 2010 */

#ifndef NEWTON_STATE_H
#define NEWTON_STATE_H

#include <stdio.h>
#include <vector>
#include "basic.h"

#define G 6.67300E-11

class State {
private:
	unsigned int mat_size;
public:
	Matrix pos, vel;
	std::vector<Real> mass;
	Real time;
	
	State();
	State(unsigned int n);
	~State();
	
	void clear();
	void reserve(unsigned int n);
	unsigned int size() const;
	
	void insert_particle(const Particle & p);
	Particle get_particle(unsigned int i) const;
	
	void calc_acc(Matrix &acc) const;
	Matrix calc_acc() const;
};

#endif
