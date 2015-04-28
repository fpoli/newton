/* Federico Poli Dicembre 2010 */

#ifndef NEWTON_INTEGRATOR_H
#define NEWTON_INTEGRATOR_H

#include <string.h>
#include "basic.h"
#include "state.h"

typedef void (*func)(State &, const State &, Real);

void euler(State & res, const State & state, Real dt);

void euler2(State & res, const State & state, Real dt);

void leapfrog(State & res, const State & state, Real dt);

void rungekutta(State & res, const State & state, Real dt);

func get_method(const char method[]);

#endif
