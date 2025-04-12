#ifndef LOGIC_H
#define LOGIC_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/curve2d.hpp>
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/geometry2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include "../character.h"

using namespace godot;

class SetFlyPathCurveOptions;
class SetCurveOptions;
class TweenMoveOptions;
class TweenMoveCurveOptions;

class Logic : public RefCounted {
    GDCLASS(Logic, RefCounted);

protected:
    static void _bind_methods();

public:
    Logic();

    static Vector2 find_hit_position(const Rect2& target_hitbox_rect, const Vector2& attack_origin);

    // Create a curve for air unit character's fly path
    static void set_fly_path_curve(Ref<Curve2D> curve, Ref<SetFlyPathCurveOptions> options);
    
    static void set_curve(Ref<Curve2D> curve, Ref<SetCurveOptions> options);

    // Can return null
    static Character* find_random_enemy(Character* character);

    // Find x position in a crowd of enemies
    // kind: type of the character that needing find the x position
    // rate: 0.0 -> 1.0, 0.0 means x position of character in the frontline of the crowd, 1.0 means the position of character in the backline of the crowd
    static float find_x(Character::Kind kind, const Array& enemies, float rate);

    static Ref<Curve> default_tween_curve;
    
    // Duration default to character's normal speed
    static Ref<Tween> tween_move(Character* character, Ref<TweenMoveOptions> options);
    
    static Ref<Tween> tween_move_curve(Character* character, Ref<TweenMoveCurveOptions> options);

private:
    static void _on_target_dead(Ref<TweenMoveCurveOptions> options);
    static void _on_curve_progress(float value, Character* character, Ref<Curve2D> curve, bool use_default_easing, Character* face_toward_target, int8_t face_direction, float length, float original_movement_speed_multiplier, Ref<Tween> tween);
};

#endif // LOGIC_H
