#include "delta_timer.h"
#include "../global.h"

DeltaTimer::DeltaTimer() {
}

void DeltaTimer::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("timeout"));
}

bool DeltaTimer::is_running() const
{
    return _running;
}

void DeltaTimer::start(float p_wait_time)
{
    wait_time = p_wait_time;
    time_left = p_wait_time;
    _running = true;
}

bool DeltaTimer::update(float delta)
{
    float time_scale = (immune_to_time_scale * Global::get_singleton()->time_scale) + ((!immune_to_time_scale) * 1.0f);
    time_left -= (delta / time_scale) * _speed_scale;

    if (time_left <= 0.0f)
    {
        time_left = 0.0f;
        _running = false;
        emit_signal("timeout");
        return true;
    }

    return false;
}

float DeltaTimer::get_time_left() const
{
    return time_left;
}

float DeltaTimer::get_wait_time() const
{
    return wait_time;
}

bool DeltaTimer::get_immune_to_time_scale() const
{
    return immune_to_time_scale;
}

void DeltaTimer::set_immune_to_time_scale(bool enabled)
{
    immune_to_time_scale = enabled;
}

void DeltaTimer::set_speed_scale(float speed_scale)
{
    _speed_scale = speed_scale;
}
