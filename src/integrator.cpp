/* Federico Poli Dicembre 2010 */

#include "integrator.h"

void euler(State & res, const State & state, Real dt) {
	res.pos = state.pos + dt * state.vel;
	res.vel = state.vel + dt * state.calc_acc();
	res.time = state.time + dt;
}

void euler2(State & res, const State & state, Real dt) {
	Matrix acc = state.calc_acc();
	res.pos = state.pos + dt * state.vel + sqr(dt)/2 * acc;
	res.vel = state.vel + dt * acc;
	res.time = state.time + dt;
}

void leapfrog(State & res, const State & state, Real dt) {
	Matrix acc_i = state.calc_acc();
	res.pos = state.pos + dt * state.vel + sqr(dt)/2 * acc_i;
	res.vel = state.vel + dt * acc_i;
	Matrix acc_f = res.calc_acc();
	res.pos = state.pos + dt * state.vel + sqr(dt)/2 * (acc_i + acc_f)/2;
	res.vel = state.vel + dt * (acc_i + acc_f)/2;
	res.time = state.time + dt;
}

void rungekutta(State & res, const State & state, Real dt) {
	static Matrix k1, k2, k3, k4;
/*
	// Non ottimizzato:
	state.calc_acc(k1);
	res.pos = state.pos + dt/2 * state.vel + sqr(dt/2)/2 * k1;
	res.vel = state.vel; res.vel += dt/2 * k1;

	res.calc_acc(k2);
	res.pos = state.pos + dt/2 * state.vel + sqr(dt/2)/2 * k2;
	res.vel = state.vel + dt/2 * k2;

	res.calc_acc(k3);
	res.pos = state.pos + dt * state.vel + sqr(dt)/2 * k3;
	res.vel = state.vel + dt * k3;

	res.calc_acc(k4);
	res.pos = state.pos + dt * state.vel + sqr(dt)/2 * (k1 + 2*k2 + 2*k3 + k4)/6;
	res.vel = state.vel + dt * (k1 + 2*k2 + 2*k3 + k4)/6;

	res.time = state.time + dt;
*/
	static Matrix mat, som;

	state.calc_acc(k1);
	res.pos = state.pos;
	mat = state.vel; mat *= dt/2; res.pos += mat;
	mat = k1; mat *= sqr(dt/2)/2; res.pos += mat;
	res.vel = state.vel;
	mat = k1; mat *= dt/2; res.vel += mat;

	res.calc_acc(k2);
	res.pos = state.pos;
	mat = state.vel; mat *= dt/2; res.pos += mat;
	mat = k2; mat *= sqr(dt/2)/2; res.pos += mat;
	res.vel = state.vel;
	mat = k2; mat *= dt/2; res.vel += mat; res.vel += mat;

	res.calc_acc(k3);
	res.pos = state.pos;
	mat = state.vel; mat *= dt; res.pos += mat;
	mat = k3; mat *= sqr(dt)/2; res.pos += mat;
	res.vel = state.vel;
	mat = k3; mat *= dt; res.vel += mat;

	res.calc_acc(k4);
	res.pos = state.pos;
	mat = state.vel; mat *= dt; res.pos += mat;
	som = k2 ; som += k3; som *= 2; som += k1; som += k4; som /= 6;
	mat = som; mat *= sqr(dt)/2; res.pos += mat;
	res.vel = state.vel;
	mat = som; mat *= dt; res.pos += mat; res.vel += mat;

	res.time = state.time + dt;
}


func get_method(const char method[]) {
	if (strcmp(method, "euler") == 0) {
		return euler;
	} else
	if (strcmp(method, "euler2") == 0) {
		return euler2;
	} else
	if (strcmp(method, "leapfrog") == 0) {
		return leapfrog;
	} else
	if (strcmp(method, "rungekutta") == 0) {
		return rungekutta;
	}
	return NULL;
}

