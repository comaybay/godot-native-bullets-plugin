#ifndef BULLET_FORM_H
#define BULLET_FORM_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/material.hpp>

using namespace godot;

class BulletForm : public Resource {
  GDCLASS(BulletForm, Resource)

private:
	Ref<Texture2D> texture;
	Ref<Material> material;
  uint16_t radius;

protected:
  static void _bind_methods();

public:
  Ref<Texture2D> get_texture() const;
  void set_texture(const Ref<Texture2D> &p_texture);
  Ref<Material> get_material() const;
  void set_material(const Ref<Material> &p_material);
  uint16_t get_radius() const;
  void set_radius(uint16_t p_radius);
};

#endif