#include "status_effect_stack.h"
#include "../../character.h"

#include <godot_cpp/variant/utility_functions.hpp>

float StatusEffectStack::apply_add(float base_value, const std::vector<float>& stack) {
    float result = base_value;
    for (int i = 0; i < stack.size(); i++) {
        float strength = stack[i];
        result += base_value * (strength - 1.0f);
    }
    
    return result;
}

float StatusEffectStack::apply_mul(float base_value, const std::vector<float>& stack) {
    float result = base_value;
    for (int i = 0; i < stack.size(); i++) {
        float strength = stack[i];
        result *= 1.0f / strength;
    }
    
    return result;
}

void StatusEffectStack::clear() {
    attack_damage_stack.clear();
    movement_speed_stack.clear();
    attack_cooldown_stack.clear();
    attack_speed_stack.clear();
    defense_stack.clear();
}