#ifndef TWEEN_MOVE_CURVE_OPTIONS_H
#define TWEEN_MOVE_CURVE_OPTIONS_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/curve2d.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include "../../macros.h"
#include "../../character.h"

using namespace godot;

// NOTE: Do not reuse this object since it will be modified by the method, 
// use duplicate() if you have to
class TweenMoveCurveOptions : public RefCounted {
    GDCLASS(TweenMoveCurveOptions, RefCounted);

protected:
    static void _bind_methods();

public:
    // curve to move the character along with
    PROPERTY(Ref<Curve2D>, curve, Ref<Curve2D>());

    // default to character's speed
    // note that this speed is not the final speed, it will be multiplied by character's status_effects.movement_speed_multiplier
    PROPERTY(float, speed, -1.0f);

    // duration will take over speed if both are set
    PROPERTY(float, duration, -1.0f);

    // create own tween if not set, else will use given tween
    PROPERTY(Ref<Tween>, tween, nullptr);

    // default will look at direction of the movement
    PROPERTY(int, face_direction, 0);

    // if set, while moving character will face towards this target
    // this will automatically set to null if target is dead
    PROPERTY(Character*, face_toward_target, nullptr);

    static Ref<TweenMoveCurveOptions> create(const Ref<Curve2D>& _curve = Ref<Curve2D>(), float _speed = -1.0f, 
                                            Ref<Tween> _tween = nullptr);

    Ref<TweenMoveCurveOptions> with_duration(float value);
    Ref<TweenMoveCurveOptions> with_speed(float value);
    Ref<TweenMoveCurveOptions> with_tween(Ref<Tween> value);
    Ref<TweenMoveCurveOptions> with_curve(const Ref<Curve2D>& value);
    Ref<TweenMoveCurveOptions> with_face_toward_target(Character* value);
};

#endif // TWEEN_MOVE_CURVE_OPTIONS_H
