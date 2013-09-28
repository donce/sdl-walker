#include "object.h"

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

void Object::setImage(Image *image) {
	this->image = image;
}

void Object::draw() {
	image->draw(position);
}
