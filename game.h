#ifndef GAME_H
#define GAME_H

#include "image.h"
#include "point.h"
#include "object.h"
#include "world.h"
#include "screen.h"
#include "path.h"

#define WORLD_FILE "data/world"

class Game {
public:
	void init();
	void run();
	Game();
private:
	void stop();
	void changeTarget(Point position);
	void draw();
	void handleEvent(SDL_Event e);

	bool running;

	Screen screen;
	Image agent, target;
	Uint8 mx, my;
	bool buttonLeft;

	Path path;

	World world;
	Object player;

	void update(Uint32 ticks);
};

#endif
