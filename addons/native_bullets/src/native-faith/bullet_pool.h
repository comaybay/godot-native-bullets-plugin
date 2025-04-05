#ifndef BULLET_POOL_H
#define BULLET_POOL_H

#include <godot_cpp/godot.hpp>
#include <vector>

#include "bullet_form.h"
#include "structures.h"

using namespace godot;

class BulletCCircleList
{
private:
  uint32_t size;

  UnorderedPackedByteArray vec2_position_bytes;
  UnorderedPackedByteArray float_radius_bytes;
  UnorderedPackedByteArray uint_id_bytes;

public:
  BulletCCircleList(uint32_t capacity);
  void reserve(uint32_t capacity);
  void add(const Bullet*bullet);
  void swap_remove(uint32_t index);
};

class BulletPool
{
private:
  int32_t id_counter;
  std::vector<Bullet> bullets_storage; // store bullets in continuous memory for performance... maybe???
  BulletCCircleList bullet_ccircles;
  UnorderedVector<Bullet*> active_bullets;

  // bullets that are not active and can be reused
  UnorderedVector<Bullet*> reusable_bullets;

  // bullets that are not active but is still being used by bullet controller so they can't be reused
  UnorderedVector<Bullet*> pending_bullets;

public:
  BulletPool(uint32_t initial_capacity);

  void get(uint32_t amount, Ref<BulletForm> bullet_form, std::vector<Bullet*> &added_bullets);

  /** indexes must be sorted in descending order */
  void release(const std::vector<uint32_t> &indexes);

  void update_pending_bullets();
};

#endif
