#include "game.h"

#include <fstream>

const float SPEED = 600;

Game::Game() {
	buttonLeft = false;
	running = false;

}

void Game::init() {
	SDL_Init(SDL_INIT_VIDEO);
	mainScreen.init();

	world.load(WORLD_FILE);

	agentImage.loadBMP("agent.bmp");
	targetImage.loadBMP("target.bmp");
	enemyImage.loadBMP("enemy.bmp");

	player = new Object(&agentImage);
}

void Game::startNewGame() {
	player->setPosition(Point(50, 50));

	for (int i = 100; i <= 300; i += 50) {
		Enemy *enemy = new Enemy(&enemyImage);
		enemy->setPosition(Point(i, 50));
		enemies.push_back(enemy);
	}
}

bool Game::save() {
	std::ofstream f(SAVE_FILE, std::ios::binary);
	if (!f)
		return 0;

	Point position = player->getPosition();
	f.write((char*)&position, sizeof(position));

	f.close();
	return 1;
}

bool Game::load() {
	std::ifstream f(SAVE_FILE, std::ios::binary);
	if (!f)
		return false;

	Point position;
	f.read((char*)&position, sizeof(position));
	player->setPosition(position);

	f.close();
	return true;
}

void Game::stop() {
	running = false;
	save();
}

void Game::run() {
	if (!load())
		startNewGame();
	running = true;
	Uint32 ticksLast = SDL_GetTicks();
	running = true;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e))
			handleEvent(e);

		Uint32 ticksNow = SDL_GetTicks();
		update(ticksNow - ticksLast);
		ticksLast = ticksNow;
		draw();
	}
}

void Game::draw() {
	mainScreen.drawBackground();
	std::vector<Segment> segments = world.getSegments();
	for (int i = 0; i < segments.size(); ++i)
		mainScreen.draw(segments[i]);
	if (!path.empty())
		targetImage.draw(path.back());
	
	mainScreen.draw();

	mainScreen.update();
}

void Game::changeTarget(Point position) {
	path = world.findPath(player->getPosition(), position);
}

void Game::update(Uint32 ticks) {
	float left = SPEED * ticks / 1000;
	Point pos = player->getPosition();
	while (!path.empty() && left > 0) {
		Point dir = path.front() - pos;
		if (dir.length() > left) {
			dir.normalize();
			dir *= left;
			left = 0;
		}
		else {
			path.pop_front();
			left -= dir.length();
		}
		player->move(dir);
	}
}

void Game::handleEvent(SDL_Event e) {
	if (e.type == SDL_KEYDOWN) {
		int nr = e.key.keysym.sym;
		if (nr == SDLK_ESCAPE)
			stop();
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		Uint8 nr = e.button.button;
		if (nr == SDL_BUTTON_RIGHT)
			changeTarget(Point(e.button.x, e.button.y));
		else if (nr == SDL_BUTTON_LEFT)
			buttonLeft = false;
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN) {
		Uint8 nr = e.button.button;
		if (nr == SDL_BUTTON_LEFT)
			buttonLeft = true;
	}
	else if (e.type == SDL_MOUSEMOTION) {
		mx = e.motion.x;
		my = e.motion.y;
	}
	else if (e.type == SDL_QUIT)
		stop();
}
