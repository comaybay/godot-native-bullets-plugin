#include "status_effects.h"

#include <godot_cpp/variant/utility_functions.hpp>

StatusEffects::StatusEffects()
    : buff_stack(),
      debuff_stack(),
      buffs(),
      debuffs(),
      attack_damage_multiplier(1.0f),
      attack_speed_multiplier(1.0f),
      attack_cooldown_multiplier(1.0f),
      movement_speed_multiplier(1.0f),
      defense_multiplier(1.0f)
{
}

void StatusEffects::add_status_effect(Ref<StatusEffect> status_effect)
{
    if (status_effect->type == StatusEffect::Type::BUFF)
    {
        buffs.push_back(status_effect);
        _update_status_effect_stack(buff_stack, buffs);
    } else {
        debuffs.push_back(status_effect);
        _update_status_effect_stack(debuff_stack, debuffs);
    }

    _update_multipliers();
}

void StatusEffects::remove_status_effect(Ref<StatusEffect> status_effect)
{
    if (status_effect->type == StatusEffect::Type::BUFF) {
        auto it = std::find(buffs.begin(), buffs.end(), status_effect);
        if (it != buffs.end())
        {
            buffs.erase(it);
            _update_status_effect_stack(buff_stack, buffs);
            _update_multipliers();
        }
        else {
            WARN_PRINT(vformat("Status effect not found: %s", status_effect->id));
        }
    }
    else {
        auto it = std::find(debuffs.begin(), debuffs.end(), status_effect);
        if (it != debuffs.end())
        {
            debuffs.erase(it);
            _update_status_effect_stack(debuff_stack, debuffs);
            _update_multipliers();
        }
        else {
            WARN_PRINT(vformat("Status effect not found: %s", status_effect->id));
        }
    }
}

void StatusEffects::_update_status_effect_stack(StatusEffectStack& stack, const std::vector<Ref<StatusEffect>>& status_effects)
{
    stack.clear();
    // filter unique status effects
    std::vector<Ref<StatusEffect>> unique_status_effects(status_effects.size());
    for (int i = 0; i < status_effects.size(); i++)
    {
        Ref<StatusEffect> status_effect = status_effects[i];
        if (std::find(unique_status_effects.begin(), unique_status_effects.end(), status_effect) == unique_status_effects.end())
        {
            unique_status_effects.push_back(status_effect);

            if (status_effect->attack_damage_strength > 1.0f)
            {
                stack.attack_damage_stack.push_back(status_effect->attack_damage_strength);
            }

            if (status_effect->movement_speed_strength > 1.0f)
            {
                stack.movement_speed_stack.push_back(status_effect->movement_speed_strength);
            }

            if (status_effect->attack_cooldown_strength > 1.0f)
            {
                stack.attack_cooldown_stack.push_back(status_effect->attack_cooldown_strength);
            }

            if (status_effect->attack_speed_strength > 1.0f)
            {
                stack.attack_speed_stack.push_back(status_effect->attack_speed_strength);
            }
            
            if (status_effect->defense_strength > 1.0f)
            {
                stack.defense_stack.push_back(status_effect->defense_strength);
            }
        }
    }
}

void StatusEffects::_update_multipliers()
{
    attack_damage_multiplier = _get_attack_damage_multiplier();
    attack_cooldown_multiplier = _get_attack_cooldown_multiplier();
    movement_speed_multiplier = _get_movement_speed_multiplier();
    defense_multiplier = _get_defense_multiplier();
    attack_speed_multiplier = _get_attack_speed_multiplier();
}

float StatusEffects::_get_attack_damage_multiplier()
{
    float base_value = buff_stack.apply_add(1.0f, buff_stack.attack_damage_stack);
    return debuff_stack.apply_mul(base_value, debuff_stack.attack_damage_stack);
}

float StatusEffects::_get_attack_speed_multiplier()
{
    float value = buff_stack.apply_add(1.0f, buff_stack.attack_speed_stack);
    return debuff_stack.apply_mul(value, debuff_stack.attack_speed_stack);
}

float StatusEffects::_get_attack_cooldown_multiplier()
{
    float cooldown = buff_stack.apply_mul(1.0f, buff_stack.attack_cooldown_stack);
    return debuff_stack.apply_add(cooldown, debuff_stack.attack_cooldown_stack);
}

float StatusEffects::_get_movement_speed_multiplier()
{
    float base_value = buff_stack.apply_add(1.0f, buff_stack.movement_speed_stack);
    return debuff_stack.apply_mul(base_value, debuff_stack.movement_speed_stack);
}

float StatusEffects::_get_defense_multiplier()
{
    float damage_taken = buff_stack.apply_mul(1.0f, buff_stack.defense_stack);
    return debuff_stack.apply_add(damage_taken, debuff_stack.defense_stack);
}
