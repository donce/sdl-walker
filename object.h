#ifndef OBJECT_H
#define OBJECT_H

#include "point.h"
#include "image.h"


class Image;

class Object {
public:
	Object(Image *image);
	void setPosition(Point position);
	void move(Point direction);
	Point getPosition();
	void setImage(Image *image);
	void draw();
	void save(std::ostream &out);
	void load(std::istream &in);
private:
	Point position;
	Image *image;
};

#endif
