#include "bullet_form.h"

Ref<Texture2D> BulletForm::get_texture() const {
  return texture;
}

void BulletForm::set_texture(const Ref<Texture2D> &p_texture) {
  texture = p_texture;
}

Ref<Material> BulletForm::get_material() const {
  return material;
}

void BulletForm::set_material(const Ref<Material> &p_material) {
  material = p_material;
}

uint16_t BulletForm::get_radius() const {
  return radius;
}

void BulletForm::set_radius(uint16_t p_radius) {
  radius = p_radius;
}

void BulletForm::_bind_methods() {
  ClassDB::bind_method(D_METHOD("set_texture", "texture"), &BulletForm::set_texture);
  ClassDB::bind_method(D_METHOD("get_texture"), &BulletForm::get_texture);
  ClassDB::bind_method(D_METHOD("set_material", "material"), &BulletForm::set_material);
  ClassDB::bind_method(D_METHOD("get_material"), &BulletForm::get_material);
  ClassDB::bind_method(D_METHOD("set_radius", "radius"), &BulletForm::set_radius);
  ClassDB::bind_method(D_METHOD("get_radius"), &BulletForm::get_radius);

  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_texture", "get_texture");
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

  String hint_range = "0.0," + String::num_uint64(UINT16_MAX);
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius", PROPERTY_HINT_RANGE, hint_range), "set_radius", "get_radius");
}
