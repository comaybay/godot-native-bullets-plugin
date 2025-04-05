#include "bullet_pool.h"

#ifdef _DEBUG
#include <algorithm>
#endif

BulletCCircleList::BulletCCircleList(uint32_t capacity)
    : vec2_position_bytes(capacity), float_radius_bytes(capacity), uint_id_bytes(capacity),
      size(0)
{
  reserve(capacity);
}

void BulletCCircleList::reserve(uint32_t capacity)
{
  vec2_position_bytes.reserve(capacity * 8);
  float_radius_bytes.reserve(capacity * 4);
  uint_id_bytes.reserve(capacity * 4);
}

void BulletCCircleList::add(const Bullet *bullet)
{
  vec2_position_bytes.push_back(bullet->position.x);
  vec2_position_bytes.push_back(bullet->position.y);
  float_radius_bytes.push_back(bullet->radius);
  uint_id_bytes.push_back(bullet->id);

  size++;
}

void BulletCCircleList::swap_remove(uint32_t index)
{
  vec2_position_bytes.swap_remove(index, 8);
  float_radius_bytes.swap_remove(index, 4);
  uint_id_bytes.swap_remove(index, 4);
  size--;
}

BulletPool::BulletPool(uint32_t initial_capacity)
    : bullets_storage(initial_capacity),
      bullet_ccircles(initial_capacity), active_bullets(initial_capacity),
      reusable_bullets(initial_capacity), pending_bullets(initial_capacity / 2), id_counter(0) {}

void BulletPool::get(uint32_t amount, Ref<BulletForm> bullet_form, std::vector<Bullet *> &added_bullets)
{
  added_bullets.reserve(amount);

  // reuse bullets are the ones after the last active bullet (no need to create new bullets)
  uint32_t reused_num = std::min(reusable_bullets.size(), amount);
  for (uint32_t i = 0; i < reused_num; i++)
  {
    Bullet *bullet = reusable_bullets.pop();
    bullet->reset(id_counter, bullet_form->get_radius()); // reuse bullet

    bullet_ccircles.add(bullet);
    added_bullets.push_back(bullet);
    active_bullets.push_back(bullet);

    id_counter = (id_counter + 1) % UINT32_MAX;
  }

  uint32_t new_bullets_num = amount - reused_num;
  for (uint32_t i = 0; i < new_bullets_num; i++)
  {
    Bullet &bullet = bullets_storage.emplace_back(id_counter, bullet_form->get_radius());
    bullet_ccircles.add(&bullet);
    active_bullets.push_back(&bullet);
    added_bullets.push_back(&bullet);

    id_counter = (id_counter + 1) % UINT32_MAX;
  }
}

void BulletPool::release(const std::vector<uint32_t> &indexes)
{
#ifdef _DEBUG
  // print error if indexes are not sorted in descending order
  if (!std::is_sorted(indexes.begin(), indexes.end(), std::greater<uint32_t>()))
  {
    ERR_PRINT("Indexes are not sorted in descending order");
  }
#endif

  for (uint32_t i = 0; i < indexes.size(); i++)
  {
    uint32_t index = indexes[i];
    Bullet *bullet = active_bullets[index];
    bullet->destroyed = true;

    active_bullets.swap_remove(index);
    bullet_ccircles.swap_remove(index);

    if (bullet->is_reusable())
    {
      reusable_bullets.push_back(bullet);
    }
    else
    {
      pending_bullets.push_back(bullet);
    }
  }
}

void BulletPool::update_pending_bullets()
{
  // Process all pending bullets
  for (uint32_t i = pending_bullets.size() - 1; i >= 0; i--)
  {
    Bullet *bullet = pending_bullets[i];
    if (bullet->is_reusable())
    {
      reusable_bullets.push_back(bullet);
      pending_bullets.swap_remove(i);
    }
  }
}
