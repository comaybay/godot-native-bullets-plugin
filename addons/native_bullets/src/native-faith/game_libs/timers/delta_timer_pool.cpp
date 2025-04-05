#include "delta_timer_pool.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

DeltaTimerPool::DeltaTimerPool() : _unused_timers(50), _active_timers(50), _timers(50)
{
}

DeltaTimer &DeltaTimerPool::get_timer(float wait_time, bool immune_to_time_scale)
{
    DeltaTimer *p_timer;
    if (_unused_timers.size() > 0)
    {
        p_timer = _unused_timers.pop();
        p_timer->set_immune_to_time_scale(immune_to_time_scale);
        p_timer->start(wait_time);
    }
    else
    {
        p_timer = &_timers.emplace_back();
        p_timer->set_immune_to_time_scale(immune_to_time_scale);
        p_timer->start(wait_time);
    }

    _active_timers.push_back(p_timer);
    return *p_timer;
}

void DeltaTimerPool::physics_process(float delta)
{
    for (int i = _active_timers.size() - 1; i >= 0; i--)
    {
        DeltaTimer* p_timer = _active_timers[i];
        bool stopped_running = p_timer->update(delta);
        
        if (stopped_running)
        {
            _active_timers.swap_remove(i);
            _unused_timers.push_back(p_timer);
        }
    }
}

void DeltaTimerPool::clear()
{
    _unused_timers.clear();
    _active_timers.clear();
    _timers.clear();
}
