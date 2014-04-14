#ifndef OBJECT_H
#define OBJECT_H

#include "point.h"
#include "image.h"
#include "path.h"
#include "world.h"


class Image;

class Object {
public:
	Object(World *world, Image *image, float speed);
	void setPosition(Point position);
	void move(Point direction);
	Point getPosition();
	void setImage(Image *image);
	void draw();
	void save(std::ostream &out);
	void load(std::istream &in);
	
	void changeTarget(Point position);
	void update(unsigned int ticks);
	
	Path getPath();
private:
	Point position;
	Image *image;
	World *world;

	Path path;
	float speed;
};

#endif
