/* Federico Poli Dicembre 2010 */

#include "simulator.h"

Simulator::Simulator() {
    dt = 1;
    state = new State;
    old_state = new State;
    clear();
}

Simulator::~Simulator() {
    clear();
    delete state;
    delete old_state;
}

void Simulator::clear() {
    state->clear();
    old_state->clear();
    integrator = NULL;
    path.clear();
    path_length = 0;
    path_begin = 0;
    path_step = 0;
    count = 0;
    planet.clear();
    planet_size = 0;
}

void Simulator::reset() {
    // Reset path
    path.clear();
    path_begin = 0;
    count = 0;

    // Reset states
    init_state();
}

// ======= Salva la configurazione =======

void Simulator::update_configuration(Json::Value &conf) {
    // Aggiorna posizione e velocità per tutti i pianeti
    for ( unsigned int index = 0; index < planet_size; ++index ) {
        for ( unsigned int j=0 ; j<3; ++j ) {
            conf["planets"][index]["pos"][j] = (double)state->pos[index][j];
        }
        for ( unsigned int j=0 ; j<3; ++j ) {
            conf["planets"][index]["vel"][j] = (double)state->vel[index][j];
        }
    }
}

// ======= Configurazione iniziale =======

void Simulator::load_configuration(const Json::Value conf) {
    // Aggiungi tutti i pianeti
    const Json::Value planets = conf["planets"];
    Planet p;
    Vector v;
    for ( unsigned int index = 0; index < planets.size(); ++index ) {
        p.mass = planets[index].get( "mass", 5E+24 ).asDouble();
        p.radius = planets[index].get( "radius", 5000000 ).asDouble();
        if (planets[index].isMember("color"))
            for(int i=0; i<3; i++)
                p.color[i] = planets[index].get( "color", Json::Value(1) ).get( Json::UInt(i), 1 ).asDouble();
        v = 0;
        for(int i=0; i<3; i++) v[i] = planets[index].get( "pos", Json::Value(0) ).get( Json::UInt(i), 0 ).asDouble();
        p.pos = v;
        v = 0;
        for(int i=0; i<3; i++) v[i] = planets[index].get( "vel", Json::Value(0) ).get( Json::UInt(i), 0 ).asDouble();
        p.vel = v;
        snprintf(p.name, sizeof(p.name), "%s", planets[index].get( "name", "Planet" ).asString().c_str() );
        insert_planet(p);
    }
    if (planets.size() > 0)    printf("Inseriti %d pianeti\n", planets.size());
    else printf("Atenzione: non è stato inserito nessun pianeta!\n");

    if (conf.get( "delete_center_mass_movement", 0 ).asBool()) {
        delete_center_mass_movement();
    }

    set_integrator( conf.get( "integrator", "rungekutta" ).asString().c_str() );
    set_dt(conf.get( "dt", (double) dt ).asDouble() );
    path_length = conf.get( "path_length", path_length ).asInt();
    path_step = conf.get( "path_step", path_step ).asInt();
}

unsigned int Simulator::insert_planet(Planet p) {
    printf("Aggiunto il pianeta '%s' massa: %.2Le raggio: %.2Le pos: (%.2Le, %.2Le, %.2Le) vel: (%.2Le, %.2Le, %.2Le) col: (%.2Le, %.2Le, %.2Le)\n",
        p.name,p.mass,p.radius, p.pos[0],p.pos[1],p.pos[2], p.vel[0],p.vel[1],p.vel[2], p.color[0],p.color[1],p.color[2]);
    planet.push_back(p);
    ++planet_size;
    return planet_size;
}

Planet Simulator::get_original_planet(unsigned int i) {
    #ifndef NDEBUG
    if (i >= get_planet_number()) {
        error("[Simulator::get_original_planet] L'elemento %u non esiste (min 0, max %u)\n", i, get_planet_number());
    }
    #endif
    return planet[i];
}

unsigned int Simulator::get_planet_number() {
    #ifndef NDEBUG
    if (planet_size != planet.size() || planet_size != state->size() || planet_size != old_state->size()) {
        error("[Simulator::get_planet_number] I valori planet_size, planet.size(), state->size(), old_state->size() sono diversi (%u, %u, %u e %u)\n", planet_size, planet.size(), state->size(), old_state->size());
    }
    #endif
    return planet_size;
}

