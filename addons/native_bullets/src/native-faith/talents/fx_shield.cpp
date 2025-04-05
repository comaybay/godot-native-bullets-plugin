#include "fx_shield.h"

#include <godot_cpp/classes/interval_tweener.hpp>
#include <godot_cpp/classes/property_tweener.hpp>

void FxShield::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_preview", "value"), &FxShield::set_preview);
    ClassDB::bind_method(D_METHOD("get_preview"), &FxShield::get_preview);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "__preview", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "set_preview", "get_preview");
}

void FxShield::set_preview(bool value) {
    __preview = true;
    Ref<Tween> tween = play_animation();
    if (Engine::get_singleton()->is_editor_hint()) {
        tween->connect("finished", callable_mp(this, &FxShield::set_preview).bind(false));
    }
}

bool FxShield::get_preview() const {
    return __preview;
}

void FxShield::_enter_tree() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        set_modulate(Color(get_modulate().r, get_modulate().g, get_modulate().b, 0.0f));
    }
}

void FxShield::setup(const Vector2& global_pos, float angle) {
    set_global_position(global_pos);
    set_rotation(angle);
}

void FxShield::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        Ref<Tween> tween = play_animation();
        tween->connect("finished", callable_mp(static_cast<Node*>(this), &FxShield::queue_free));
    }
}

Ref<Tween> FxShield::play_animation() {
    if (_tween.is_valid()) {
        _tween->kill();
    }
    
    _tween = create_tween();
    _tween->set_trans(Tween::TRANS_EXPO);
    _tween->set_ease(Tween::EASE_IN);
    _tween->tween_property(this, "modulate:a", 1.0, 0.05);
    _tween->tween_interval(0.1);
    _tween->set_trans(Tween::TRANS_SINE);
    _tween->set_ease(Tween::EASE_IN_OUT);
    _tween->tween_property(this, "modulate:a", 0.0, 0.4);
    
    return _tween;
}
