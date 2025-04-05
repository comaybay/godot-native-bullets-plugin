#include "status_effects.h"
#include "../../character.h"

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

void StatusEffects::add_buff(Ref<StatusEffect> buff)
{
    buffs.push_back(buff);
    _update_status_effect_stack(buff_stack, buffs);
    _update_character_stats();
}

void StatusEffects::remove_buff(Ref<StatusEffect> buff)
{
    auto it = std::find(buffs.begin(), buffs.end(), buff);
    if (it != buffs.end())
    {
        buffs.erase(it);
        _update_status_effect_stack(buff_stack, buffs);
        _update_character_stats();
    }
}

void StatusEffects::add_debuff(Ref<StatusEffect> debuff)
{
    debuffs.push_back(debuff);
    _update_status_effect_stack(debuff_stack, debuffs);
    _update_character_stats();
}

void StatusEffects::remove_debuff(Ref<StatusEffect> debuff)
{
    auto it = std::find(debuffs.begin(), debuffs.end(), debuff);
    if (it != debuffs.end())
    {
        debuffs.erase(it);
        _update_status_effect_stack(debuff_stack, debuffs);
        _update_character_stats();
    }
}

Ref<StatusEffect> StatusEffects::get_debuff_id(StringName id)
{
    for (int i = 0; i < debuffs.size(); i++)
    {
        Ref<StatusEffect> debuff = debuffs[i];
        if (debuff->get_id() == id)
        {
            return debuff;
        }
    }
    return Ref<StatusEffect>();
}

Ref<StatusEffect> StatusEffects::get_buff_id(StringName id)
{
    for (int i = 0; i < buffs.size(); i++)
    {
        Ref<StatusEffect> buff = buffs[i];
        if (buff->get_id() == id)
        {
            return buff;
        }
    }
    return Ref<StatusEffect>();
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

void StatusEffects::_update_character_stats()
{
    attack_damage_multiplier = _get_attack_damage_multiplier();
    attack_cooldown_multiplier = _get_attack_cooldown_multiplier();
    movement_speed_multiplier = _get_movement_speed_multiplier();
    defense_multiplier = _get_defense_multiplier();
    attack_speed_multiplier = _get_attack_speed_multiplier();

    character->update_attack_damage();
    character->update_attack_cooldown();
    character->update_speed();
    character->update_defense();
    character->update_animation_speed();
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

void StatusEffects::setup(Character *character)
{
    this->character = character;
}