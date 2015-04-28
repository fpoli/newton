/* Federico Poli Dicembre 2010 */

#include "state.h"

State::State() { clear(); }
State::State(unsigned int n) { clear(); reserve(n); }
State::~State() { clear(); }
	
void State::clear() {
	pos.clear();
	vel.clear();
	mass.clear();
	time = 0;
	mat_size = 0;
}
void State::reserve(unsigned int n) {
	pos.reserve(n);
	vel.reserve(n);
	mass.reserve(n);
}
unsigned int State::size() const {
/*
	if ((pos.size() != vel.size()) || (pos.size() != mass.size()) ) {
		error("[State::size] Gli elementi hanno dimensioni diverse (%d, %d, %d)\n",
			pos.size(), vel.size(), mass.size()
		);
	}
	return pos.size();
*/
	#ifndef NDEBUG
	if (mat_size != mass.size() || mat_size != pos.size() || mat_size != vel.size()) {
		error("[State::size] I valori di mat_size, mass.size(), pos.size() e vel.size() sono diversi (%u, %u, %u e %u)\n",
			mat_size, mass.size(), pos.size(), vel.size() );
	}
	#endif
	return mat_size;
}

void State::insert_particle(const Particle & p) {
	++mat_size;
	pos.insert(p.pos);
	vel.insert(p.vel);
	mass.push_back(p.mass);
	#ifndef NDEBUG
	if (pos.size() != vel.size() || pos.size() != mass.size()) {
		error("[State::insert] Gli elementi hanno dimensioni diverse (%d, %d, %d)\n", pos.size(), vel.size(), mass.size());
	}
	#endif
}
Particle State::get_particle(unsigned int i) const {
	#ifndef NDEBUG
	if (i >= size()) {
		error("[State::get_particle] L'elemento %u non esiste (min 0, max %u)\n", i, size());
	}
	#endif
	return Particle(mass[i], pos(i), vel(i));
}

/*
	La funzione più chiamata di tutte
*/
Matrix State::calc_acc() const {
	static Matrix acc;
	calc_acc(acc);
	return acc;
}
void State::calc_acc(Matrix & acc) const {
	acc.resize(size());
	Vector dist, tmp;
	// per ogni coppia di pianeti j --> k
	for (unsigned int j=0; j<size(); ++j) {
		acc[j] = 0;
		for (unsigned int k=0; k<size(); ++k) {
			if (j == k) continue; // salta se stesso!
			dist = pos(k); dist -= pos(j);
			acc[j] += (G * mass[k] / dist.sqr_module()) * dist.unit();
			//acc[j] += G * dist.unit() * dist.module() ;
			//acc[j] += G / mass[j] * dist.unit() * dist.module() ;
			#ifndef NDEBUG
				if (acc[j].module() >= 300 *1000 *1000) {
					printf("[State::calc_acc] L'accelerazione di #%u è troppo grande (%e m/s^2)\n", j, acc[j].module());
				}
			#endif
		}
	}
}