// calcola e annulla il movimento del centro di massa
void Simulator::delete_center_mass_movement() {
    Vector cm_vel; cm_vel = 0;
    Real cm_mass = 0;
    for (int i=0; i<(int)planet.size(); ++i) {
        cm_vel += planet[i].vel * planet[i].mass;
        cm_mass += planet[i].mass;
    }
    cm_vel /= cm_mass;
    for (int i=0; i<(int)planet.size(); ++i) {
        planet[i].vel -= cm_vel;
    }
    printf("Compensata la velocità del centro di massa: (%.2Le, %.2Le, %.2Le)\n", cm_vel[0],cm_vel[1],cm_vel[2]);
}

// ======= Stato del sistema =======

void Simulator::init_state() {
    state->clear();
    old_state->clear();
    for (unsigned int i=0; i<planet.size(); ++i) {
        state->insert_particle(planet[i]);
        old_state->insert_particle(planet[i]);
        #ifndef NDEBUG
        if (i+1 != state->size() || i+1 != (old_state->size())) {
            error("[Simulator::init_state] Al passo %d gli elementi hanno dimensioni diverse (%d, %d)\n",
                i+1, state->size(), old_state->size() );
        }
        #endif
    }
}

Planet Simulator::get_planet(unsigned int i) {
    #ifndef NDEBUG
    if (i >= get_planet_number()) {
        error("[Simulator::get_planet] L'elemento %u non esiste (min 0, max %u)\n", i, get_planet_number());
    }
    #endif
    // Take mass, radius, ... from planet[i], then position, velocity, ...
    // from state->get_particle(i)
    return Planet(planet[i], state->get_particle(i));
}

// ======= Tracciato =======

void Simulator::save_path_point() {
    if (path_length == 0) return;
    if (path.size() < path_length) {
        PathPoint mem;
        mem.time = state->time;
        mem.position.reserve(state->size());
        for (unsigned int i=0; i < state->size(); ++i) {
            mem.position.push_back(state->get_particle(i).pos);
        }
        path.push_back(mem);
    } else {
        int t = path_begin;
        path_begin = (path_begin+1) % path.size();
        for (unsigned int i=0; i < state->size(); ++i) {
            path[t].position[i] = state->get_particle(i).pos;
        }
    }
}

const PathPoint * Simulator::get_path_point(int i) {
    #ifndef NDEBUG
    if (path.size() == 0) {
        error("[Simulator::get_path_point] Il punto %d non esiste: la traccia è vuota\n", i);
    }
    #endif
    int t = (path.size()+path_begin-i-1) % path.size();
    #ifndef NDEBUG
    if (t+1 > (int)path.size()) {
        error("[Simulator::get_path_point] Il punto %d della traccia che è stato richiesto non esiste [range: 0..%d]\n",
            i, path.size()-1);
    }
    #endif
    return &path[t];
}

void Simulator::set_path_step(int _step) {
    path_step = _step;
}

unsigned int Simulator::get_path_length() {
    return path.size();
}

unsigned int Simulator::get_max_path_length() {
    return path_length;
}

// ======= Simulatore =======

void Simulator::set_dt(Real _dt) {
    dt = _dt;
}

Real Simulator::get_time() {
    return state->time;
}

void Simulator::set_integrator(const char method[]) {
    integrator = get_method(method);
    if (integrator == NULL) {
        // dipende da un parametro del file di configurazione
        // non chiudere tra #ifndef NDEBUG #endif !
        error("[Simulator::set_integrator] metodo '%s' non riconosciuto\n", method);
    }
}

void Simulator::evolve(Real _dt) {
    // dipende da un parametro del file di configurazione
    // non chiudere tra #ifndef NDEBUG #endif !
    if (integrator == NULL) error("[Simulator::evolve] E' necessario specificare un metodo per l'integrale\n");

    // swap state <--> old_state
    State * old_tmp;
    old_tmp = old_state;
    old_state = state;
    state = old_tmp;
    // state = funct(old_state, _dt)
    integrator(*state, *old_state, _dt);

    // Salva il tracciato
    if (path_step > 0) {
        count++;
        if (count >= path_step) {
            save_path_point();
            count = 0;
        }
    }
}
void Simulator::evolve() {
    evolve(dt);
}

