#include "set_curve_options.h"

void SetCurveOptions::_bind_methods()
{
    BIND_STATIC_METHOD(SetCurveOptions, create, "start_pos", "end_pos", "curve_angle", "curve_length_scale", "curve_wide_scale");
   
    BIND_METHOD(with_points, "start_pos", "end_pos");
    BIND_METHOD(with_curvyness, "curve_angle", "curve_length_scale", "curve_wide_scale");
    BIND_METHOD(with_curve_angle, "value");
    BIND_METHOD(with_curve_length_scale, "value");
    BIND_METHOD(with_curve_wide_scale, "value");
}

Ref<SetCurveOptions> SetCurveOptions::create(const Vector2 &_start_pos, const Vector2 &_end_pos,
                                             float _curve_angle, float _curve_length_scale, float _curve_wide_scale)
{
  Ref<SetCurveOptions> options;
  options.instantiate();
  options->start_pos = _start_pos;
  options->end_pos = _end_pos;
  options->curve_angle = _curve_angle;
  options->curve_length_scale = _curve_length_scale;
  options->curve_wide_scale = _curve_wide_scale;
  return options;
}

Ref<SetCurveOptions> SetCurveOptions::with_points(const Vector2& _start_pos, const Vector2& _end_pos) {
    start_pos = _start_pos;
    end_pos = _end_pos;
    return this;
}

Ref<SetCurveOptions> SetCurveOptions::with_curvyness(float _curve_angle, float _curve_length_scale, float _curve_wide_scale) {
    curve_angle = _curve_angle;
    curve_length_scale = _curve_length_scale;
    curve_wide_scale = _curve_wide_scale;
    return this;
}

Ref<SetCurveOptions> SetCurveOptions::with_curve_angle(float value) {
    curve_angle = value;
    return this;
}

Ref<SetCurveOptions> SetCurveOptions::with_curve_length_scale(float value) {
    curve_length_scale = value;
    return this;
}

Ref<SetCurveOptions> SetCurveOptions::with_curve_wide_scale(float value) {
    curve_wide_scale = value;
    return this;
}