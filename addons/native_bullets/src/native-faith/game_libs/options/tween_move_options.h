#ifndef TWEEN_MOVE_OPTIONS_H
#define TWEEN_MOVE_OPTIONS_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include "../../macros.h"

using namespace godot;

class TweenMoveOptions : public RefCounted {
    GDCLASS(TweenMoveOptions, RefCounted);

protected:
    static void _bind_methods();

public:
    // destination position
    PROPERTY(Vector2, end_pos, Vector2());

    // default to character's speed
    // note that this speed is not the final speed, it will be multiplied by character's status_effects.movement_speed_multiplier
    PROPERTY(float, speed, -1.0f);

    // duration will take over speed if both are set
    PROPERTY(float, duration, -1.0f);

    // create own tween if not set, else will use given tween (use this if you want to set custom easing)
    PROPERTY(Ref<Tween>, tween, Ref<Tween>());

    static Ref<TweenMoveOptions> create(const Vector2& _end_pos = Vector2(), float _speed = -1.0f, 
                                        float _duration = -1.0f, Ref<Tween> _tween = nullptr);

    Ref<TweenMoveOptions> with_duration(float value);
    Ref<TweenMoveOptions> with_speed(float value);
    Ref<TweenMoveOptions> with_tween(Ref<Tween> value);
};

#endif // TWEEN_MOVE_OPTIONS_H
