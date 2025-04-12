#ifndef CHARACTER_H
#define CHARACTER_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/canvas_group.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters2d.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/transform2d.hpp>

// https://forum.godotengine.org/t/how-to-bind-a-c-virtual-method-to-gdscript/42024/6
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

#include "special_attack_turn_checker.h"
#include "talents/talents.h"
#include "power_system/status_effects/status_effects.h"
#include "macros.h"
#include "game_libs/timers/cooldown_timer.h"
#include "game_libs/fsm/fsm.h"

using namespace godot;

class Character : public Area2D
{
  GDCLASS(Character, Area2D);

public:
  // Enums
  enum Kind
  {
    DOG,
    CAT,
  };

  enum Relationship {
    FRIEND,
    FOE,
  };

  enum UnitType
  {
    GROUND,
    AIR
  };

  enum AttackType
  {
    ATTACK_SINGLE,
    ATTACK_AREA,
    ATTACK_UNIQUE
  };

  // Pooling
  uint32_t index_in_pool;
  StatusEffects status_effects = {};

  // Constants
  static constexpr uint32_t MIN_GROUND_CHARACTER_Z_INDEX = 0;
  static constexpr uint32_t MAX_GROUND_CHARACTER_Z_INDEX = 40;

  // to avoid miss detection
  static constexpr uint32_t MIN_ATTACK_AREA_HEIGHT = 120;

  static inline const StringName GROUP_CHARACTERS = "characters";
  static inline const StringName GROUP_CATS = "cats";
  static inline const StringName GROUP_DOGS = "dogs";
  static inline const StringName GROUP_AIR_UNIT_CATS = "air_unit_cats";
  static inline const StringName GROUP_AIR_UNIT_DOGS = "air_unit_dogs";
  static inline const StringName GROUP_AIR_UNITS = "air_units";

  static constexpr uint32_t DOG_COLLISION_MASK = 0b010000;
  static constexpr uint32_t CAT_COLLISION_MASK = 0b100000;
  static constexpr uint32_t DOG_COLLISION_LAYER = 0b10000000010;
  static constexpr uint32_t CAT_COLLISION_LAYER = 0b10000000100;
  static constexpr uint32_t AIR_UNIT_DOG_COLLISION_LAYER = 0b10000000;
  static constexpr uint32_t AIR_UNIT_CAT_COLLISION_LAYER = 0b1000000;
  static constexpr uint32_t DOG_ENEMY_DETECTION_MASK = 0b010100;
  static constexpr uint32_t CAT_ENEMY_DETECTION_MASK = 0b100010;
  static constexpr uint32_t DOG_DANMAKU_HITBOX_COLLISION_LAYER = 0b0100000000;
  static constexpr uint32_t CAT_DANMAKU_HITBOX_COLLISION_LAYER = 0b1000000000;
  static constexpr uint32_t DOG_CUSTOM_AREA_COLLISION_MASK = 0b01010100;
  static constexpr uint32_t CAT_CUSTOM_AREA_COLLISION_MASK = 0b10100010;

  static constexpr float FACING_ANIMATION_DURATION = 0.075f;

  static Ref<AudioStream> DEFAULT_ATTACK_HIT_SFX;
  static Ref<AudioStream> DEFAULT_DIE_SFX;
  static Ref<AudioStream> DEFAULT_BOSS_DIE_SFX;

  Character();
  ~Character();

private:
  Node* _audio_player = nullptr;

  // TODO: remove once implemented own bullet system
  Node* _native_bullets = nullptr;

  GDVIRTUAL1(_on_setup_power_scaling, float);
  void _setup_power_scaling(float power_scale);

  GDVIRTUAL0(_on_ready);
  GDVIRTUAL0(_on_updated_character);
  GDVIRTUAL3(_handle_damage_taken, uint32_t, Variant, Node*);
  GDVIRTUAL0(_handle_knockedback_by_damage_taken);

  // Methods
  void _handle_past_knockback_health();
  void _update_character();
  void _update_charcter_collision_shapes();
  void _setup_rect_shape_query();
  void _update_next_knockback_health();
  void _apply_sprite_offset();
  void _update_rect_shape_query_attack_range();
  void _update_character_stats();
  uint32_t _calculate_next_knockback_health() const;

public:
  void setup(Vector2 global_position, int level, TypedArray<String> abilities, bool special_attack_unlocked, Relationship relationship, bool is_boss = false);
  void update_rect_shape_query_for_idle();
  void update_rect_shape_query_for_attack();
  void update_rect_shape_query_for_air_unit_enemy_detection();
  TypedArray<Dictionary> intersect_rect_shape();
  Dictionary intersect_rect_shape_get_closest();
  bool is_rect_shape_colliding();
  bool is_on_ground() const;
  bool is_above_ground() const;
  bool _is_past_knockback_health() const;
  void knockback(float scale = 1.0);
  void kill();
  void play_attack_sfx();
  void _on_danmaku_bullet_entered(RID area_rid, Area2D *area, int area_shape_index, int local_shape_index);
  bool is_immune_to_attack();
  void set_mouse_area_detectable(bool detectable);
  void set_immune_to_attack(bool immune);
  bool is_knockedback() const;
  float move(float delta);
  void face_towards(Character *target);
  void reset();
  void take_damage(uint32_t amount, Variant attack, Node *attacker);
  void add_status_effect(Ref<StatusEffect> status_effect);
  void remove_status_effect(Ref<StatusEffect> status_effect);

