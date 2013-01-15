#include "point.h"
#include <cmath>

Point::Point() {
	c[0] = c[1] = 0;
}

Point::Point(int x, int y) {
	c[0] = x;
	c[1] = y;
}

float Point::length() {
	return sqrt(c[0]*c[0] + c[1]*c[1]);
}

void Point::normalize() {
	float l = length();
	if (l) {
		c[0] /= l;
		c[1] /= l;
	}
}

Point Point::operator+(const Point &p) {
	Point now;
	for (int i = 0; i < 2; ++i)
		now.c[i] = c[i] + p.c[i];
	return now;
}

Point Point::operator-() const {
	Point point(-c[0], -c[1]);
	return point;
}

Point Point::operator-(const Point &p) {
	return *this + (-p);
}

Point Point::operator+=(const Point &p) {
	c[0] += p.c[0];
	c[1] += p.c[1];
	return *this;
}


Point Point::operator*=(const float f) {
	for (int i = 0; i < 2; ++i)
		c[i] *= f;
}
