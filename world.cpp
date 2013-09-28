#include "world.h"

#include <fstream>

void World::load(const char *filename) {
	std::ifstream f(filename);
	Segment s;
	while (f >> s.p[0].c[0] >> s.p[0].c[1] >> s.p[1].c[0] >> s.p[1].c[1])
		segments.push_back(s);
	f.close();
	generateVertices();
}

std::vector<Segment> World::getSegments() {
	return segments;
}

void World::generateVertices() {
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

bool* World::calcWays(Point from) {
	bool *ways = new bool[vertices.size()];
	for (int i = 0; i < vertices.size(); ++i)
		ways[i] = !intersect(Segment(from, vertices[i]));
	return ways;
}

bool World::intersect(Segment s) {
	for (int i = 0; i < segments.size(); ++i)
		if (s.intersect(segments[i]))
			return true;
	return false;
}

Path World::findPath(Point begin, Point end) {
	Path path;

	Segment s(begin, end);
	path.clear();
	if (intersect(s)) {
		int n = vertices.size();

		bool *from = calcWays(begin);
		bool *to = calcWays(end);

		bool *visited = new bool[n];
		float *dist = new float[n];
		int *back = new int[n];

		for (int i = 0; i < n; ++i) {
			dist[i] = from[i] ? Segment(begin, vertices[i]).length() : -1;
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
				float distNow = dist[i] + Segment(vertices[i], end).length();
				if (best == -1 || distNow < distBest) {
					best = i;
					distBest = distNow;
				}
			}
		if (best != -1) {
			path.push_front(end);
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
		path.push_back(end);
	return path;
}