  // Helper methods
  Vector2 get_hitbox_size() const;
  Rect2 get_hitbox_rect() const;
  Vector2 get_danmaku_hitbox_position() const;
  Kind get_enemy_type() const;
  StringName get_enemy_group() const;
  StringName get_air_unit_enemy_group() const;
  StringName get_air_unit_group() const;
  uint32_t get_actual_z_index() const;
  uint32_t get_knockbacks_left() const;
  float get_power_scale() const;
  Vector2 get_effect_center_global_position() const;
  Vector2 get_bottom_global_position() const;

  // Properties
  PROPERTY(uint32_t, reward_money, 0);
  PROPERTY(Kind, kind, Kind::DOG);
  PROPERTY(Relationship, relationship, Relationship::FRIEND);
  PROPERTY(UnitType, unit_type, UnitType::GROUND);
  PROPERTY(AttackType, attack_type, AttackType::ATTACK_SINGLE);

  PROPERTY(bool, has_special_attack, false);

  void add_buff(Ref<StatusEffect> status_effect);
  void add_debuff(Ref<StatusEffect> status_effect);
  void remove_buff(Ref<StatusEffect> status_effect);
  void remove_debuff(Ref<StatusEffect> status_effect);

private: bool _special_attack_unlocked = false;
public:
  bool is_able_to_use_special_attack() const;
  void set_1hp_mode(bool enable);

  PROPERTY(uint32_t, base_speed, 120);
  float _speed;
  float get_speed() const;
  void update_speed();

  PROPERTY(float, base_attack_cooldown, 2.0f);
  float _attack_cooldown;
  float get_attack_cooldown() const;
  void update_attack_cooldown();
  
  PROPERTY(uint32_t, health, 160)
  float _defense_multiplier;
  float get_defense_multiplier() const;
  void update_defense();

  PROPERTY(TypedArray<uint32_t>, base_attack_damages, {})
  std::vector<uint32_t> _attack_damages;
  uint32_t get_base_attack_damage(uint32_t index = 0) const;
  uint32_t get_attack_damage(uint32_t index = 0) const;
  void update_attack_damage();

  PROPERTY(float, animation_speed_scale, 1.0f)
  void update_animation_speed();

  PROPERTY(Ref<SpecialAttackTurnChecker>, special_attack_turn_checker, nullptr)
  PROPERTY(Ref<Talents>, talents, nullptr)
  PROPERTY(Area2D *, custom_attack_area, nullptr)
  PROPERTY(uint32_t, attack_range, 40)
  PROPERTY(bool, is_long_range_attack, false)
  PROPERTY(uint32_t, attack_area, 0)
  PROPERTY(float, attack_area_offset, 0.0f)
  PROPERTY(uint32_t, attack_area_height, MIN_ATTACK_AREA_HEIGHT)
  PROPERTY(float, time_idle_after_attack, 0.0f)
  PROPERTY(uint32_t, knockbacks, 3)

  // dictate the direction the character will be going to
  PROPERTY(int8_t, march_direction, 1)

  // dictate the direction the character is currently facing
  PROPERTY(int8_t, face_direction, 1)

private:
  bool _1hp_mode = false;

public:

  PROPERTY(Ref<AudioStream>, attack_hit_sfx, nullptr)
  PROPERTY(Ref<AudioStream>, attack_sfx, nullptr)
  PROPERTY(Ref<AudioStream>, before_death_sfx, nullptr)
  PROPERTY(Ref<AudioStream>, die_sfx, nullptr)

  PROPERTY(Ref<PhysicsShapeQueryParameters2D>, rect_shape_query, nullptr)
  PROPERTY(Vector2, velocity, Vector2())

  PROPERTY_GETONLY(String, character_id, "")

  PROPERTY_GETONLY(Vector2, sprite_offset, Vector2())

  bool _is_boss = false;
  bool is_boss() const;

  PROPERTY_GETONLY(uint32_t, level, 1)
  PROPERTY_GETONLY(uint32_t, max_health, 160)
  PROPERTY_GETONLY(uint32_t, next_knockback_health, 0)

  // Node references
  PROPERTY_GETONLY(AnimationPlayer *, animation_player, nullptr)
  PROPERTY_GETONLY(FSM*, fsm, nullptr)
  PROPERTY_GETONLY(Node2D *, character_animation_node, nullptr)
  PROPERTY_GETONLY(CooldownTimer *, attack_cooldown_timer, nullptr)
  PROPERTY_GETONLY(CollisionShape2D *, hitbox, nullptr)
  PROPERTY_GETONLY(Area2D *, danmaku_hitbox_area2d, nullptr)
  PROPERTY_GETONLY(CollisionShape2D *, danmaku_hitbox, nullptr)

private:
  TypedArray<String> _abilities;

public:
  bool has_ability(StringName ability) const;

  // Internal method, will be called by spawner when z_index is modified by props
private:
  bool __z_index_already_set = false;

public:
  void __z_index_modified_by_props();

private:
  float _prev_cooldown_multiplier = 1.0f;
  Ref<Tween> _face_anim_tween = nullptr;
  Ref<RectangleShape2D> _hitbox_shape = nullptr;

  // don't ask me wtf im doing with this
private:
  CanvasGroup *__effect_canvas_group = nullptr;

public:
  void __set_effect_canvas_group(CanvasGroup *group);

private:
  void _super_ready();

protected:
  static void _bind_methods();

public:
  void _ready() override;
	void _validate_property(PropertyInfo &p_property) const;
  void _notification(int p_what);
};

VARIANT_ENUM_CAST(Character::Kind);
VARIANT_ENUM_CAST(Character::Relationship);
VARIANT_ENUM_CAST(Character::UnitType);
VARIANT_ENUM_CAST(Character::AttackType);

#endif
