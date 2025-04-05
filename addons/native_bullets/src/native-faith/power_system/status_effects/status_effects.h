#ifndef STATUS_EFFECTS_H
#define STATUS_EFFECTS_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/signal.hpp>

#include "../../macros.h"
#include "status_effect.h"
#include "status_effect_stack.h"

class Character;

using namespace godot;

class StatusEffects {
private:
    StatusEffectStack buff_stack;
    StatusEffectStack debuff_stack;
    Character* character;

    std::vector<Ref<StatusEffect>> buffs;
    std::vector<Ref<StatusEffect>> debuffs;

    void _update_status_effect_stack(StatusEffectStack& stack, const std::vector<Ref<StatusEffect>>& status_effects);
    void _update_character_stats();
    float _get_attack_damage_multiplier();
    float _get_attack_speed_multiplier();
    float _get_attack_cooldown_multiplier();
    float _get_movement_speed_multiplier();
    float _get_defense_multiplier();

public:
    StatusEffects();
    float attack_damage_multiplier = 1.0f;
    float attack_speed_multiplier = 1.0f;
    float attack_cooldown_multiplier = 1.0f;
    float movement_speed_multiplier = 1.0f;
    float defense_multiplier = 1.0f;

    void add_buff(Ref<StatusEffect> status_effect);
    void add_debuff(Ref<StatusEffect> status_effect);
    void remove_buff(Ref<StatusEffect> status_effect);
    void remove_debuff(Ref<StatusEffect> status_effect);
    Ref<StatusEffect> get_debuff_id(StringName id);
    Ref<StatusEffect> get_buff_id(StringName id);
    void setup(Character* character);
};

#endif // STATUS_EFFECTS_H
