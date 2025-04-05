#include "fsm.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

void FSM::_bind_methods() {
    ClassDB::bind_method(D_METHOD("redirect_state", "state", "redirect_to_state"), &FSM::redirect_state);
    ClassDB::bind_method(D_METHOD("clear_state_redirections"), &FSM::clear_state_redirections);
    ClassDB::bind_method(D_METHOD("get_current_state_name"), &FSM::get_current_state_name);
    ClassDB::bind_method(D_METHOD("get_current_state"), &FSM::get_current_state);
    ClassDB::bind_method(D_METHOD("get_state", "state_name"), &FSM::get_state);
    ClassDB::bind_method(D_METHOD("has_state", "state_name"), &FSM::has_state);
    ClassDB::bind_method(D_METHOD("get_current_state_data"), &FSM::get_current_state_data);
    ClassDB::bind_method(D_METHOD("add_state", "name", "state"), &FSM::add_state);
    ClassDB::bind_method(D_METHOD("register_state", "state"), &FSM::_register_state);
    ClassDB::bind_method(D_METHOD("change_state", "next_state_name", "data"), &FSM::change_state, DEFVAL(Dictionary()));
    ClassDB::bind_method(D_METHOD("set_disabled", "disabled"), &FSM::set_disabled);

    BIND_PROPERTY(initial_state, Variant::STRING_NAME, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(state_owner, Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Node", PROPERTY_USAGE_DEFAULT);

    ADD_SIGNAL(MethodInfo("state_entering", PropertyInfo(Variant::STRING_NAME, "state_name"), PropertyInfo(Variant::DICTIONARY, "state_data")));
    ADD_SIGNAL(MethodInfo("state_entered", PropertyInfo(Variant::STRING_NAME, "state_name")));
    ADD_SIGNAL(MethodInfo("state_exited", PropertyInfo(Variant::STRING_NAME, "state_name"), PropertyInfo(Variant::STRING_NAME, "next_state_name")));
}

FSM::FSM()
{
}

FSM::~FSM()
{
}

void FSM::redirect_state(const StringName& state, const StringName& redirect_to_state) {
    _redirect_state_map[state] = redirect_to_state;
}

void FSM::clear_state_redirections() {
    _redirect_state_map.clear();
}

void FSM::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        set_process(false);
        set_process_input(false);
        set_physics_process(false);
        return;
    }

    current_state = get_state(initial_state);
    state_owner->connect("ready", callable_mp(this, &FSM::_on_owner_ready), Object::CONNECT_ONE_SHOT);
    
    connect("tree_exiting", callable_mp(this, &FSM::_on_tree_exiting));
}

StringName FSM::get_current_state_name() const {
    return current_state->state_name;
}

Ref<FSMState> FSM::get_current_state() const {
    return current_state;
}

Ref<FSMState> FSM::get_state(const StringName& state_name) const {
    for (int i = 0; i < states.size(); i++) {
        Ref<FSMState> state = states[i];
        if (state->state_name == state_name) {
            return state;
        }
    }

    return nullptr;
}

bool FSM::has_state(const StringName& state_name) const {
    return get_state(state_name).is_valid();
}

Dictionary FSM::get_current_state_data() const {
    return _state_data;
}

void FSM::add_state(Ref<FSMState> state) {
    _register_state(state);
    states.append(state);
}

void FSM::_on_owner_ready() {
    for (int i = 0; i < states.size(); i++) {
        Ref<FSMState> state = states[i];
        if (state->fsm != this) {
            _register_state(state);
        }
    }
    
    _state_data = Dictionary();
    _state_data["prev_state"] = Variant();
    _state_data["next_state"] = Variant();
    
    current_state->data = _state_data;
    current_state->state_entered = false;
    emit_signal("state_entering", initial_state, _state_data);
    update_FSM_process();
    
    if (GDVIRTUAL_IS_OVERRIDDEN_PTR(current_state, enter)) {
        GDVIRTUAL_CALL_PTR(current_state, enter);
    }
    
    // in case already changed and entered other state while is entering this state
    if (!current_state->state_entered) {
        current_state->state_entered = true;
        emit_signal("state_entered", initial_state);
    }
}

