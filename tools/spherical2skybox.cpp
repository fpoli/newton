#include "SDL.h"
#include "SDL_image.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double PI = 3.14159265;

int clamp(int val, int min, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}
double clamp(double val, double min, double max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

Uint8 * pixel (SDL_Surface *img, int x, int y) {
	x = clamp(x, 0, img->w);
	y = clamp(y, 0, img->h);
	/*printf(" %d %d --> %d + %d * %d + %d * %d --> %d\n",
		x, y,
		(int)img->pixels, y, (int)img->pitch, x, (int)img->format->BytesPerPixel,
		(int)img->pixels + y * (int)img->pitch + x * (int)img->format->BytesPerPixel
	);*/
	return (Uint8 *)img->pixels + y * img->pitch + x * img->format->BytesPerPixel;
}
Uint8 * pixel_polare (SDL_Surface *img, double rad_x, double rad_y) {
	while (rad_y > PI/2) { rad_y -= PI; rad_x += PI; }
	while (rad_y < -PI/2) { rad_y += PI; rad_x -= PI; }
	while (rad_x > 2*PI) rad_x -= 2*PI;
	while (rad_x < 0) rad_x += 2*PI;
	
	//printf("Angoli %.1f %.1f\n", rad_x*180/PI, rad_y*180/PI);
	return pixel(img, img->w-rad_x*img->w/(2*PI), img->h/2-rad_y*img->h/PI);
}

double pita_ipo(double a, double b) {
	return sqrt(a*a + b*b);
}
double pita_cat(double a, double b) {
	return sqrt(a*a - b*b);
}

int main(int argc, char *argv[])
{
	// Inizializza sdl
	assert(SDL_Init(SDL_INIT_VIDEO) != -1);
	atexit(SDL_Quit);
	
	// Carica la texture sferica (dove x e y sono le coordinate polari)
	assert(argv[1]);
	SDL_Surface *sphere = IMG_Load(argv[1]);
	assert(sphere);

	assert(argv[2]);
	int boxside = atoi(argv[2]);
	assert(boxside > 0);
	SDL_Surface *face;
	face = SDL_CreateRGBSurface(SDL_SWSURFACE, boxside, boxside, 32,0,0,0,0);
	assert(face);
	Uint8 *p, *d;
	
	printf("milkway_bk..\n");
	// Per ogni pixel dell'immagine
	for(int y=0; y<face->h; y++) {
		for(int x=0; x<face->w; x++) {
			/* Here p is the address to the pixel we want to set */
			p = pixel_polare(sphere, atan2(face->w/2-x, face->w/2), atan2(face->h/2-y, pita_ipo(face->w/2, face->w/2-x)));
			d = pixel(face, x, y);
			
			// Copia i 3 canali
			d[0] = p[2];
			d[1] = p[1];
			d[2] = p[0];
		}
	}
	SDL_SaveBMP(face, "milkway_bk.bmp");
	
	printf("milkway_rt..\n");
	// Per ogni pixel dell'immagine
	for(int y=0; y<face->h; y++) {
		for(int x=0; x<face->w; x++) {
			/* Here p is the address to the pixel we want to set */
			p = pixel_polare(sphere, atan2(face->w/2, x-face->w/2), atan2(face->h/2-y, pita_ipo(face->w/2, face->w/2-x)));
			d = pixel(face, x, y);
			
			// Copia i 3 canali
			d[0] = p[2];
			d[1] = p[1];
			d[2] = p[0];
		}
	}
	SDL_SaveBMP(face, "milkway_rt.bmp");
	
	printf("milkway_up..\n");
	// Per ogni pixel dell'immagine
	for(int y=0; y<face->h; y++) {
		for(int x=0; x<face->w; x++) {
			/* Here p is the address to the pixel we want to set */
			p = pixel_polare(sphere, atan2(x-face->w/2, face->w/2-y), atan2(face->h/2, pita_ipo(face->w/2-x,face->h/2-y)));
			d = pixel(face, x, y);
			
			// Copia i 3 canali
			d[0] = p[2];
			d[1] = p[1];
			d[2] = p[0];
		}
	}
	SDL_SaveBMP(face, "milkway_up.bmp");
	
	printf("milkway_fr..\n");
	// Per ogni pixel dell'immagine
	for(int y=0; y<face->h; y++) {
		for(int x=0; x<face->w; x++) {
			/* Here p is the address to the pixel we want to set */
			p = pixel_polare(sphere, atan2(x-face->w/2, -face->w/2), atan2(face->h/2-y, pita_ipo(face->w/2, face->w/2-x)));
			d = pixel(face, x, y);
			
			// Copia i 3 canali
			d[0] = p[2];
			d[1] = p[1];
			d[2] = p[0];
		}
	}
	SDL_SaveBMP(face, "milkway_fr.bmp");
	
	printf("milkway_dn..\n");
	// Per ogni pixel dell'immagine
	for(int y=0; y<face->h; y++) {
		for(int x=0; x<face->w; x++) {
			/* Here p is the address to the pixel we want to set */
			p = pixel_polare(sphere, atan2(x-face->w/2, y-face->h/2), atan2(-face->h/2, pita_ipo(x-face->w/2,y-face->h/2)));
			d = pixel(face, x, y);
			
			// Copia i 3 canali
			d[0] = p[2];
			d[1] = p[1];
			d[2] = p[0];
		}
	}
	SDL_SaveBMP(face, "milkway_dn.bmp");
	
	printf("milkway_lf..\n");
	// Per ogni pixel dell'immagine
	for(int y=0; y<face->h; y++) {
		for(int x=0; x<face->w; x++) {
			/* Here p is the address to the pixel we want to set */
			p = pixel_polare(sphere, atan2(-face->w/2, face->w/2-x), atan2(face->h/2-y, pita_ipo(face->w/2, face->w/2-x)));
			d = pixel(face, x, y);
			
			// Copia i 3 canali
			d[0] = p[2];
			d[1] = p[1];
			d[2] = p[0];
		}
	}
	SDL_SaveBMP(face, "milkway_lf.bmp");
	
	return 0;
}

