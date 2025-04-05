#ifndef SHIELD_TALENT_H
#define SHIELD_TALENT_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/rect2.hpp>

#include "../character.h"
#include "../game_libs/logic.h"
#include "fx_shield.h"

using namespace godot;

class ShieldTalent : public Resource {
    GDCLASS(ShieldTalent, Resource);

private:
    int _shield_health;
    Character* shield_owner;
    Node* battlefield;

protected:
    static void _bind_methods();

public:
    int shield_health = 0;
    float from_character_health = 0.0f;
    Ref<PackedScene> fx_shield_scene;
    
    void set_shield_health(int p_health);
    int get_shield_health() const;
    
    void set_from_character_health(float p_scale);
    float get_from_character_health() const;
    
    void set_fx_shield_scene(const Ref<PackedScene>& p_scene);
    Ref<PackedScene> get_fx_shield_scene() const;
    
    void setup(Character* character);
    
    bool is_shield_broken() const;
    
    void take_damage(int amount, Variant attack);
    
    int get_exceeded_taken_damage() const;
};

#endif // SHIELD_TALENT_H
