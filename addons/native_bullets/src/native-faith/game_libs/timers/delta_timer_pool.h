#ifndef DELTA_TIMER_POOL_H
#define DELTA_TIMER_POOL_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <unordered_map>
#include "delta_timer.h"
#include "../../structures.h"

using namespace godot;

class DeltaTimerPool {

private:
    std::vector<DeltaTimer> _timers;
    UnorderedVector<DeltaTimer*> _active_timers;
    UnorderedVector<DeltaTimer*> _unused_timers;

public:
    DeltaTimerPool();
    // Returns a timer.
    // The timer might be shared if they are in the same physics frame and same wait time
    // Timers might be created / reused depending if there are enough timers to be shared
    DeltaTimer& get_timer(float wait_time, bool immune_to_time_scale);
    
    inline void physics_process(float delta);
    
    void clear();
};

#endif // DELTA_TIMER_POOL_H
