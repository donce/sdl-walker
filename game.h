#ifndef GAME_H
#define GAME_H

#include "image.h"
#include "point.h"
#include "object.h"
#include "world.h"
#include "screen.h"
#include "enemy.h"

#define WORLD_FILE "data/world"
#define SAVE_FILE "data/save"

class Game {
public:
	void init();
	void run();
	Game();
private:
	void startNewGame();
	bool save();
	bool load();
	void stop();

	void draw();
	void handleEvent(SDL_Event e);

	bool running;

	Image agentImage, targetImage, enemyImage;
	Uint8 mx, my;
	bool buttonLeft;

	std::vector<Enemy*> enemies;


	World world;
	Object *player;

	void update(Uint32 ticks);
};

#endif
