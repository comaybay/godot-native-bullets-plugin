#ifndef DELTA_TIMER_H
#define DELTA_TIMER_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/signal.hpp>

using namespace godot;

class DeltaTimer : public RefCounted
{
private:
    float time_left = 0.0f;
    float wait_time = 0.0f;
    bool immune_to_time_scale = false;
    float _speed_scale = 1.0f;
    bool _running = false;
    
protected:
    static void _bind_methods();

public:
    DeltaTimer();
    bool is_running() const;
    void start(float p_wait_time);

    // return true if the timer stopped running
    bool update(float delta);

    float get_time_left() const;
    float get_wait_time() const;
    bool get_immune_to_time_scale() const;
    float get_recover_delta() const;
    void set_immune_to_time_scale(bool p_immune);
    void set_speed_scale(float speed_scale);
};

#endif // DELTA_TIMER_H
