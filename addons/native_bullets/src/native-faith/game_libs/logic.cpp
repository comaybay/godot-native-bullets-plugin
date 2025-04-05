#include "logic.h"
#include "../character.h"
#include "options/set_fly_path_curve_options.h"
#include "options/set_curve_options.h"
#include "options/tween_move_options.h"
#include "options/tween_move_curve_options.h"
#include "global.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/geometry2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/curve2d.hpp>
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/method_tweener.hpp>
#include <godot_cpp/classes/property_tweener.hpp>

Logic::Logic()
{
    if (default_tween_curve.is_null()) {
        default_tween_curve = ResourceLoader::get_singleton()->load("res://src/game_libs/default_fly_movement_curve.tres");
    }
}

Vector2 Logic::find_hit_position(const Rect2 &target_hitbox_rect, const Vector2 &attack_origin)
{
  // https://github.com/godotengine/godot/issues/44723
  if (target_hitbox_rect.grow(1.0).has_point(attack_origin))
  {
    return attack_origin;
  }

  Vector2 hitbox_center = target_hitbox_rect.get_center();

  Vector2 intersection_left = Geometry2D::get_singleton()->segment_intersects_segment(
      attack_origin, hitbox_center,
      target_hitbox_rect.position, target_hitbox_rect.position + Vector2(0, target_hitbox_rect.size.y));

  if (intersection_left != Vector2())
  {
    return intersection_left;
  }

  Vector2 intersection_right = Geometry2D::get_singleton()->segment_intersects_segment(
      attack_origin, hitbox_center,
      target_hitbox_rect.position + target_hitbox_rect.size, target_hitbox_rect.position + Vector2(target_hitbox_rect.size.x, 0));

  if (intersection_right != Vector2())
  {
    return intersection_right;
  }

  Vector2 intersection_top = Geometry2D::get_singleton()->segment_intersects_segment(
      attack_origin, hitbox_center,
      target_hitbox_rect.position, target_hitbox_rect.position + Vector2(target_hitbox_rect.size.x, 0));

  if (intersection_top != Vector2())
  {
    return intersection_top;
  }

  // Bottom edge
  return Geometry2D::get_singleton()->segment_intersects_segment(
      attack_origin, hitbox_center,
      target_hitbox_rect.position + Vector2(0, target_hitbox_rect.size.y), target_hitbox_rect.position + target_hitbox_rect.size);
}

// Create a curve for air unit character's fly path
void Logic::set_fly_path_curve(Ref<Curve2D> curve, Ref<SetFlyPathCurveOptions> options) {
    float up_angle = (Math_PI * 0.5) * Global::get_singleton()->rand_sign();
    float direction_angle = (options->end_pos - options->character->get_global_position()).angle();

    Ref<SetCurveOptions> curve_options = memnew(SetCurveOptions);
    curve_options->start_pos = options->character->get_global_position();
    curve_options->end_pos = options->end_pos;
    curve_options->curve_angle = up_angle + direction_angle;
    curve_options->curve_length_scale = UtilityFunctions::randf_range(options->min_curve_length_scale, options->max_curve_length_scale);
    curve_options->curve_wide_scale = options->curve_wide_scale;
    
    set_curve(curve, curve_options);
    
    // Ensure character fly path curves doesn't go below the battlefield ground
    float min_allowed_height = -options->character->get_collision_rect().size.y * 0.5;
    if (curve->get_point_position(1).y > min_allowed_height) {
        up_angle = -up_angle;
        
        curve_options->curve_angle = up_angle + direction_angle;
        curve_options->curve_length_scale = UtilityFunctions::randf_range(options->min_curve_length_scale, options->max_curve_length_scale);
        
        set_curve(curve, curve_options);
    }
}

void Logic::set_curve(Ref<Curve2D> curve, Ref<SetCurveOptions> options) {
    Vector2 direction = options->end_pos - options->start_pos;
    float up_length = direction.length() * options->curve_length_scale;
    
    float dest_angle = direction.angle();
    Vector2 mid_pos = (options->start_pos + direction * 0.5) + Vector2(up_length, 0).rotated(options->curve_angle);
    
    curve->clear_points();
    curve->add_point(options->start_pos);
    curve->add_point(mid_pos, Vector2(up_length * options->curve_wide_scale, 0).rotated(dest_angle + Math_PI), Vector2(up_length * options->curve_wide_scale, 0).rotated(dest_angle));
    curve->add_point(options->end_pos);
}

// Can return null
Character* Logic::find_random_enemy(Character* character) {
    Array enemies = character->get_tree()->get_nodes_in_group(character->get_enemy_group());
    if (enemies.is_empty()) {
        return nullptr;
    } else {
        return Object::cast_to<Character>(enemies[UtilityFunctions::randi_range(0, enemies.size() - 1)]);
    }
}

