#ifndef SCREEN_H
#define SCREEN_H

#include "segment.h"
#include "image.h"

#include "SDL.h"

class Image;

class Screen {
private:
	void draw(SDL_Surface *tex, int x, int y);
	SDL_Surface *surface;
	Image *background;
public:
	void init();
	void drawBackground();
	void draw(Segment s);
	void draw(SDL_Surface *tex, Point point);
	void update();
	Point size();
};

#endif
