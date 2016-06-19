/* Federico Poli Dicembre 2010 */

#ifndef NEWTON_SIMULATOR_H
#define NEWTON_SIMULATOR_H

#include <stdio.h>
#include <vector>
#include <json/json.h>

#include "basic.h"
#include "state.h"
#include "integrator.h"

class Simulator {
private:
    Real dt; // step di incremento in secondi
    
    State *state, *old_state;
    void (*integrator)(State &, const State &, Real); // puntatore ad una funzione
    
    std::vector<Planet> planet; // nome, raggio, colore, posizione iniziale, ecc
    unsigned int planet_size;

    std::vector<PathPoint> path; // posizione [# salvataggio (0=ultimo, 1=penultimo, ...)] [pianeta] 
    int path_begin;
    unsigned int path_length;
    int path_step;
    
    int count;
public:
    Simulator();
    ~Simulator();
    void clear();
    void reset();
    
    void update_configuration(Json::Value &conf);
    
    void load_configuration(const Json::Value conf);
    unsigned int insert_planet(Planet p);
    Planet get_original_planet(unsigned int i);
    unsigned int get_planet_number();
    void delete_center_mass_movement();
    
    void init_state();
    Planet get_planet(unsigned int i);
    
    void save_path_point();
    const PathPoint * get_path_point(int i); // 0 = ultimo, 1 = penultimo, ...
    unsigned int get_path_length();
    unsigned int get_max_path_length();
    void set_path_step(int _step);
    
    void set_dt(Real _dt);
    Real get_time();
    void set_integrator(const char method[]);
    void evolve(Real _dt);
    void evolve();
};

#endif
