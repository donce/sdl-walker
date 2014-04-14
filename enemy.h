#ifndef ENEMY_H
#define ENEMY_H

#include "object.h"

class Enemy : public Object {
public:
	Enemy(World *world, Image *image, float speed);
};

#endif
