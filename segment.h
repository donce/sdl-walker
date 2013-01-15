#ifndef SEGMENT_H
#define SEGMENT_H

#include "point.h"

class Segment {
private:
public:
	Point p[2];
	Segment(Point a, Point b);
	Segment(int ax, int ay, int bx, int by);

	float length();
	bool intersect(const Segment &s);
};

#endif
