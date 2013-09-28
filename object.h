#ifndef OBJECT_H
#define OBJECT_H

#include "point.h"
#include "image.h"


class Image;

class Object {
public:
	Object(Image *image);
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
