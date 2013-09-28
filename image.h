#ifndef IMAGE_H
#define IMAGE_H

#include "screen.h"

#include "SDL.h"

class Screen;

class Image {
private:
	SDL_Surface *surface;
public:
	bool loadBMP(const char *file);
	void draw(Point position);
	Point size();
};

#endif
