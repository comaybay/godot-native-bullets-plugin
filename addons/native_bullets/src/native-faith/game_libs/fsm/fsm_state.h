#ifndef FSM_STATE_H
#define FSM_STATE_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

#include "../../macros.h"
using namespace godot;

class FSM;

class FSMState : public RefCounted {
    GDCLASS(FSMState, RefCounted);

protected:
    static void _bind_methods();

public:
    GDVIRTUAL1(setup, Node*);
    GDVIRTUAL0(enter);
    GDVIRTUAL0(exit);
    GDVIRTUAL1(update, float);
    GDVIRTUAL1(physics_update, float);
    GDVIRTUAL1(input, const Ref<InputEvent>&);
    
    Ref<FSM> fsm;
    Dictionary data;

    bool state_entered;
    bool state_entering;
    bool state_exited;
    PROPERTY(StringName, state_name, "");
};

#endif // FSM_STATE_H
