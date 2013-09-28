#include "object.h"

Object::Object(Image *image) {
	this->image = image;
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
