#include "screen.h"

#include <algorithm>

Screen mainScreen;

void Screen::init() {
	surface = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	background = new Image();
	background->loadBMP("background.bmp");
}

void Screen::addObject(Object *object) {
	objects.push_back(object);
}

//TODO: make generic or screen?
void drawPixel(SDL_Surface *surface, int x, int y, Uint8 R, Uint8 G, Uint8 B) {
	if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
		return;
	Uint32 color = SDL_MapRGB(surface->format, R, G, B);
	Uint32 *pixel = (Uint32*)surface->pixels + y*surface->pitch/4 + x;
	*pixel = color;
}

void Screen::draw(SDL_Surface *tex, int x, int y) {
	SDL_Rect rSrc, rDst;
	rSrc.w = rDst.w = tex->w;
	rSrc.h = rDst.h = tex->h;
	rSrc.x = rSrc.y = 0;
	rDst.x = x - rDst.w/2;
	rDst.y = y - rDst.h/2;
	SDL_BlitSurface(tex, &rSrc, surface, &rDst);
	//SDL_UpdateRects(screen, 1, &rDst);
}

void Screen::draw(SDL_Surface *tex, Point point) {
	draw(tex, point.c[0], point.c[1]);
}

void Screen::draw(Segment s) {
	int dx = s.p[1].c[0] - s.p[0].c[0];
	int dy = s.p[1].c[1] - s.p[0].c[1];
	if (abs(dx) > abs(dy)) {
		if (s.p[0].c[0] > s.p[1].c[0])
			std::swap(s.p[0], s.p[1]);
		for (int x = s.p[0].c[0]; x < s.p[1].c[0]; ++x) {
			int y = s.p[0].c[1] + dy * (x - s.p[0].c[0]) / dx;
			drawPixel(surface, x, y, 255, 0, 0);
		}
	}
	else {
		if (s.p[0].c[1] > s.p[1].c[1])
			std::swap(s.p[0], s.p[1]);
		for (int y = s.p[0].c[1]; y < s.p[1].c[1]; ++y) {
			int x = s.p[0].c[0] + dx * (y - s.p[0].c[1]) / dy;
			drawPixel(surface, x, y, 255, 0, 0);
		}
	}
}

void Screen::drawBackground() {
	Point size = background->size();
	for (int x = 0; x < surface->w; x += size.c[0])
		for (int y = 0; y < surface->h; y += size.c[1])
			background->draw(Point(x, y));
}

void Screen::draw() {
	for (int i = 0; i < objects.size(); ++i)
		objects[i]->draw();
}

void Screen::update() {
	SDL_UpdateRect(surface, 0, 0, surface->w, surface->h);
}

Point Screen::size() {
	return Point(surface->w, surface->h);
}
