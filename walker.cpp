#include "SDL.h"

#include "point.h"
#include "segment.h"

#include <list>
#include <vector>
#include <iostream>
#include <fstream>


using namespace std;

const float SPEED = 600;

SDL_Surface *screen;
SDL_Surface *agentTex, *targetTex, *backgroundTex;
Uint8 mx, my;
bool buttonLeft = false;
bool running = false;

vector<Segment> segments;
Point pos;
list<Point> path;

void quit() {
	running = false;
}

void drawPixel(SDL_Surface *surface, int x, int y, Uint8 R, Uint8 G, Uint8 B) {
	if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
		return;
	Uint32 color = SDL_MapRGB(surface->format, R, G, B);
	Uint32 *pixel = (Uint32*)surface->pixels + y*surface->pitch/4 + x;
	*pixel = color;
}

void draw(SDL_Surface *tex, int x, int y) {
	SDL_Rect rSrc, rDst;
	rSrc.w = rDst.w = tex->w;
	rSrc.h = rDst.h = tex->h;
	rSrc.x = rSrc.y = 0;
	rDst.x = x - rDst.w/2;
	rDst.y = y - rDst.h/2;
	SDL_BlitSurface(tex, &rSrc, screen, &rDst);
	//SDL_UpdateRects(screen, 1, &rDst);
}

void draw(SDL_Surface *tex, Point point) {
	draw(tex, point.c[0], point.c[1]);
}

void draw(Segment s) {
	int dx = s.p[1].c[0] - s.p[0].c[0];
	int dy = s.p[1].c[1] - s.p[0].c[1];
	if (abs(dx) > abs(dy)) {
		if (s.p[0].c[0] > s.p[1].c[0])
			swap(s.p[0], s.p[1]);
		for (int x = s.p[0].c[0]; x < s.p[1].c[0]; ++x) {
			int y = s.p[0].c[1] + dy * (x - s.p[0].c[0]) / dx;
			drawPixel(screen, x, y, 255, 0, 0);
		}
	}
	else {
		if (s.p[0].c[1] > s.p[1].c[1])
			swap(s.p[0], s.p[1]);
		for (int y = s.p[0].c[1]; y < s.p[1].c[1]; ++y) {
			int x = s.p[0].c[0] + dx * (y - s.p[0].c[1]) / dy;
			drawPixel(screen, x, y, 255, 0, 0);
		}
	}
}

void drawBackground() {
	for (int x = 0; x < screen->w; x += backgroundTex->w)
		for (int y = 0; y < screen->h; y += backgroundTex->h)
			draw(backgroundTex, x, y);
}

void updateAll(SDL_Surface *surface) {
	SDL_UpdateRect(surface, 0, 0, surface->w, surface->h);
}

bool intersect(Segment s) {
	for (int i = 0; i < segments.size(); ++i)
		if (s.intersect(segments[i]))
			return true;
	return false;
}

vector<Point> vertices;
vector<vector<int> > edges;
void generateVertices() {
	vertices.clear();
	edges.clear();

	int n = segments.size()*2;
	vertices.resize(n);
	edges.resize(n);
	for (int i = 0; i < n; ++i)
		edges[i].resize(n);

	for (int i = 0; i < segments.size(); ++i) {
		Point dir = segments[i].p[0] - segments[i].p[1];
		dir.normalize();
		dir *= 3;
		for (int j = 0; j < 2; ++j) {
			vertices[2*i+j] = segments[i].p[j] + dir;
			for (int k = 0; k < 2*i; ++k)
				edges[2*i+j][k] = edges[k][2*i+j] = !intersect(Segment(vertices[2*i+j], vertices[k]));
			edges[2*i][2*i+1] = edges[2*i+1][2*i] = false;
			dir *= -1;
		}
	}
}

bool* calcWays(Point from) {
	bool *ways = new bool[vertices.size()];
	for (int i = 0; i < vertices.size(); ++i)
		ways[i] = !intersect(Segment(from, vertices[i]));
	return ways;
}

void changeTarget(int x, int y) {
	Point target(x, y);
	Segment s(pos, target);
	path.clear();
	if (intersect(s)) {
		int n = vertices.size();

		bool *from = calcWays(pos);
		bool *to = calcWays(target);

		bool *visited = new bool[n];
		float *dist = new float[n];
		int *back = new int[n];

		for (int i = 0; i < n; ++i) {
			dist[i] = from[i] ? Segment(pos, vertices[i]).length() : -1;
			back[i] = -1;
			visited[i] = false;
		}
		int now = 0;
		while (now != -1) {
			//find
			now = -1;
			for (int i = 0; i < n; ++i)
				if (!visited[i] && dist[i] >= 0 && (now == -1 || dist[i] < dist[now]))
					now = i;
			if (now != -1) {
				//add
				visited[now] = true;
				for (int i = 0; i < n; ++i)
					if (!visited[i] && edges[now][i]) {
						float newDist = dist[now] + Segment(vertices[now], vertices[i]).length();
						if (dist[i] < 0 || newDist < dist[i]) {
							dist[i] = newDist;
							back[i] = now;
						}
					}
			}
		}
		float distBest;
		int best = -1;
		for (int i = 0; i < n; ++i)
			if (visited[i] && to[i]) {
				float distNow = dist[i] + Segment(vertices[i], target).length();
				if (best == -1 || distNow < distBest) {
					best = i;
					distBest = distNow;
				}
			}
		if (best != -1) {
			path.push_front(target);
			int now = best;
			while (best != -1) {
				path.push_front(vertices[best]);
				best = back[best];
			}
		}

		delete[] visited;
		delete[] dist;
		delete[] back;
		delete[] from;
		delete[] to;
	}
	else
		path.push_back(target);
}

void draw() {
	drawBackground();
	for (int i = 0; i < segments.size(); ++i)
		draw(segments[i]);
	if (!path.empty())
		draw(targetTex, path.back());
	draw(agentTex, pos);
	updateAll(screen);
}

void update(Uint32 ticks) {
	float left = SPEED * ticks / 1000;
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
		pos += dir;
	}
}

void handleEvent(SDL_Event e) {
	if (e.type == SDL_KEYDOWN) {
		int nr = e.key.keysym.sym;
		if (nr == SDLK_ESCAPE)
			quit();
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		Uint8 nr = e.button.button;
		if (nr == SDL_BUTTON_RIGHT)
			changeTarget(e.button.x, e.button.y);
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
		quit();
}

SDL_Surface* loadBMP(const char* file) {
	const char *BASE = "data/";
	char location[strlen(BASE) + strlen(file) + 1];
	strcpy(location, BASE);
	strcat(location, file);
	SDL_Surface *tex = SDL_LoadBMP(location);
	if (tex == NULL)
		throw "Error loading BMP";
	return tex;
}

void loadSegments(const char *filename) {
	ifstream f(filename);
	Segment s;
	while (f >> s.p[0].c[0] >> s.p[0].c[1] >> s.p[1].c[0] >> s.p[1].c[1])
		segments.push_back(s);
	f.close();
}

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

	agentTex = loadBMP("agent.bmp");
	targetTex = loadBMP("target.bmp");
	backgroundTex = loadBMP("background.bmp");

	loadSegments("data/world");
	generateVertices();

	pos.c[0] = pos.c[1] = 50;

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

	return 0;
}
