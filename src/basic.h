/* Federico Poli Dicembre 2010 */

#ifndef NEWTON_BASIC_H
#define NEWTON_BASIC_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <stdarg.h>

// Precisione
#define Real long double

#define sqr(a) ((a)*(a))
#define abs(a) ((a)>0?(a):(-a))

#define PI (3.1415926535)


int clamp(int val, int min, int max);

// Stampa un errore ed esci
void error(const char* msg, ...);
void error(char* msg, ...);

class Vector {
private:
	Real vec[3];
public:
	// costruttori
	Vector() { vec[0] = 0; vec[1] = 0; vec[2] = 0; }
	Vector(const Real & r) { vec[0] = r; vec[1] = r; vec[2] = r; }
	Vector(Real _x, Real _y, Real _z)  { vec[0] = _x; vec[1] = _y; vec[2] = _z; }
	void x(Real _x) { vec[0] = _x; }
	void y(Real _y) { vec[1] = _y; }
	void z(Real _z) { vec[2] = _z; }
	// distruttore
	~Vector() { }
	// operazioni
	Vector & operator = (const Vector & v) { vec[0] = v(0); vec[1] = v(1); vec[2] = v(2); return *this; }
	Vector & operator = (const Real & r) { vec[0] = r; vec[1] = r; vec[2] = r; return *this; }
	Vector operator + (const Vector & v) const { return Vector(vec[0] + v(0), vec[1] + v(1), vec[2] + v(2)); }
	Vector operator - () const { return Vector(-vec[0], -vec[0], -vec[2]); }
	Vector operator - (const Vector & v) const { return Vector(vec[0] - v(0), vec[1] - v(1), vec[2] - v(2)); }
	Vector operator * (const Real & r) const { return Vector(vec[0] * r, vec[1] * r, vec[2] * r); }
	Vector operator / (const Real & r) const  { return Vector(vec[0] / r, vec[1] / r, vec[2] / r); }
	void operator += (const Vector & v) { vec[0] += v(0); vec[1] += v(1); vec[2] += v(2); }
	void operator -= (const Vector & v) { vec[0] -= v(0); vec[1] -= v(1); vec[2] -= v(2); }
	void operator *= (const Real & r) { vec[0] *= r; vec[1] *= r; vec[2] *= r; }
	void operator /= (const Real & r) { vec[0] /= r; vec[1] /= r; vec[2] /= r; }
	bool operator == (const Vector & v) { return (vec[0] == v(0)) && (vec[1] == v(1)) && (vec[2] == v(2)); }
	Real operator () (unsigned int i) const { // read-only !!
		#ifndef NDEBUG
		if (i >= 3) {
			error("[Vector::operator()] L'elemento %u non esiste (min 0, max 3)\n", i);
		}
		#endif
		return vec[i];
	}
	Real & operator [] (int i) {
		#ifndef NDEBUG
		if (i >= 3) {
			error("[Vector::operator[]] L'elemento %u non esiste (min 0, max 3)\n", i);
		}
		#endif
		return vec[i];
	}
	friend Vector operator * (const Real & r, const Vector & v) { return v * r; }

	Real sqr_module() { return sqr(vec[0]) + sqr(vec[1]) + sqr(vec[2]); }
	Real module() { return sqrt( sqr(vec[0]) + sqr(vec[1]) + sqr(vec[2]) ); }
	Vector unit() { /*return (*this)/module();*/ Real m = module(); return Vector(vec[0] / m, vec[1] / m, vec[2] / m); }
};

class Point {
public:
	Vector pos, vel;
	Point() {}
	~Point() {}
};

class Particle : public Point {
public:
	Real mass;
	Particle() { mass = 1; /* per evitare accidentali divisioni per zero */ }
	Particle(Real _mass, Vector _pos, Vector _vel) {
		mass = _mass;
		pos = _pos;
		vel = _vel;
	}
	~Particle() {}
};

class Planet : public Particle {
public:
	Real radius;
	char name[256];
	Vector color; // colore della scia

	Planet() : Particle(), color(1) {
		radius = 1;
		name[0] = '\0';
	}
	Planet(const Planet & pl, const Particle & p) : Particle(p) {
		radius = pl.radius;
		snprintf(name, sizeof(name), "%s", pl.name);
		color = pl.color;
	}
	Planet(Real _mass, Real _radius, char _name[], Vector _pos, Vector _vel) : Particle (_mass, _pos, _vel) {
		radius = _radius;
		snprintf(name, sizeof(name), "%s", _name);
	}
	~Planet() {}
	// Assegnazione
	Planet & operator = (const Particle & p) {
		pos = p.pos;
		vel = p.vel;
		mass = p.mass;
		return *this;
	}
};

/*
	Memorizza un punto della scia di tutti i pianeti
*/
struct PathPoint {
	Real time; // tempo al quale è stato generato il punto
	std::vector<Vector> position; // posizione di ciascun pianeta
};