// Find x position in a crowd of enemies
// character_type: type of the character that needing find the x position
// rate: 0.0 -> 1.0, 0.0 means x position of character in the frontline of the crowd, 1.0 means the position of character in the backline of the crowd
float Logic::find_x(int character_type, const Array& enemies, float rate) {
    ERR_FAIL_COND_V(enemies.size() == 0, 0.0f);
    
    float min_x = INFINITY;
    float max_x = -INFINITY;
    
    for (int i = 0; i < enemies.size(); i++) {
        Node2D* node = Object::cast_to<Node2D>(enemies[i]);
        if (node) {
            min_x = MIN(min_x, node->get_global_position().x);
            max_x = MAX(max_x, node->get_global_position().x);
        }
    }
    
    if (character_type == Character::Kind::DOG) {
        return min_x + ((max_x - min_x) * rate);
    } else {
        return min_x + ((max_x - min_x) * (1.0 - rate));
    }
}

// Duration default to character's normal speed
Ref<Tween> Logic::tween_move(Character* character, Ref<TweenMoveOptions> options) {
    Ref<Tween> tween = options->tween->is_valid() ? options->tween : character->create_tween();
    float speed = options->speed >= 0.0 ? options->speed : (float)character->_speed;
    float duration = options->duration >= 0.0 ? options->duration : options->end_pos.distance_to(character->get_global_position()) / speed;
    
    tween->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS);
    tween->tween_property(character, "global_position", options->end_pos, duration);
    
    character->get_animation_player()->play("move");
    tween->connect("finished", Callable(character->get_animation_player(), "play").bind("idle"), Object::CONNECT_ONE_SHOT);
    character->face_direction = SIGN(options->end_pos.x - character->get_global_position().x);
    return tween;
}

Ref<Tween> Logic::tween_move_curve(Character* character, Ref<TweenMoveCurveOptions> options) {
    if (options->curve.is_null()) {
       ERR_PRINT("ERROR: curve must be set");
       return Ref<Tween>();
    }
    
    float original_movement_speed_multiplier = character->status_effects.movement_speed_multiplier;
    Ref<Curve2D> curve = options->curve;
    float speed = options->speed >= 0.0 ? options->speed : character->get_speed();
    float length = curve->get_baked_length();
    float duration = options->duration > 0.0 ? options->duration : length / speed;
    
    bool use_default_easing = !options->tween.is_valid();
    
    Ref<Tween> tween = use_default_easing ? character->create_tween() : options->tween;
    tween->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS);
 
    if (options->face_toward_target != nullptr) {
        options->face_toward_target->connect("dead", callable_mp_static(&Logic::_on_target_dead), CONNECT_ONE_SHOT);
    }

    Callable callback = callable_mp_static(&Logic::_on_curve_progress);
    callback.bind(character, curve, use_default_easing, options->face_toward_target, options->face_direction, length, original_movement_speed_multiplier, tween);
    
    tween->tween_method(callback, 0.0, 1.0, duration);
    return tween;
}

void Logic::_on_target_dead(Ref<TweenMoveCurveOptions> options) {
    options->face_toward_target = nullptr;
}

void Logic::_on_curve_progress(float value, Character* character, Ref<Curve2D> curve, bool use_default_easing, Character* face_toward_target, int8_t face_direction, float length, float original_movement_speed_multiplier, Ref<Tween> tween) {
    float progress = use_default_easing ? Logic::default_tween_curve->sample_baked(value) * length : value * length;
    Vector2 new_pos = curve->sample_baked(progress);
    
    if (face_toward_target != nullptr) {
        character->face_towards(face_toward_target);
    } else if (face_direction != 0) {
        character->face_direction = face_direction;
    } else {
        character->face_direction = SIGN(new_pos.x - character->get_global_position().x);
    }
    
    character->set_global_position(new_pos);
    
    tween->set_speed_scale(character->status_effects.movement_speed_multiplier / original_movement_speed_multiplier);
}


void Logic::_bind_methods() {
    ClassDB::bind_static_method("Logic", D_METHOD("find_hit_position", "target_hitbox_rect", "attack_origin"), &Logic::find_hit_position);
    ClassDB::bind_static_method("Logic", D_METHOD("set_fly_path_curve", "curve", "options"), &Logic::set_fly_path_curve);
    ClassDB::bind_static_method("Logic", D_METHOD("set_curve", "curve", "options"), &Logic::set_curve);
    ClassDB::bind_static_method("Logic", D_METHOD("find_random_enemy", "character"), &Logic::find_random_enemy);
    ClassDB::bind_static_method("Logic", D_METHOD("find_x", "character_type", "enemies", "rate"), &Logic::find_x);
    ClassDB::bind_static_method("Logic", D_METHOD("tween_move", "character", "options"), &Logic::tween_move);
    ClassDB::bind_static_method("Logic", D_METHOD("tween_move_curve", "character", "options"), &Logic::tween_move_curve);
}
