#include "shield_talent.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include "../character.h"
#include "../game_libs/logic.h"

void ShieldTalent::_bind_methods() {
    ADD_SIGNAL(MethodInfo("shield_broken"));
    
    ClassDB::bind_method(D_METHOD("setup", "character", "battlefield"), &ShieldTalent::setup);
    ClassDB::bind_method(D_METHOD("is_shield_broken"), &ShieldTalent::is_shield_broken);
    ClassDB::bind_method(D_METHOD("take_damage", "amount", "attack"), &ShieldTalent::take_damage);
    ClassDB::bind_method(D_METHOD("get_exceeded_taken_damage"), &ShieldTalent::get_exceeded_taken_damage);
    
    ClassDB::bind_method(D_METHOD("set_shield_health", "value"), &ShieldTalent::set_shield_health);
    ClassDB::bind_method(D_METHOD("get_shield_health"), &ShieldTalent::get_shield_health);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "shield_health"), "set_shield_health", "get_shield_health");
    
    ClassDB::bind_method(D_METHOD("set_from_character_health", "value"), &ShieldTalent::set_from_character_health);
    ClassDB::bind_method(D_METHOD("get_from_character_health"), &ShieldTalent::get_from_character_health);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "from_character_health"), "set_from_character_health", "get_from_character_health");
    
    ClassDB::bind_method(D_METHOD("set_fx_shield_scene", "value"), &ShieldTalent::set_fx_shield_scene);
    ClassDB::bind_method(D_METHOD("get_fx_shield_scene"), &ShieldTalent::get_fx_shield_scene);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fx_shield_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_fx_shield_scene", "get_fx_shield_scene");
}

void ShieldTalent::set_shield_health(int p_health) { 
    shield_health = p_health; 
}

int ShieldTalent::get_shield_health() const { 
    return shield_health; 
}

void ShieldTalent::set_from_character_health(float p_scale) { 
    from_character_health = p_scale; 
}

float ShieldTalent::get_from_character_health() const { 
    return from_character_health; 
}

void ShieldTalent::set_fx_shield_scene(const Ref<PackedScene>& p_scene) { 
    fx_shield_scene = p_scene; 
}

Ref<PackedScene> ShieldTalent::get_fx_shield_scene() const { 
    return fx_shield_scene; 
}

void ShieldTalent::setup(Character* character) {
    _shield_health = shield_health + character->get_max_health() * from_character_health;
    shield_owner = character;
    SceneTree* scene_tree = Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
    battlefield = scene_tree->get_current_scene();
}

bool ShieldTalent::is_shield_broken() const {
    return _shield_health <= 0;
}

void ShieldTalent::take_damage(int amount, Variant attack) {
    _shield_health -= amount;
        
    Rect2 owner_hitbox = shield_owner->get_hitbox_rect();
    
    Vector2 attack_pos;
    if (Character* char_attack = Object::cast_to<Character>(attack)) {
        attack_pos = char_attack->get_global_position();
    } else if (Node2D* node = Object::cast_to<Node2D>(attack)) {
        attack_pos = node->get_global_position();
    } else {
        attack_pos = owner_hitbox.get_center() + Vector2(owner_hitbox.get_size().x * 0.5f * shield_owner->get_march_direction(), 0.0f);
    }
    
    Vector2 shield_owner_pos = shield_owner->get_global_position();
    Vector2 direction = (attack_pos - shield_owner_pos).normalized();
    Vector2 hit_pos = Logic::find_hit_position(owner_hitbox, attack_pos) + (direction * 100);
    
    FxShield* fx_shield = Object::cast_to<FxShield>(fx_shield_scene->instantiate());
    fx_shield->setup(hit_pos, direction.angle());

    battlefield->call("add_effect", fx_shield);
    
    if (is_shield_broken()) {
        emit_signal("shield_broken");
    }
}

int ShieldTalent::get_exceeded_taken_damage() const {
    return is_shield_broken() ? abs(_shield_health) : 0;
}
