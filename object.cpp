#include "object.h"

Object::Object(World *world, Image *image, float speed) {
	this->image = image;
	this->world = world;
	this->speed = speed;
	mainScreen.addObject(this);
}

void Object::setPosition(Point position) {
	this->position = position;
}

void Object::move(Point direction) {
	position += direction;
}

Point Object::getPosition() {
	return position;
}

void Object::draw() {
	image->draw(position);
}

void Object::save(std::ostream &out) {
	position.save(out);
}

void Object::load(std::istream &in) {
	position.load(in);
}

Path Object::getPath() {
	return path;
}

void Object::changeTarget(Point position) {
	path = world->findPath(getPosition(), position);
}

void Object::update(unsigned int ticks) {
	float left = speed * ticks / 1000;
	Point pos = getPosition();
	while (!path.empty() && left > 0) {
		Point dir = path.front() - pos;
		if (dir.length() > left) {
			dir.normalize();
			dir *= left;
			move(dir);
			left = 0;
		}
		else {
			setPosition(path.front());
			path.pop_front();
			left -= dir.length();
		}
	}
}
