#ifndef SET_FLY_PATH_CURVE_OPTIONS_H
#define SET_FLY_PATH_CURVE_OPTIONS_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include "../../macros.h"
#include "../../character.h"

using namespace godot;

class SetFlyPathCurveOptions : public RefCounted {
    GDCLASS(SetFlyPathCurveOptions, RefCounted);

protected:
    static void _bind_methods();

public:
    PROPERTY(Character*, character, nullptr);
    PROPERTY(Vector2, end_pos, Vector2());
    PROPERTY(float, min_curve_length_scale, 0.15);
    PROPERTY(float, max_curve_length_scale, 0.3);
    PROPERTY(float, curve_wide_scale, 1.0);

    static Ref<SetFlyPathCurveOptions> create(Character* _character = nullptr, const Vector2& _end_pos = Vector2(), 
                          float _min_curve_length_scale = 0.15, float _max_curve_length_scale = 0.3, 
                          float _curve_wide_scale = 1.0);

    Ref<SetFlyPathCurveOptions> with_character(Character* value);
    Ref<SetFlyPathCurveOptions> with_end_pos(const Vector2& value);
    Ref<SetFlyPathCurveOptions> with_min_curve_length_scale(float value);
    Ref<SetFlyPathCurveOptions> with_max_curve_length_scale(float value);
    Ref<SetFlyPathCurveOptions> with_curve_wide_scale(float value);
};

#endif // SET_FLY_PATH_CURVE_OPTIONS_H
