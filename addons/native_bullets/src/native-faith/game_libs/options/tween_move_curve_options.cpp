#include "tween_move_curve_options.h"

void TweenMoveCurveOptions::_bind_methods()
{
    BIND_STATIC_METHOD(TweenMoveCurveOptions, create, "curve", "speed", "tween");

    BIND_METHOD(with_duration, "value");
    BIND_METHOD(with_speed, "value");
    BIND_METHOD(with_tween, "value");
    BIND_METHOD(with_curve, "value");
    BIND_METHOD(with_face_toward_target, "value");
}

Ref<TweenMoveCurveOptions> TweenMoveCurveOptions::create(const Ref<Curve2D>& _curve, float _speed, 
                                                        Ref<Tween> _tween)
{
    Ref<TweenMoveCurveOptions> options;
    options.instantiate();
    options->curve = _curve;
    options->speed = _speed;
    options->tween = _tween;
    return options;
}

Ref<TweenMoveCurveOptions> TweenMoveCurveOptions::with_duration(float value)
{
    duration = value;
    return this;
}

Ref<TweenMoveCurveOptions> TweenMoveCurveOptions::with_speed(float value)
{
    speed = value;
    return this;
}

Ref<TweenMoveCurveOptions> TweenMoveCurveOptions::with_tween(Ref<Tween> value)
{
    tween = value;
    return this;
}

Ref<TweenMoveCurveOptions> TweenMoveCurveOptions::with_curve(const Ref<Curve2D>& value)
{
    curve = value;
    return this;
}

Ref<TweenMoveCurveOptions> TweenMoveCurveOptions::with_face_toward_target(Character* value)
{
    face_toward_target = value;
    return this;
}
