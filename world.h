#ifndef WORLD_H
#define WORLD_H

#include "segment.h"
#include "path.h"

#include <vector>

class World {
public:
	void load(const char *filename);
	Path findPath(Point from, Point to);
	bool intersect(Segment s);
	bool* calcWays(Point from);
	std::vector<Segment> getSegments();
private:
	void generateVertices();
	std::vector<Point> vertices;
	std::vector<std::vector<int> > edges;
	std::vector<Segment> segments;
};

#endif
