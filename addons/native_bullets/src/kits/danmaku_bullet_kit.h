#ifndef DANMAKU_BULLET_KIT_H
#define DANMAKU_BULLET_KIT_H

#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include "../bullet.h"
#include "../bullet_kit.h"

using namespace godot;

// Bullet definition.
// This is necessary only if your BulletKit needs custom efficiently accessible bullet properties.
class DanmakuBullet : public Bullet
{
  // Godot requires you to add this macro to make this class work properly.
  GDCLASS(DanmakuBullet, Bullet)
public:
  float rotation_speed = 0;
  float get_rotation_speed() { return rotation_speed; }
  void set_rotation_speed(float rotation_speed)
  {
    this->rotation_speed = rotation_speed;
  }

  static void _bind_methods()
  {
    ClassDB::bind_method(D_METHOD("set_rotation_speed", "rotation_speed"), &DanmakuBullet::set_rotation_speed);
    ClassDB::bind_method(D_METHOD("get_rotation_speed"), &DanmakuBullet::get_rotation_speed);

    // Registering an Object reference property with GODOT_PROPERTY_HINT_NODE_TYPE and hint_string is just
    // a way to tell the editor plugin the type of the property, so that it can be viewed in the BulletKit inspector.
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "rotation_speed", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, ""),
                 "set_rotation_speed", "get_rotation_speed");
  }
};

// Bullet kit definition.
class DanmakuBulletKit : public BulletKit
{
  GDCLASS(DanmakuBulletKit, BulletKit)
public:
  BULLET_KIT(DanmakuBulletKit, DanmakuBulletsPool, DanmakuBullet)

  Ref<Texture2D> texture;

  void set_texture(Ref<Texture2D> texture)
  {
    this->texture = texture;
    emit_changed();
  }

  Ref<Texture2D> get_texture()
  {
    return texture;
  }

  Ref<AudioStream> hit_sfx;
  Ref<AudioStream> get_hit_sfx() { return hit_sfx; }
  void set_hit_sfx(Ref<AudioStream> hit_sfx)
  {
    this->hit_sfx = hit_sfx;
  }

  int damage = 10;
  float get_damage() { return damage; }
  void set_damage(int damage)
  {
    this->damage = damage;
  }

  static void _bind_methods()
  {
    ClassDB::bind_method(D_METHOD("set_texture", "texture"), &DanmakuBulletKit::set_texture);
    ClassDB::bind_method(D_METHOD("get_texture"), &DanmakuBulletKit::get_texture);
    ClassDB::bind_method(D_METHOD("set_hit_sfx", "hit_sfx"), &DanmakuBulletKit::set_hit_sfx);
    ClassDB::bind_method(D_METHOD("get_hit_sfx"), &DanmakuBulletKit::get_hit_sfx);
    ClassDB::bind_method(D_METHOD("set_damage", "damage"), &DanmakuBulletKit::set_damage);
    ClassDB::bind_method(D_METHOD("get_damage"), &DanmakuBulletKit::get_damage);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D",
                              PROPERTY_USAGE_DEFAULT, "Texture2D"),
                 "set_texture", "get_texture");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "hit_sfx", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream",
                              PROPERTY_USAGE_DEFAULT, "AudioStream"),
                 "set_hit_sfx", "get_hit_sfx");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "damage", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, ""),
                 "set_damage", "get_damage");
  }
};

// Bullets pool definition.
class DanmakuBulletsPool : public AbstractBulletsPool<DanmakuBulletKit, DanmakuBullet>
{

  void _enable_bullet(DanmakuBullet *bullet)
  {
    // Reset the bullet lifetime.
    bullet->lifetime = 0.0f;
    Rect2 texture_rect = Rect2(-kit->texture->get_size() / 2.0f, kit->texture->get_size());
    RID texture_rid = kit->texture->get_rid();

    // Configure the bullet to draw the kit texture each frame.
    RenderingServer::get_singleton()->canvas_item_add_texture_rect(bullet->item_rid,
                                                                   texture_rect,
                                                                   texture_rid);
  }

  bool _process_bullet(DanmakuBullet *bullet, float delta)
  {
    bullet->velocity = bullet->velocity.rotated(bullet->rotation_speed * delta);
    bullet->transform.set_origin(bullet->transform.get_origin() + bullet->velocity * delta);

    if (!active_rect.has_point(bullet->transform.get_origin()))
    {
      // Return true if the bullet should be deleted.
      return true;
    }
    // Rotate the bullet based on its velocity if "auto_rotate" is enabled.
    if (kit->auto_rotate)
    {
      bullet->transform.set_rotation(bullet->velocity.angle());
    }

    // Bullet is still alive, increase its lifetime.
    bullet->lifetime += delta;
    // Return false if the bullet should not be deleted yet.
    return false;
  }

  void _reset_bullet(DanmakuBullet *bullet) {
    bullet->cycle += 1;
    bullet->velocity = Vector2(0, 0);
    bullet->rotation_speed = 0;
  }
};

BULLET_KIT_IMPLEMENTATION(DanmakuBulletKit, DanmakuBulletsPool)

#endif