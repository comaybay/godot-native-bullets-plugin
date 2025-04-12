#ifndef COOLDOWN_TIMER_H
#define COOLDOWN_TIMER_H

#include <godot_cpp/classes/ref_counted.hpp>

#include "../../macros.h"
#include "delta_timer.h"
using namespace godot;

class CooldownTimer : public RefCounted {
    GDCLASS(CooldownTimer, RefCounted);

private:
    float _speed_scale = 1.0f;
    DeltaTimer* _delta_timer = nullptr;
    bool _immune_to_time_scale = false;
    void _on_timeout();
    
protected:
    static void _bind_methods();

public:
    PROPERTY(float, wait_time, 1.0f);
    
    void set_speed_scale(float speed_scale);

    float get_time_left() const;
    void set_time_left(float time_left); // won't do anything if not running
    bool is_stopped() const;
    void start();
    void timeout_now();

    void set_immune_to_time_scale(bool enabled);
};

#endif // COOLDOWN_TIMER_H
