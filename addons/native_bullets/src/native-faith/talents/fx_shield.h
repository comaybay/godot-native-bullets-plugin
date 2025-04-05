#ifndef FX_SHIELD_H
#define FX_SHIELD_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

class FxShield : public Sprite2D {
    GDCLASS(FxShield, Sprite2D);

protected:
    static void _bind_methods();

private:
    bool __preview;
    Ref<Tween> _tween;

public:
    void set_preview(bool value);
    bool get_preview() const;

    void _enter_tree();
    void _ready();
    
    void setup(const Vector2& global_pos, float angle);
    Ref<Tween> play_animation();
};

#endif // FX_SHIELD_H
