#include "image.h"

bool Image::loadBMP(const char* file) {
	const char *BASE = "data/";
	char location[strlen(BASE) + strlen(file) + 1];
	strcpy(location, BASE);
	strcat(location, file);
	surface = SDL_LoadBMP(location);
	return surface != NULL;
}

void Image::draw(Point position) {
	mainScreen.draw(surface, position);
}

Point Image::size() {
	return Point(surface->w, surface->h);
}
