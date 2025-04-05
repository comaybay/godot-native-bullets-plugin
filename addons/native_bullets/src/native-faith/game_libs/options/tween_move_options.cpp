#include "tween_move_options.h"

void TweenMoveOptions::_bind_methods()
{
    BIND_STATIC_METHOD(TweenMoveOptions, create, "end_pos", "speed", "duration", "tween");

    BIND_METHOD(with_duration, "value");
    BIND_METHOD(with_speed, "value");
    BIND_METHOD(with_tween, "value");
}

Ref<TweenMoveOptions> TweenMoveOptions::create(const Vector2& _end_pos, float _speed, 
                                              float _duration, Ref<Tween> _tween)
{
    Ref<TweenMoveOptions> options;
    options.instantiate();
    options->end_pos = _end_pos;
    options->speed = _speed;
    options->duration = _duration;
    options->tween = _tween;
    return options;
}

Ref<TweenMoveOptions> TweenMoveOptions::with_duration(float value)
{
    duration = value;
    return this;
}

Ref<TweenMoveOptions> TweenMoveOptions::with_speed(float value)
{
    speed = value;
    return this;
}

Ref<TweenMoveOptions> TweenMoveOptions::with_tween(Ref<Tween> value)
{
    tween = value;
    return this;
}
