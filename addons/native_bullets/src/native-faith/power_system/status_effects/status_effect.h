#ifndef STATUS_EFFECT_H
#define STATUS_EFFECT_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include "../../macros.h"

using namespace godot;

class StatusEffect : public Resource {
    GDCLASS(StatusEffect, Resource);
    
protected:
    static void _bind_methods();

public:
    PROPERTY(StringName, id, StringName());
    PROPERTY(float, attack_damage_strength, 0.0f);
    PROPERTY(float, movement_speed_strength, 0.0f);
    PROPERTY(float, attack_cooldown_strength, 0.0f);
    PROPERTY(float, attack_speed_strength, 0.0f);
    PROPERTY(float, defense_strength, 0.0f);
};

#endif // STATUS_EFFECT_H
