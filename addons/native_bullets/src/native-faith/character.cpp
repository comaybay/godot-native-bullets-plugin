#include "character.h"
#include "game_libs/global.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/physics_server2d.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/physics_direct_space_state2d.hpp>
#include <godot_cpp/classes/world2d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

using namespace godot;

Character::Character() {
    if (DEFAULT_ATTACK_HIT_SFX.is_null()) {
        DEFAULT_ATTACK_HIT_SFX = ResourceLoader::get_singleton()->load("res://resources/sound/bite.mp3");
        DEFAULT_DIE_SFX = ResourceLoader::get_singleton()->load("res://resources/sound/death.mp3");
        DEFAULT_BOSS_DIE_SFX = ResourceLoader::get_singleton()->load("res://resources/sound/boss_knockback_cry.mp3");
    }
}

Character::~Character() {
    PhysicsServer2D::get_singleton()->free_rid(rect_shape_query->get_shape_rid());
}

void Character::_bind_methods() {
    GDVIRTUAL_BIND(_on_setup_power_scaling);
    GDVIRTUAL_BIND(_on_updated_character);
    GDVIRTUAL_BIND(_handle_damage_taken, "amount", "attack", "attacker");
    GDVIRTUAL_BIND(_handle_knockedback_by_damage_taken);

    // Register enums
    BIND_ENUM_CONSTANT(DOG);
    BIND_ENUM_CONSTANT(CAT);
    
    BIND_ENUM_CONSTANT(GROUND);
    BIND_ENUM_CONSTANT(AIR);
    
    BIND_ENUM_CONSTANT(ATTACK_SINGLE);
    BIND_ENUM_CONSTANT(ATTACK_AREA);
    BIND_ENUM_CONSTANT(ATTACK_UNIQUE);
    
    // Register properties
    BIND_METHOD(get_character_id);
    BIND_PROPERTY(kind, Variant::INT, PROPERTY_HINT_ENUM, "DOG,CAT", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(unit_type, Variant::INT, PROPERTY_HINT_ENUM, "GROUND,AIR", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(attack_type, Variant::INT, PROPERTY_HINT_ENUM, "SINGLE,AREA,CUSTOM_AREA,UNIQUE", PROPERTY_USAGE_DEFAULT);

    BIND_PROPERTY(base_speed, INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_METHOD(get_speed);

    BIND_PROPERTY(base_attack_cooldown, FLOAT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_METHOD(get_attack_cooldown);
    
    BIND_PROPERTY(base_attack_damages, ARRAY, PROPERTY_HINT_ARRAY_TYPE, "int", PROPERTY_USAGE_DEFAULT);
    BIND_METHOD_1D1(get_attack_damage, "index", DEFVAL(0));
    BIND_METHOD_1D1(get_base_attack_damage, "index", DEFVAL(0));
    BIND_METHOD(get_defense_multiplier);

    BIND_PROPERTY(reward_money, INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(has_special_attack, BOOL, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(special_attack_turn_checker, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "SpecialAttackChecker", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(talents, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Talents", PROPERTY_USAGE_DEFAULT);
    
    BIND_PROPERTY(custom_attack_area, OBJECT, PROPERTY_HINT_NODE_TYPE, "Area2D", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(attack_range, INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(is_long_range_attack, BOOL, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(attack_area_offset, FLOAT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(attack_area, INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(attack_area_height, INT, PROPERTY_HINT_RANGE, "120,99999", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(time_idle_after_attack, FLOAT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(health, INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(knockbacks, INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    
    BIND_PROPERTY(attack_hit_sfx, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "AudioStream", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(attack_sfx, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "AudioStream", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(before_death_sfx, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "AudioStream", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(die_sfx, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "AudioStream", PROPERTY_USAGE_DEFAULT);
    
    BIND_PROPERTY(relationship, Variant::INT, PROPERTY_HINT_ENUM, "FRIEND,FOE", PROPERTY_USAGE_NONE);
    BIND_PROPERTY(animation_speed_scale, FLOAT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);
    BIND_PROPERTY(rect_shape_query, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "PhysicsShapeQueryParameters2D", PROPERTY_USAGE_NONE);
    BIND_PROPERTY(face_direction, Variant::INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);
    BIND_PROPERTY(march_direction, Variant::INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);
    BIND_PROPERTY(velocity, Variant::VECTOR2, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);

    BIND_METHOD_GETTER(max_health);
    BIND_METHOD_GETTER(next_knockback_health);
    BIND_METHOD_GETTER(level);
    BIND_METHOD(is_boss);
    BIND_METHOD(get_attack_cooldown_timer);

    // Register methods
    BIND_METHOD(__z_index_modified_by_props);
    BIND_METHOD(__set_effect_canvas_group, "group");
    BIND_METHOD(get_actual_z_index);
    
    BIND_METHOD(get_character_animation_node);
    BIND_METHOD(get_hitbox_size);
    BIND_METHOD(get_hitbox_rect);
    BIND_METHOD(is_able_to_use_special_attack);
    BIND_METHOD(has_ability, "ability");
    BIND_METHOD(get_knockbacks_left);
    BIND_METHOD_GETTER(sprite_offset);
    BIND_METHOD(get_power_scale);
    
    // Use direct binding for methods with default arguments
    BIND_METHOD_6D1(setup, "global_pos", "level", "abilities", "special_attack_unlocked", "relationship", "is_boss", DEFVAL(false));
                        
    BIND_METHOD(update_rect_shape_query_for_idle);
    BIND_METHOD(update_rect_shape_query_for_attack);
    BIND_METHOD(update_rect_shape_query_for_air_unit_enemy_detection);
    BIND_METHOD(intersect_rect_shape);
    BIND_METHOD(intersect_rect_shape_get_closest);
    BIND_METHOD(is_rect_shape_colliding);
    BIND_METHOD(is_on_ground);
    BIND_METHOD(is_above_ground);
    BIND_METHOD(get_bottom_global_position);
    BIND_METHOD(get_effect_center_global_position);
    BIND_METHOD(get_danmaku_hitbox_position);
        
    BIND_METHOD(_is_past_knockback_health);
    
    // Fix knockback default parameter
    BIND_METHOD_1D1(knockback, "scale", DEFVAL(1.0f));
    
    BIND_METHOD(kill);
    BIND_METHOD(play_attack_sfx);
    BIND_METHOD(get_enemy_type);
    BIND_METHOD(get_enemy_group);
    BIND_METHOD(get_air_unit_enemy_group);
    BIND_METHOD(get_air_unit_group);
    BIND_METHOD(is_immune_to_attack);
    BIND_METHOD(set_mouse_area_detectable, "detectable");
    BIND_METHOD(set_immune_to_attack, "immune");
    BIND_METHOD(set_1hp_mode, "enable");
    BIND_METHOD(is_knockedback);
    BIND_METHOD(_handle_past_knockback_health);
    BIND_METHOD(move, "delta");
    BIND_METHOD(face_towards, "target");
    BIND_METHOD(reset);
    BIND_METHOD(take_damage, "amount", "attack", "attacker");
    // Register signals
    ADD_SIGNAL(MethodInfo("knockedback"));
    ADD_SIGNAL(MethodInfo("zero_health"));
    ADD_SIGNAL(MethodInfo("dead"));
}

uint32_t Character::get_base_speed() const { return base_speed; }
void Character::set_base_speed(uint32_t value) { base_speed = value; update_speed(); }
float Character::get_speed() const { return _speed; }

float Character::get_base_attack_cooldown() const { return base_attack_cooldown; }
void Character::set_base_attack_cooldown(float value) { base_attack_cooldown = value; update_attack_cooldown(); }
float Character::get_attack_cooldown() const { return _attack_cooldown; }

TypedArray<uint32_t> Character::get_base_attack_damages() const { return base_attack_damages; }
void Character::set_base_attack_damages(TypedArray<uint32_t> value) { base_attack_damages = value; update_attack_damage(); }
uint32_t Character::get_attack_damage(uint32_t index) const { return _attack_damages[index]; }
uint32_t Character::get_base_attack_damage(uint32_t index) const { return base_attack_damages[index]; }
float Character::get_defense_multiplier() const { return _defense_multiplier; }

float Character::get_animation_speed_scale() const { return animation_speed_scale; }
void Character::set_animation_speed_scale(float value) { 
    animation_speed_scale = value; 
    update_animation_speed();
}

uint32_t Character::get_attack_range() const { return attack_range; }
void Character::set_attack_range(uint32_t value) {
    attack_range = value;
    _update_rect_shape_query_attack_range();
}

// Get global position of the character's bottom
Vector2 Character::get_bottom_global_position() const {
    return Vector2(
        hitbox->get_global_position().x,
        hitbox->get_global_position().y + (get_hitbox_size().y * 0.5f)
    );
}

// Get center of character visually (include sprite offset)
Vector2 Character::get_effect_center_global_position() const {
    return hitbox->get_global_position() + sprite_offset;
}

DEFINE_GETTER_SETTER(Character, Character::Kind, kind);
DEFINE_GETTER_SETTER(Character, Character::Relationship, relationship);
DEFINE_GETTER_SETTER(Character, Character::UnitType, unit_type);
DEFINE_GETTER_SETTER(Character, Character::AttackType, attack_type);
DEFINE_GETTER_SETTER(Character,uint32_t, reward_money);
DEFINE_GETTER_SETTER(Character, bool, has_special_attack);
DEFINE_GETTER_SETTER(Character, Ref<SpecialAttackTurnChecker>, special_attack_turn_checker);
DEFINE_GETTER_SETTER(Character, Ref<Talents>, talents);
DEFINE_GETTER_SETTER(Character, Area2D*, custom_attack_area);
DEFINE_GETTER_SETTER(Character, bool, is_long_range_attack);
DEFINE_GETTER_SETTER(Character, uint32_t, attack_area);
DEFINE_GETTER_SETTER(Character, float, attack_area_offset);
DEFINE_GETTER_SETTER(Character, uint32_t, attack_area_height);
DEFINE_GETTER_SETTER(Character, float, time_idle_after_attack);
DEFINE_GETTER_SETTER(Character, uint32_t, health);
DEFINE_GETTER(Character, uint32_t, max_health);
DEFINE_GETTER_SETTER(Character, uint32_t, knockbacks);
DEFINE_GETTER(Character, uint32_t, next_knockback_health);
DEFINE_GETTER(Character, uint32_t, level);
DEFINE_GETTER_SETTER(Character, int8_t, march_direction);
DEFINE_GETTER(Character, int8_t, face_direction);
DEFINE_GETTER(Character, Vector2, sprite_offset);

DEFINE_GETTER(Character, String, character_id);
DEFINE_GETTER_SETTER(Character, Ref<AudioStream>, attack_hit_sfx);
DEFINE_GETTER_SETTER(Character, Ref<AudioStream>, attack_sfx);
DEFINE_GETTER_SETTER(Character, Ref<AudioStream>, before_death_sfx);
DEFINE_GETTER_SETTER(Character, Ref<AudioStream>, die_sfx);

// Node references
DEFINE_GETTER(Character, AnimationPlayer*, animation_player);
DEFINE_GETTER(Character, FSM*, fsm);
DEFINE_GETTER(Character, Node2D*, character_animation_node);
DEFINE_GETTER(Character, CooldownTimer*, attack_cooldown_timer);
DEFINE_GETTER(Character, CollisionShape2D*, hitbox);
DEFINE_GETTER(Character, Area2D*, danmaku_hitbox_area2d);
DEFINE_GETTER(Character, CollisionShape2D*, danmaku_hitbox);

// Other variables
DEFINE_GETTER_SETTER(Character, Ref<PhysicsShapeQueryParameters2D>, rect_shape_query);
DEFINE_GETTER_SETTER(Character, Vector2, velocity);

#undef DEFINE_GETTER_SETTER

bool Character::is_boss() const {
    return _is_boss;
}

void Character::set_face_direction(int8_t value) {
    int8_t old_direction = face_direction;
    face_direction = (value == 0) ? march_direction : Math::sign(value);
    
    if (!is_node_ready() || old_direction == face_direction) {
        return;
    }
    
    if (_face_anim_tween.is_valid()) {
        _face_anim_tween->kill();
    }
    
    _face_anim_tween = create_tween()->set_trans(Tween::TRANS_SINE);
    _face_anim_tween->tween_property(
        character_animation_node,
        "scale:x",
        (kind == Kind::DOG) ? float(face_direction) : float(-face_direction),
        FACING_ANIMATION_DURATION
    );
}

void Character::__set_effect_canvas_group(CanvasGroup* group) { __effect_canvas_group = group; }
uint32_t Character::get_actual_z_index() const { 
    return __effect_canvas_group == nullptr ? get_z_index() : __effect_canvas_group->get_z_index(); 
}

bool Character::is_able_to_use_special_attack() const { return has_special_attack && _special_attack_unlocked; }

void Character::set_1hp_mode(bool enable) { _1hp_mode = enable; }

void Character::update_speed()
{
    _speed = base_speed * status_effects.movement_speed_multiplier;
}

void Character::update_attack_cooldown()
{
    _attack_cooldown = base_attack_cooldown * status_effects.attack_cooldown_multiplier;
}

void Character::update_attack_damage()
{
    for (int i = 0; i < base_attack_damages.size(); i++)
    {
        float base_damage = base_attack_damages[i];
        _attack_damages[i] = round(base_damage * status_effects.attack_damage_multiplier);
    }
}

void Character::update_defense()
{
    _defense_multiplier = status_effects.defense_multiplier;
}

void Character::update_animation_speed()
{
    if (animation_player == nullptr) 
    {
        return; // node not ready yet
    }

    animation_player->set_speed_scale(animation_speed_scale * status_effects.attack_speed_multiplier);
    attack_cooldown_timer->set_speed_scale(status_effects.attack_speed_multiplier);
}

Vector2 Character::get_hitbox_size() const {
    return _hitbox_shape->get_size();
}

Rect2 Character::get_hitbox_rect() const {
    Rect2 hitbox_rect = Rect2(hitbox->get_global_position(), get_hitbox_size());
	hitbox_rect.position -= (hitbox_rect.size * 0.5f);
    return hitbox_rect;
}

Vector2 Character::get_danmaku_hitbox_position() const {
    return danmaku_hitbox_area2d->get_global_position() + danmaku_hitbox->get_position();
}
// attack: the attack object, can be null
// attacker: owner of the attack (usually is a character), can be null
void Character::take_damage(uint32_t amount, Variant attack, Node* attacker) {
    if (talents->shield != nullptr && !talents->shield->is_shield_broken()) {
        talents->shield->take_damage(amount, attack);
        return;
    }
    
    health -= amount * status_effects.defense_multiplier;
    
    if (_1hp_mode) {
        health = MAX(health, 1);
    }
    
    if (_is_past_knockback_health()) {
        _handle_past_knockback_health();
    }
}

void Character::_handle_past_knockback_health() {
    if (health > 0) {
        _update_next_knockback_health();
    }
    
    // instantly recharge attack on last knockback
    if (get_knockbacks_left() == 1) {
        attack_cooldown_timer->timeout_now();
    }
    
    if (health > 0) {
        knockback();
        return;
    }
    
    if (talents->survive.is_valid() && talents->survive->next()) {
        health = 1;
        knockback();
        return;
    }
    
    if (before_death_sfx.is_valid()) {
        _audio_player->call("play_in_battle_sfx", before_death_sfx);
    }
    
    emit_signal("zero_health");
    knockback();
}

uint32_t Character::get_knockbacks_left() const {
    if (health <= 0) {
        return 0;
    } else {
        // health + 1 because that's just how it works ok
        return MIN(1 + MIN(health - 1, max_health) / (max_health / knockbacks), knockbacks);
    }
}

void Character::_update_next_knockback_health() {
    while (_is_past_knockback_health()) {
        next_knockback_health = _calculate_next_knockback_health();
    }
}

uint32_t Character::_calculate_next_knockback_health() const {
    return MAX(0, next_knockback_health - MAX(1, (max_health / knockbacks)));
}

bool Character::_is_past_knockback_health() const {
    return health <= next_knockback_health;
}

void Character::knockback(float scale) {
    if (health <= 0) {
        // override scale when character is about to die
        scale = MAX(1.25f, scale);
    }
    
    Dictionary data = {};
    data["scale"] = scale;
    fsm->change_state("KnockbackState", data);
    emit_signal("knockedback");
}

void Character::kill() {
    emit_signal("knockedback");
    emit_signal("zero_health");
    fsm->change_state("DieState");
}

void Character::play_attack_sfx() {
    _audio_player->call("play_in_battle_sfx", attack_sfx, _audio_player->call("get_random_pitch_scale"));
}

void Character::_on_danmaku_bullet_entered(RID area_rid, Area2D* _area, int area_shape_index, int local_shape_index) {
    if (!_native_bullets->call("is_bullet_existing", area_rid, area_shape_index)) {
        return;
    }
    
    int bullet_id = _native_bullets->call("get_bullet_from_shape", area_rid, area_shape_index);
    
    Ref<RefCounted> controller = _native_bullets->call("get_bullet_property", bullet_id, "data");
    
    if (!(controller->get("destroy_on_hit") && controller->get("has_hit")) && (controller->get("kind").operator uint32_t()) != kind) {
        controller->emit_signal("body_enter", this);
        controller->set("has_hit", true);
        controller->set("hit_position", controller->get("position"));
    }
}

Character::Kind Character::get_enemy_type() const {
	return kind == Kind::DOG ? Kind::CAT : Kind::DOG;
}

StringName Character::get_enemy_group() const {
	return kind == Kind::DOG ? GROUP_CATS : GROUP_DOGS;
}

StringName Character::get_air_unit_enemy_group() const {
	return kind == Kind::DOG ? GROUP_AIR_UNIT_CATS : GROUP_AIR_UNIT_DOGS;
}

StringName Character::get_air_unit_group() const {
	return kind == Kind::DOG ? GROUP_AIR_UNIT_DOGS : GROUP_AIR_UNIT_CATS;
}

// This happens when character is knockedback
bool Character::is_immune_to_attack() {
    if (kind == Kind::DOG) {
        return !get_collision_layer_value(2);
    } else {
        return !get_collision_layer_value(3);
    }
}

void Character::set_mouse_area_detectable(bool detectable) {
    set_collision_layer_value(11, detectable);
}

void Character::set_immune_to_attack(bool immune) {
    bool value = !immune;
    danmaku_hitbox_area2d->set_deferred("monitoring", value);
    danmaku_hitbox_area2d->set_collision_layer_value(4, value);
    if (kind == Kind::DOG) {
        set_collision_layer_value(2, value);
        danmaku_hitbox_area2d->set_collision_layer_value(9, value);
        if (unit_type == UnitType::AIR) {
            set_collision_layer_value(8, value);
        }
    } else {
        set_collision_layer_value(3, value);
        danmaku_hitbox_area2d->set_collision_layer_value(10, value);
        if (unit_type == UnitType::AIR) {
            set_collision_layer_value(7, value);
        }
    }
}

bool Character::is_knockedback() const {
    return fsm->get_current_state_name() == StringName("KnockbackState");
}

void Character::__z_index_modified_by_props() {
    __z_index_already_set = true;
}

// returns the delta that is not used if hitting ground
float Character::move(float delta) {
    Vector2 _prev_bottom = get_bottom_global_position();
    
    set_global_position(get_global_position() + velocity * delta);
    
    if (is_above_ground()) {
        return 0.0f;
    } else {
        float impact_scale = abs(_prev_bottom.y) / (velocity.y * delta);
        float remainder_delta = delta * (1.0f - impact_scale);
        // revert back the position to the ground
        set_global_position(get_global_position() - velocity * remainder_delta);
        
        return remainder_delta;
    }
}

void Character::face_towards(Character* target) {
    face_direction = UtilityFunctions::signi(target->get_effect_center_global_position().x - get_effect_center_global_position().x);
}

bool Character::has_ability(StringName ability) const {
    return _abilities.has('all') || _abilities.has(ability);
}

void Character::setup(Vector2 global_position, int level, TypedArray<String> abilities, bool special_attack_unlocked, Relationship relationship, bool is_boss) {
    this->relationship = relationship;
    this->level = level;
    this->_is_boss = is_boss;
    set_global_position(global_position);
    
    _setup_power_scaling(get_power_scale());

    if (is_boss) {
        add_to_group("bosses");
    }
        
    _special_attack_unlocked = special_attack_unlocked;
    _abilities = abilities;
}

void Character::_setup_power_scaling(float power_scale) {
	health = UtilityFunctions::roundi(health * power_scale);
	for (size_t i = 0; i < base_attack_damages.size(); i++) {
        uint32_t base_attack_damage = base_attack_damages[i];
		base_attack_damages[i] = UtilityFunctions::roundi(base_attack_damage * power_scale);
	}

    GDVIRTUAL_CALL(_on_setup_power_scaling, power_scale);
}

float Character::get_power_scale() const {
    if (level < 10) {
        return 1.0f + ((level - 1) * (2.0f / 9.0f));
    } else {
        return 1.0f + level * 0.2f;
    }
}

void Character::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        _update_character();

        character_id = get_scene_file_path().get_file().split(".")[0];

        // set default reward money for dog kind if not set
        if (kind == Kind::DOG) {
	        Ref<FileAccess> file = FileAccess::open("res://resources/game_data/dogs.json", FileAccess::READ);
            Dictionary dog_info = JSON::parse_string(file->get_as_text());
            
            if (dog_info.has(character_id)) {
                reward_money = UtilityFunctions::roundi(dog_info[character_id]["spawn_price"] * 0.5f);
            }
            else {
                WARN_PRINT("No spawn price found for character: " + character_id);
            }

            file->close();
        }

        return;
    }

    _audio_player = get_tree()->get_root()->get_node<Node>("AudioPlayer");
    _native_bullets = get_tree()->get_root()->get_node<Node>("Bullets");

    hitbox = get_node<CollisionShape2D>("Hitbox");
    animation_player = get_node<AnimationPlayer>("AnimationPlayer");
    fsm = get_node<FSM>("FSM");

    character_animation_node = get_node<Node2D>("CharacterAnimation");
    // Set animation node as unique name in owner for shader mouse hover
    character_animation_node->set_unique_name_in_owner(true);

    danmaku_hitbox_area2d = get_node<Area2D>("DanmakuHitbox");
    danmaku_hitbox = get_node<CollisionShape2D>("DanmakuHitbox/CollisionShape2D");

    ERR_FAIL_COND_MSG(hitbox->get_position() != Vector2(), "hitbox must be centered");

    ERR_FAIL_COND_MSG(character_id.is_empty(), "character_id is not set");

    ERR_FAIL_COND_MSG(talents.is_null(), 
                      "talents is not set for character: " + get_character_id());

    ERR_FAIL_COND_MSG(talents->is_local_to_scene(), 
                      "talents must be local to scene, character: " + get_character_id());

    ERR_FAIL_COND_MSG(fsm == nullptr, 
                      "fsm is not set for character: " + get_character_id());

    ERR_FAIL_COND_MSG(has_special_attack && special_attack_turn_checker.is_null(), 
                      "special_attack_turn_checker is not set for character: " + get_character_id());

    ERR_FAIL_COND_MSG(has_special_attack && special_attack_turn_checker.is_valid() && special_attack_turn_checker->is_local_to_scene(), 
                      "special_attack_turn_checker must be local to scene, character: " + get_character_id());
    
    // Verify special attack state exists if character has special attack
    ERR_FAIL_COND_MSG(has_special_attack && fsm->get_state("SpecialAttackState") == nullptr, 
                      "Character has special attack but does not implement 'SpecialAttack' state");

    ERR_FAIL_COND_MSG(base_attack_damages.size() == 0, 
                      "Attack damages is empty for character: " + get_character_id());

    // Verify hitbox is a rectangle shape
    Ref<RectangleShape2D> hitbox_shape = hitbox->get_shape();
    ERR_FAIL_COND_MSG(hitbox_shape.is_null(), 
                      "ERROR: Character hitbox must always be a rectangle");
    
    // Verify animation node exists
    ERR_FAIL_COND_MSG(character_animation_node == nullptr, 
                      "ERROR: CharacterAnimation does not have accessed as unique name");
    
    // Set default boss death SFX if not provided
    if (_is_boss && before_death_sfx.is_null()) {
        before_death_sfx = DEFAULT_BOSS_DIE_SFX;
    }

    if (attack_hit_sfx.is_null()) {
        attack_hit_sfx = DEFAULT_ATTACK_HIT_SFX;
    }

    if (die_sfx.is_null()) {
        die_sfx = DEFAULT_DIE_SFX;
    }

    // setup default facing and marching direction
    march_direction = kind == Kind::DOG ? 1 : -1;
    face_direction = march_direction;
    
    // Setup shape query and update character
    _setup_rect_shape_query();
    
    _update_character();

    // Flip character if moving in opposite direction
    bool is_friend_shaped = relationship == Relationship::FRIEND;
    if ((is_friend_shaped && kind == Kind::CAT) || (!is_friend_shaped && kind == Kind::DOG)) {
        character_animation_node->set_scale(Vector2(-1.0f, 1.0f));
        Vector2 hitbox_position = hitbox->get_position();
        hitbox->set_position(Vector2(-hitbox_position.x, hitbox_position.y));
        hitbox->set_rotation(-hitbox->get_rotation());
        
        Vector2 danmaku_hitbox_area_position = danmaku_hitbox_area2d->get_position();
        danmaku_hitbox_area2d->set_position(Vector2(-danmaku_hitbox_area_position.x, danmaku_hitbox_area_position.y));
        danmaku_hitbox_area2d->set_rotation(-danmaku_hitbox_area2d->get_rotation());
        
        TypedArray<CollisionShape2D> children = danmaku_hitbox_area2d->get_children();
        for (int i = 0; i < children.size(); i++) {
            CollisionShape2D* danmaku_hitbox = Object::cast_to<CollisionShape2D>(children[i]);
            if (danmaku_hitbox) {
                Vector2 danmaku_hitbox_position = danmaku_hitbox->get_position();
                danmaku_hitbox->set_position(Vector2(-danmaku_hitbox_position.x, danmaku_hitbox_position.y));
                danmaku_hitbox->set_rotation(-danmaku_hitbox->get_rotation());
            }
        }
        
        if (custom_attack_area != nullptr) {
            Vector2 custom_attack_area_position = custom_attack_area->get_position();
            custom_attack_area->set_position(Vector2(-custom_attack_area_position.x, custom_attack_area_position.y));
            custom_attack_area->set_rotation(-custom_attack_area->get_rotation());
        }
    }

    // Set danmaku hitbox shape to match collision shape if not specified
    Ref<RectangleShape2D> danmaku_rect = Object::cast_to<RectangleShape2D>(danmaku_hitbox->get_shape().ptr());
    if (danmaku_rect.is_valid() && danmaku_rect->get_size() == Vector2()) {
        danmaku_hitbox->set_shape(hitbox->get_shape());
        danmaku_hitbox->set_position(hitbox->get_position());
    }
    
    // Adjust global position
    set_global_position(Vector2(get_global_position().x, 
                               get_global_position().y + (get_global_position().y - get_bottom_global_position().y)));
    
    // Set animation speed
    animation_player->set_speed_scale(animation_speed_scale);

    attack_cooldown_timer->set_wait_time(_attack_cooldown);
    
    // Setup talents
    talents->setup(this);

    // Apply sprite offset
    _apply_sprite_offset();
    
    // Initialize special attack turn checker if needed
    if (has_special_attack) {
        bool _;
        GDVIRTUAL_CALL_PTR(special_attack_turn_checker, next, _);
    }

    GDVIRTUAL_CALL(_on_ready);
}

// Add random sprite offset for better visibility when characters are stacked on each other
// For ground unit, z-index will be around 0 - 40
// For air unit, the z-index will be around 50 - 90
// 91 if "closest to camera" mode enabled
// The range from 41 - 49 will be for foreground and stuff
void Character::_apply_sprite_offset() {
    // If z_index is not modified by props, then assign random z_index
    if (!__z_index_already_set) {
        int rand_y = _is_boss ? 0 : UtilityFunctions::randi_range(-20, 20);
        int new_z_index = rand_y + 20;

        if (unit_type == UnitType::GROUND) {
            sprite_offset = Vector2(0, rand_y * 2.0f);
            // Render stuff correctly
            set_z_index(new_z_index);
        }
        // Let air unit type be on top of ground unit type and not apply sprite offset
        else if (unit_type == UnitType::AIR) {
            set_z_index(new_z_index + 50);
            sprite_offset = Vector2();
        }
    } else {
        sprite_offset = unit_type == UnitType::AIR ? Vector2() : Vector2(0, get_z_index() - 20);
    }

    character_animation_node->set_position(character_animation_node->get_position() + sprite_offset);
}

void Character::_setup_rect_shape_query() {
    rect_shape_query = Ref<PhysicsShapeQueryParameters2D>(memnew(PhysicsShapeQueryParameters2D));
    rect_shape_query->set_collide_with_areas(true);
    rect_shape_query->set_collide_with_bodies(false);
    rect_shape_query->set_shape_rid(PhysicsServer2D::get_singleton()->rectangle_shape_create());
}

void Character::_update_character() {
    max_health = health;
    
    // this is kinda dumb i couldn't think if a way for this to update correctly while reusing the same logic
    next_knockback_health = max_health;
    next_knockback_health = _calculate_next_knockback_health();

    // for non long range attack, attack area will also includes half of the hitbox x length
    // (character will attack enemies that are inside of them)
    if (!is_long_range_attack) {
        attack_area = attack_range + get_hitbox_size().x * 0.5;
        attack_area_offset = -attack_area;
    }
        
    if (unit_type == UnitType::AIR) {
        add_to_group(GROUP_AIR_UNITS);
        add_to_group(kind == Kind::DOG ? GROUP_AIR_UNIT_DOGS : GROUP_AIR_UNIT_CATS);
        attack_type = AttackType::ATTACK_UNIQUE; // air unit will always attack unique (using bullets or whatever else)
    } else {
        remove_from_group(GROUP_AIR_UNITS);
        remove_from_group(GROUP_AIR_UNIT_DOGS);
        remove_from_group(GROUP_AIR_UNIT_CATS);
    }
            
    if (kind == Kind::DOG) {
        remove_from_group(GROUP_CATS);
        add_to_group(GROUP_DOGS);
        
        rect_shape_query->set_collision_mask(DOG_ENEMY_DETECTION_MASK);

        if (custom_attack_area != nullptr) {
            custom_attack_area->set_collision_mask(DOG_CUSTOM_AREA_COLLISION_MASK);
        }
        
        set_collision_mask(DOG_COLLISION_MASK);
        set_collision_layer(DOG_COLLISION_LAYER);
        danmaku_hitbox_area2d->set_collision_layer(DOG_DANMAKU_HITBOX_COLLISION_LAYER);
        danmaku_hitbox_area2d->set_collision_mask(0b1000);
    } else {
        remove_from_group(GROUP_DOGS);
        add_to_group(GROUP_CATS);
    
        rect_shape_query->set_collision_mask(CAT_ENEMY_DETECTION_MASK);
        if (custom_attack_area != nullptr) {
            custom_attack_area->set_collision_mask(CAT_CUSTOM_AREA_COLLISION_MASK);
        }
            
        set_collision_mask(CAT_COLLISION_MASK);
        set_collision_layer(CAT_COLLISION_LAYER);
        danmaku_hitbox_area2d->set_collision_layer(CAT_DANMAKU_HITBOX_COLLISION_LAYER);
        danmaku_hitbox_area2d->set_collision_mask(0b1000);
    }
    
    if (unit_type == UnitType::AIR) {
        // air unit will not collide with dog or cat tower 
        set_collision_mask(get_collision_mask() & ~(0b110000));
        // and will not detect dog or cat tower
        rect_shape_query->set_collision_mask(get_collision_mask() & ~(0b110000));
        set_collision_layer(get_collision_layer() | (kind == Kind::DOG ? AIR_UNIT_DOG_COLLISION_LAYER : AIR_UNIT_CAT_COLLISION_LAYER));
    }

    if (!Engine::get_singleton()->is_editor_hint()) {
        _update_rect_shape_query_attack_range();

    }
}

void Character::_update_rect_shape_query_attack_range() {
    if (!is_node_ready()) {
        return;
    }

    if (unit_type == UnitType::GROUND) {
        float padding = is_long_range_attack ? get_hitbox_size().x * 0.5f : 0.0f;
        PhysicsServer2D::get_singleton()->shape_set_data(
            rect_shape_query->get_shape_rid(),
            Vector2(attack_area + padding, attack_area_height) * 0.5f + Vector2(1, 1) // vector one to allow hitting stuff just within range
        );
    } else {
        // I'm too lazy to write battlefield code in gdextension so let's just call it dynamically
        Rect2 stage_rect = Global::get_singleton()->get_tree()->get_current_scene()->call("get_battle_area_rect");
        PhysicsServer2D::get_singleton()->shape_set_data(
            rect_shape_query->get_shape_rid(),
            Vector2(attack_range, stage_rect.size.y * 0.5f)
        );
    }
}

void Character::update_rect_shape_query_for_idle() {
    Vector2 rect_half_extends = Vector2(
        (get_hitbox_size().x * 0.5f + attack_range) * 0.5f,
        attack_area_height * 0.5f
    );

    PhysicsServer2D::get_singleton()->shape_set_data(rect_shape_query->get_shape_rid(), rect_half_extends);

    Vector2 shape_pos = get_global_position();
    shape_pos.x += rect_half_extends.x * march_direction;
    shape_pos.y = get_bottom_global_position().y - rect_half_extends.y;
    rect_shape_query->set_transform(Transform2D(0, shape_pos));
}

void Character::update_rect_shape_query_for_attack() {
    Vector2 pos = get_global_position();
    pos.x += ((get_hitbox_size().x + attack_area) * 0.5f + attack_range + attack_area_offset) * march_direction;
    pos.y = get_bottom_global_position().y - attack_area_height * 0.5f;
    
    rect_shape_query->set_transform(Transform2D(0, pos));
}

void Character::update_rect_shape_query_for_air_unit_enemy_detection() {
    Vector2 pos = get_global_position();

    // Get the battlefield rect from the current scene
    Rect2 stage_rect = Global::get_singleton()->get_tree()->get_current_scene()->call("get_battle_area_rect");
    pos.y = stage_rect.position.y + stage_rect.size.y * 0.5;
    
    rect_shape_query->set_transform(Transform2D(0, pos));
}

TypedArray<Dictionary> Character::intersect_rect_shape() {
    PhysicsDirectSpaceState2D* space_state = get_world_2d()->get_direct_space_state();
    return space_state->intersect_shape(rect_shape_query, 100);
}

Dictionary Character::intersect_rect_shape_get_closest() {
    TypedArray<Dictionary> results = intersect_rect_shape();
    Vector2 character_pos = get_global_position();
    Dictionary collide_data;
    float min_d = INFINITY;
    
    for (int i = 0; i < results.size(); i++) {
        Dictionary result = results[i];
        Character* collider = Object::cast_to<Character>(result["collider"]);
        if (collider != nullptr) {
            Vector2 enemy_pos = collider->get_hitbox_rect().get_center();
            float d = enemy_pos.distance_squared_to(character_pos);
            if (min_d > d) {
                min_d = d;
                collide_data = result;
            }
        }
    }
    
    return collide_data;
}
bool Character::is_rect_shape_colliding() {
    return !intersect_rect_shape().is_empty();
}

bool Character::is_on_ground() const {
    return get_global_position().y + (get_hitbox_size().y * 0.5f) >= 0.0f;
}

bool Character::is_above_ground() const {
    return !is_on_ground();
}

void Character::add_status_effect(Ref<StatusEffect> status_effect)
{
    status_effects.add_status_effect(status_effect);
    _update_character_stats();
}

void Character::remove_status_effect(Ref<StatusEffect> status_effect)
{
    status_effects.remove_status_effect(status_effect);
    _update_character_stats();
}

void Character::_update_character_stats()
{
    update_attack_damage();
    update_attack_cooldown();
    update_speed();
    update_defense();
    update_animation_speed();
}

void Character::_validate_property(PropertyInfo& property) const {
    String property_name = property.name;
    if (property_name == "custom_attack_area" && attack_type != AttackType::ATTACK_UNIQUE) {
        property.usage = PROPERTY_USAGE_NO_EDITOR;
    }
    else if ((property_name == "is_long_range_attack" || property_name == "attack_area" || property_name == "attack_area_offset") && attack_type != AttackType::ATTACK_AREA) {
        property.usage = PROPERTY_USAGE_NO_EDITOR;
    }
    else if ((property_name == "attack_area" || property_name == "attack_area_offset") && !is_long_range_attack) {
        property.usage = PROPERTY_USAGE_READ_ONLY;
    }
    else if (property_name == "attack_area_height" && attack_type == AttackType::ATTACK_UNIQUE) {
        property.usage = PROPERTY_USAGE_NO_EDITOR;
    }
    else if ((property_name == "custom_attack_area" || property_name == "attack_type") && unit_type == UnitType::AIR) {
        property.usage = PROPERTY_USAGE_NO_EDITOR;
    }
    else if (property_name == "special_attack_turn_checker" && !has_special_attack) {
        property.usage = PROPERTY_USAGE_NO_EDITOR;
    }
    else if (property_name == "reward_money" && kind == Kind::DOG) {
        property.usage = PROPERTY_USAGE_NO_EDITOR;
    }
}





