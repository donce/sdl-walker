#include "object.h"

Object::Object(Image *image) {
	this->image = image;
	mainScreen.addObject(this);
}

void Object::setPosition(float x, float y) {
	position.c[0] = x;
	position.c[1] = y;
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