/*
	operators += -= *= /= are faster than + - * /
*/
class Matrix {
private:
	std::vector<Vector> vec;
	unsigned int vec_size;
public:
	Matrix() { vec_size = 0; }
	~Matrix() { }
	void clear() { vec.clear(); vec_size = 0; }
	void reserve(unsigned int n) { vec.reserve(n); }
	void resize(unsigned int n) { vec.resize(n); vec_size = n; }
	//void resize(unsigned int n, const Vector &val) { vec.resize(n,val); vec_size = n; }
	unsigned int size() const {
		#ifndef NDEBUG
		if (vec_size != vec.size()) {
			error("[Matrix::size] I valori di vec_size e size() sono diversi (%u e %u)\n", vec_size, vec.size());
		}
		#endif
		return vec_size;
	}
	void insert(const Vector & v) { vec.push_back(v); ++vec_size; }
	// operazioni
	Matrix & operator = (const Matrix & m) {
		if (this != &m) { // se non è se stesso
			// Togli quelli in più
			if (vec_size > m.size()) {
				vec.resize(m.size()); // cancella quelli di troppo
				vec_size = m.size();
			}
			// Copia quelli che ci sono
			for (unsigned int i=0; i<vec_size; ++i) {
				vec[i] = m(i);
			}
			// Aggiungi quelli che mancano
			vec.reserve(m.size());
			for (; vec_size<m.size(); ++vec_size) {
				vec.push_back(m(vec_size));
			}
		}
		return *this;
	}
	Matrix & operator = (const Real & r) {
		for (unsigned int i=0; i<size(); ++i) {
			vec[i] = r;
		}
		return *this;
	}
	Matrix operator + (const Matrix & m) const {
		Matrix ret;
		#ifndef NDEBUG
		if (size() != m.size()) {
			error("[Matrix::operator +] Le matrici hanno un numero di righe diverso (%d e %d)\n", size(), m.size());
		}
		#endif
		ret.reserve(size());
		for (unsigned int i=0; i<size(); ++i) {
			ret.insert(vec[i]+m(i));
		}
		return ret;
	}
	Matrix operator - () const {
		Matrix ret;
		ret.reserve(size());
		for (unsigned int i=0; i<size(); ++i) {
			ret.insert(-vec[i]);
		}
		return ret;
	}
	Matrix operator - (const Matrix & m) const {
		Matrix ret;
		#ifndef NDEBUG
		if (size() != m.size()) {
			error("[Matrix::operator -] Le matrici hanno un numero di righe diverso (%d e %d)\n", size(), m.size());
		}
		#endif
		ret.reserve(size());
		for (unsigned int i=0; i<size(); ++i) {
			ret.insert(vec[i]-m(i));
		}
		return ret;
	}
	Matrix operator * (const Real & r) const {
		Matrix ret;
		ret.reserve(size());
		for (unsigned int i=0; i<size(); ++i) {
			ret.insert(vec[i] * r);
		}
		return ret;
	}
	Matrix operator / (const Real & r) const {
		Matrix ret;
		ret.reserve(size());
		for (unsigned int i=0; i<size(); ++i) {
			ret.insert(vec[i] / r);
		}
		return ret;
	}
	void operator += (const Matrix & m) {
		#ifndef NDEBUG
		if (size() != m.size()) {
			error("[Matrix::operator +=] Le matrici hanno un numero di righe diverso (%d e %d)\n", size(), m.size());
		}
		#endif
		for (unsigned int i=0; i<size(); ++i) {
			vec[i] += m(i);
		}
	}
	void operator -= (const Matrix & m) {
		#ifndef NDEBUG
		if (size() != m.size()) {
			error("[Matrix::operator -=] Le matrici hanno un numero di righe diverso (%d e %d)\n", size(), m.size());
		}
		#endif
		for (unsigned int i=0; i<size(); ++i) {
			vec[i] -= m(i);
		}
	}
	void operator *= (const Real & r) {
		for (unsigned int i=0; i<size(); ++i) {
			vec[i] *= r;
		}
	}
	void operator /= (const Real & r) {
		for (unsigned int i=0; i<size(); ++i) {
			vec[i] /= r;
		}
	}

	friend Matrix operator * (const Real & r, const Matrix & m) { return m * r; }

	Vector operator () (unsigned int i) const {  // read-only !!
		#ifndef NDEBUG
		if (i >= size()) {
			error("[Matrix::operator()] L'elemento %u non esiste (min 0, max %u)\n", i, size());
		}
		#endif
		return vec[i];
	}
	Vector & operator [] (unsigned int i) {
		#ifndef NDEBUG
		if (i >= size()) {
			error("[Matrix::operator[]] L'elemento %u non esiste (min 0, max %u)\n", i, size());
		}
		#endif
		return vec[i];
	}
};

#endif
