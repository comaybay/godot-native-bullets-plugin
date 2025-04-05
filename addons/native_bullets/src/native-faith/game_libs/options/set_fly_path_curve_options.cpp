#include "set_fly_path_curve_options.h"

void SetFlyPathCurveOptions::_bind_methods()
{
    BIND_STATIC_METHOD(SetFlyPathCurveOptions, create, "character", "end_pos", "min_curve_length_scale", "max_curve_length_scale", "curve_wide_scale");

    BIND_METHOD(with_character, "value");
    BIND_METHOD(with_end_pos, "value");
    BIND_METHOD(with_min_curve_length_scale, "value");
    BIND_METHOD(with_max_curve_length_scale, "value");
    BIND_METHOD(with_curve_wide_scale, "value");
}
Ref<SetFlyPathCurveOptions> SetFlyPathCurveOptions::create(Character* _character, const Vector2& _end_pos, 
                                              float _min_curve_length_scale, float _max_curve_length_scale, 
                                              float _curve_wide_scale)
{
    Ref<SetFlyPathCurveOptions> options;
    options.instantiate();
    options->character = _character;
    options->end_pos = _end_pos;
    options->min_curve_length_scale = _min_curve_length_scale;
    options->max_curve_length_scale = _max_curve_length_scale;
    options->curve_wide_scale = _curve_wide_scale;
    return options;
}

Ref<SetFlyPathCurveOptions> SetFlyPathCurveOptions::with_character(Character* value)
{
    character = value;
    return this;
}

Ref<SetFlyPathCurveOptions> SetFlyPathCurveOptions::with_end_pos(const Vector2& value)
{
    end_pos = value;
    return this;
}

Ref<SetFlyPathCurveOptions> SetFlyPathCurveOptions::with_min_curve_length_scale(float value)
{
    min_curve_length_scale = value;
    return this;
}

Ref<SetFlyPathCurveOptions> SetFlyPathCurveOptions::with_max_curve_length_scale(float value)
{
    max_curve_length_scale = value;
    return this;
}

Ref<SetFlyPathCurveOptions> SetFlyPathCurveOptions::with_curve_wide_scale(float value)
{
    curve_wide_scale = value;
    return this;
}
