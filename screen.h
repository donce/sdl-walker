#ifndef SCREEN_H
#define SCREEN_H

#include "object.h"
#include "segment.h"
#include "image.h"

#include <vector>

#include "SDL.h"

class Image;
class Object;

class Screen {
private:
	void draw(SDL_Surface *tex, int x, int y);
	SDL_Surface *surface;
	Image *background;
	std::vector<Object*> objects;
public:
	void init();
	void addObject(Object *object);

	void drawBackground();
	void draw(Segment s);
	void draw(SDL_Surface *tex, Point point);

	void update();
	void draw();

	Point size();
};

extern Screen mainScreen;

#endif
