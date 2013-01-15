#ifndef POINT_H
#define POINT_H

struct Point {
	float c[2];

	Point(int x, int y);
	Point();

	float length();
	void normalize();

	Point operator+(const Point &p);
	Point operator-() const;
	Point operator-(const Point &p);
	Point operator+=(const Point &p);
	Point operator*=(const float f);
};

#endif
