#include "structures.h"

Bullet::Bullet()
{
  reset(0, 0);
}

Bullet::Bullet(uint64_t id, uint16_t radius)
{
  reset(id, radius);
}

void Bullet::reset(uint64_t id, uint16_t radius)
{
  this->id = id;
  this->radius = radius;
  this->destroyed = false;
  this->controlled = true;
}


