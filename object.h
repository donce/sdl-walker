#ifndef OBJECT_H
#define OBJECT_H

#include "point.h"
#include "image.h"

class Object {
public:
	void setPosition(float x, float y);
	void move(Point direction);
	Point getPosition();
	void setImage(Image *image);
	void draw();
private:
	Point position;
	Image *image;
};

#endif
