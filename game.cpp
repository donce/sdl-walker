#include "game.h"

#include <fstream>

const float PLAYER_SPEED = 600;
const float ENEMY_SPEED = 200;

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

	player = new Object(&world, &agentImage, PLAYER_SPEED);
	enemies.clear();
}

void Game::startNewGame() {
	player->setPosition(Point(50, 50));

	for (int i = 100; i <= 300; i += 50) {
		Enemy *enemy = new Enemy(&world, &enemyImage, ENEMY_SPEED);
		enemy->setPosition(Point(i, 50));
		enemies.push_back(enemy);
	}
}

bool Game::save() {
	std::ofstream f(SAVE_FILE, std::ios::binary);
	if (!f)
		return 0;

	player->save(f);
	unsigned int number = enemies.size();
	f.write((char*)&number, sizeof(number));
	for (unsigned int i = 0; i < number; ++i)
		enemies[i]->save(f);

	f.close();
	return 1;
}

bool Game::load() {
	std::ifstream f(SAVE_FILE, std::ios::binary);
	if (!f)
		return false;

	player->load(f);
	unsigned int number;
	f.read((char*)&number, sizeof(number));
	for (unsigned int i = 0; i < number; ++i) {
		Enemy *enemy = new Enemy(&world, &enemyImage, ENEMY_SPEED);
		enemy->load(f);
		enemies.push_back(enemy);
	}

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

	Path playerPath = player->getPath();
	if (!playerPath.empty())
		targetImage.draw(playerPath.back());
	
	mainScreen.draw();

	mainScreen.update();
}

void Game::update(Uint32 ticks) {
	player->update(ticks);
	for (int i = 0; i < enemies.size(); ++i) {
		Segment segment = Segment(player->getPosition(), enemies[i]->getPosition());
		if (!world.intersect(segment)) {
			enemies[i]->changeTarget(player->getPosition());
		}

		enemies[i]->update(ticks);
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
			player->changeTarget(Point(e.button.x, e.button.y));
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
