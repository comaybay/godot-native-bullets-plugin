#ifndef GLOBAL_H
#define GLOBAL_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/expression.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include "../character.h"
#include "timers/delta_timer_pool.h"
#include "../macros.h"
using namespace godot;

class Global : public Node
{
    GDCLASS(Global, Node);

private:
    static Global *_singleton;

public:
    Global();

    PROPERTY_GETONLY(float, idk, 2.0f)
    PROPERTY_GETONLY(float, TOUCH_EPSISLON, 2.0f)
    PROPERTY_GETONLY(Vector2, VIEWPORT_SIZE, Vector2(0, 0))
    
    PROPERTY_GETONLY(float, TIME_STOP_TIME_SCALE, 0.0001f)

protected:
    static void _bind_methods();

public:
    DeltaTimer& get_delta_timer(float wait_time, bool immune_to_time_scale);
    static Global *get_singleton();

    SceneTree *tree;

    PROPERTY(float, time_scale, 1.0f)

    DeltaTimerPool _delta_timer_pool;

    void _ready();
    void _physics_process(float delta);

    bool is_host_OS_web();
    bool is_host_OS_web_mobile();
    Signal wait(float wait_time, bool immune_to_time_scale = false);
    void clean_up();
    int rand_sign();
    float signfnz(float value);
    bool rand_bool();
    Node2D *find_closest_node(Node2D *node, const Array &others);
    Character *find_closest_character(Character *character, const Array &others);
    Node2D *find_closest_node_to_position(const Vector2 &position, const Array &others);
    bool chance(float probability);
    int weighted_chance(const Array &weighted_choices);
    void set_prop(Node *node, const String &prop_path, const Variant &new_prop_data);
    Variant get_prop_value(const Variant &original_value, const Variant &prop_value_data, Node *base_owner = nullptr);
    String strip_bb_code(const String &text);
    String strip_custom_data(const String &text);
    Ref<Tween> create_physics_tween();

private:
    Array _special_chars;
};

#endif // GLOBAL_H
