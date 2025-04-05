#ifndef STATUS_EFFECT_STACK_H
#define STATUS_EFFECT_STACK_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <vector>

using namespace godot;

class StatusEffectStack {
public:
    float apply_add(float base_value, const std::vector<float>& stack);
    float apply_mul(float base_value, const std::vector<float>& stack);
    void clear();

    std::vector<float> attack_damage_stack = {};
    std::vector<float> movement_speed_stack = {};
    std::vector<float> attack_cooldown_stack = {};
    std::vector<float> attack_speed_stack = {};
    std::vector<float> defense_stack = {};
};

#endif // STATUS_EFFECT_STACK_H
