#ifndef FSM_H
#define FSM_H

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include "../../macros.h"
#include "fsm_state.h"

using namespace godot;


class FSM : public Node {
    GDCLASS(FSM, Node);

private:
    Ref<FSMState> current_state;
    Dictionary _state_data;
    Dictionary _redirect_state_map;

    void _on_owner_ready();
    void _on_state_transition(const String& next_state_name, const Dictionary& data = Dictionary());
    void _on_tree_exiting();
    void update_FSM_process();
    void _register_state(Ref<FSMState> state);

protected:
    static void _bind_methods();

public:
    FSM();
    ~FSM();

    void _ready() override;
    void _process(double delta) override;
    void _physics_process(double delta) override;
    void _input(const Ref<InputEvent>& event) override;
    PackedStringArray _get_configuration_warnings() const override;

    bool paused;

    void redirect_state(const StringName& state, const StringName& redirect_to_state);
    void clear_state_redirections();
    StringName get_current_state_name() const;
    Ref<FSMState> get_current_state() const;
    Ref<FSMState> get_state(const StringName& state_name) const;
    bool has_state(const StringName& state_name) const;
    Dictionary get_current_state_data() const;
    void add_state(Ref<FSMState> state);
    void change_state(const StringName& next_state_name, Dictionary data = Dictionary());
    void set_disabled(bool disabled);

    PROPERTY(TypedArray<RefCounted>, states, {});
    PROPERTY(Node*, state_owner, {});
    PROPERTY(StringName, initial_state, {});
};

#endif // FSM_H
