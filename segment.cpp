#include "segment.h"

#include <cstdio>

Segment::Segment(Point a, Point b) {
	p[0] = a;
	p[1] = b;
}

Segment::Segment(int ax, int ay, int bx, int by) {
	p[0].c[0] = ax;
	p[0].c[1] = ay;
	p[1].c[0] = bx;
	p[1].c[1] = by;
}

bool between(float point, float a, float b) {
	const float EPSILON = 1;
	if (a > b) {
		float t = a;
		a = b;
		b = t;
	}
	return (a-EPSILON <= point && point <= b+EPSILON);
}

float Segment::length() {
	return (p[1] - p[0]).length();
}

bool Segment::intersect(const Segment &s) {
	int bottom = (p[0].c[0] - p[1].c[0]) * (s.p[0].c[1] - s.p[1].c[1]) - (p[0].c[1] - p[1].c[1]) * (s.p[0].c[0] - s.p[1].c[0]);
	if (bottom == 0)
		return false;
	int first = (p[0].c[0] * p[1].c[1] - p[0].c[1] * p[1].c[0]);
	int second = (s.p[0].c[0] * s.p[1].c[1] - s.p[0].c[1] * s.p[1].c[0]);
	float x = float(first * (s.p[0].c[0] - s.p[1].c[0]) - (p[0].c[0] - p[1].c[0]) * second) / bottom;
	float y = float(first * (s.p[0].c[1] - s.p[1].c[1]) - (p[0].c[1] - p[1].c[1]) * second) / bottom;
	return between(x, p[0].c[0], p[1].c[0]) && between(x, s.p[0].c[0], s.p[1].c[0]) && 
		between(y, p[0].c[1], p[1].c[1]) && between(y, s.p[0].c[1], s.p[1].c[1]);
}