void FSM::_register_state(Ref<FSMState> state) {
    state->fsm = Ref(this);
    GDVIRTUAL_CALL_PTR(state, setup, state_owner);
}

void FSM::change_state(const StringName& state_name, Dictionary& data) {
    StringName next_state_name = state_name;
    
    if (_redirect_state_map.has(next_state_name)) {
        _state_data["redirected_from"] = next_state_name;
        next_state_name = _redirect_state_map[next_state_name];
    }

    _state_data["next_state"] = next_state_name;
    
    data["prev_state"] = get_current_state_name();
    _state_data = data;
    
    // if change state before ready, the state will be run by _on_owner_ready instead
    if (state_owner->is_connected("ready", callable_mp(this, &FSM::_on_owner_ready))) {
        ERR_FAIL_COND_MSG(!has_state(next_state_name), "ERROR: state " + next_state_name + " does not exist");
        current_state = get_state(next_state_name);
        return;
    }
    
    // in case state changed during enter()
    if (!current_state->state_entered && !current_state->state_entering && !current_state->state_exited) {
        current_state->state_entered = true;
        emit_signal("state_entered", current_state->state_name);
        current_state->state_entered = false;
    }
    
    if (!current_state->state_exited && GDVIRTUAL_IS_OVERRIDDEN_PTR(current_state, exit)) {
        GDVIRTUAL_CALL_PTR(current_state, exit);
    }

    current_state->state_exited = true;
    emit_signal("state_exited", current_state->state_name, next_state_name);
    
    ERR_FAIL_COND_MSG(!has_state(next_state_name), "ERROR: state " + next_state_name + " does not exist");
    
    current_state->state_exited = true;
    current_state->state_entered = false;
    current_state->state_entering = false;
    
    Ref<FSMState> new_state = get_state(next_state_name);

    new_state->data = _state_data;
    new_state->state_exited = false;
    new_state->state_entered = false;
    new_state->state_entering = true;
    
    emit_signal("state_entering", next_state_name, _state_data);

    new_state->state_entering = false;

    // in case already changed and entered other state while is entering this state
    if (current_state->state_entered) {
        new_state->state_exited = true;
        return;
    }
    
    current_state = new_state;
    new_state->state_entering = false;
    
    update_FSM_process();
    
    if (GDVIRTUAL_IS_OVERRIDDEN_PTR(new_state, enter)) {
        GDVIRTUAL_CALL_PTR(new_state, enter);
    }
    
    // in case state changed during enter()
    if (current_state != new_state) return;
    
    current_state->state_entered = true;
    current_state->state_exited = false;
    emit_signal("state_entered", next_state_name);
}

void FSM::update_FSM_process() {
    set_physics_process(GDVIRTUAL_IS_OVERRIDDEN_PTR(current_state, physics_update));
    set_process_input(GDVIRTUAL_IS_OVERRIDDEN_PTR(current_state, input));
    set_process(GDVIRTUAL_IS_OVERRIDDEN_PTR(current_state, update));
}

void FSM::_process(double delta) {
    GDVIRTUAL_CALL_PTR(current_state, update, delta);
}

void FSM::_physics_process(double delta) {
    GDVIRTUAL_CALL_PTR(current_state, physics_update, delta);
}

void FSM::_input(const Ref<InputEvent>& event) {
    GDVIRTUAL_CALL_PTR(current_state, input, event);
}

PackedStringArray FSM::_get_configuration_warnings() const {
    PackedStringArray warnings;

    TypedArray<Node> children = get_children();
    if (children.size() == 0) {
        warnings.push_back("FSM requires at least one state to operate");
    } else if (initial_state.is_empty()) {
        warnings.push_back("Please select the Initial State");
    }
    
    return warnings;
}

void FSM::_on_tree_exiting() {
    if (is_queued_for_deletion() && current_state->has_method("exit")) {
        GDVIRTUAL_CALL_PTR(current_state, exit);
        emit_signal("state_exited", current_state->state_name, String());
    }
}

void FSM::set_disabled(bool disabled) {
    if (disabled) {
        set_physics_process(false);
        set_process_input(false);
        set_process(false);
    } else {
        update_FSM_process();
    }
}
