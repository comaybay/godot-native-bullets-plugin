#ifndef SET_CURVE_OPTIONS_H
#define SET_CURVE_OPTIONS_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/core/math.hpp>

#include "../../macros.h"

using namespace godot;

class SetCurveOptions : public RefCounted {
    GDCLASS(SetCurveOptions, RefCounted);

protected:
    static void _bind_methods();

public:
    PROPERTY(Vector2, start_pos, Vector2());
    PROPERTY(Vector2, end_pos, Vector2());

    // default is curve upward
    PROPERTY(float, curve_angle, -Math_PI * 0.5);
    PROPERTY(float, curve_length_scale, 0.15);
    PROPERTY(float, curve_wide_scale, 1.0);

    static Ref<SetCurveOptions> create(const Vector2& _start_pos = Vector2(), const Vector2& _end_pos = Vector2(), 
                   float _curve_angle = -Math_PI * 0.5, float _curve_length_scale = 0.15, float _curve_wide_scale = 1.0);

    Ref<SetCurveOptions> with_points(const Vector2& _start_pos, const Vector2& _end_pos);

    Ref<SetCurveOptions> with_curvyness(float _curve_angle, float _curve_length_scale, float _curve_wide_scale);

    Ref<SetCurveOptions> with_curve_angle(float value);

    Ref<SetCurveOptions> with_curve_length_scale(float value);

    Ref<SetCurveOptions> with_curve_wide_scale(float value);
};

#endif // SET_CURVE_OPTIONS_H
