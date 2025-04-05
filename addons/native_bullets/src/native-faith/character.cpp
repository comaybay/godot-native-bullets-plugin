#include "character.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

Character::Character() {
    if (_default_boss_die_sfx.is_null()) {
        _default_boss_die_sfx = ResourceLoader::get_singleton()->load("res://assets/sfx/DEFAULT_BOSS_DIE_SFX.wav");
    }
}

Character::~Character() {
}

void Character::_ready() {
    _super_ready();
}

void Character::_bind_methods() {
    GDVIRTUAL_BIND(_on_setup_power_scaling)
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

    BIND_PROPERTY(base_speed, INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(base_attack_cooldown, FLOAT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(base_attack_damages, ARRAY, PROPERTY_HINT_ARRAY_TYPE, "int", PROPERTY_USAGE_DEFAULT);

    BIND_METHOD(get_speed);
    BIND_METHOD(get_attack_cooldown);
    BIND_METHOD(get_defense_multiplier);
    BIND_METHOD_1D1(get_attack_damage, "index", DEFVAL(0));
    BIND_PROPERTY(animation_speed_scale, FLOAT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);


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
    BIND_PROPERTY(time_wait_after_attack, FLOAT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(health, INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);

    
    BIND_PROPERTY(knockbacks, INT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    
    BIND_PROPERTY(attack_hit_sfx, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "AudioStream", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(attack_sfx, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "AudioStream", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(before_death_sfx, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "AudioStream", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(die_sfx, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "AudioStream", PROPERTY_USAGE_DEFAULT);
    
    BIND_METHOD(set_march_direction, "value");
    BIND_METHOD(get_march_direction);
    
    BIND_METHOD(set_face_direction, "value");
    BIND_METHOD(get_face_direction);
    
    BIND_PROPERTY_GETONLY(attack_cooldown_timer, Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "CooldownTimer", PROPERTY_USAGE_NONE);

    // Register methods
    BIND_METHOD(__z_index_modified_by_props);
    BIND_METHOD(get_animation_node);
    BIND_METHOD(get_actual_z_index);
    BIND_METHOD(get_collision_rect);
    BIND_METHOD(is_able_to_use_special_attack);
    BIND_METHOD(has_ability, "ability");
    BIND_METHOD(get_knockbacks_left);
    BIND_METHOD(get_sprite_offset);
    BIND_METHOD(get_power_scale);
    
    // Use direct binding for methods with default arguments
    BIND_METHOD_6D1(setup, "global_pos", "level", "abilities", "special_attack_unlocked", "relationship", "is_boss", DEFVAL(false));
                        
    BIND_METHOD(_update_charcter_collision_shapes);
    BIND_METHOD(_setup_rect_shape_query);
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
    BIND_METHOD(get_hitbox_rect);
    BIND_METHOD(get_danmaku_hitbox_position);
        
    BIND_METHOD(_apply_time_related_status_effects);
    BIND_METHOD(is_past_knockback_health);
    BIND_METHOD(get_next_knockback_health);
    
    // Fix knockback default parameter
    BIND_METHOD_1D1(knockback, "scale", DEFVAL(1.0f));
    
    BIND_METHOD(kill);
    BIND_METHOD(play_attack_sfx);
    BIND_METHOD(_on_danmaku_bullet_entered, "area_rid", "area", "area_shape_index", "local_shape_index");
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
    BIND_METHOD(_apply_sprite_offset);
    BIND_METHOD(move, "delta");
    BIND_METHOD(face_towards, "target");
    BIND_METHOD(reset);
    BIND_METHOD(take_damage, "amount", "attack", "attacker");
    // Register signals
    ADD_SIGNAL(MethodInfo("knockedback"));
    ADD_SIGNAL(MethodInfo("zero_health"));
    ADD_SIGNAL(MethodInfo("dead"));

    BIND_METHOD(_super_ready);
}

uint32_t Character::get_base_speed() const { return base_speed; }
void Character::set_base_speed(uint32_t value) { base_speed = value; update_speed(); }

float Character::get_base_attack_cooldown() const { return base_attack_cooldown; }
void Character::set_base_attack_cooldown(float value) { base_attack_cooldown = value; update_attack_cooldown(); }

TypedArray<uint32_t> Character::get_base_attack_damages() const { return base_attack_damages; }
void Character::set_base_attack_damages(TypedArray<uint32_t> value) { base_attack_damages = value; update_attack_damage(); }
uint32_t Character::get_attack_damage(uint32_t index) const { return _attack_damages[index]; }

float Character::get_animation_speed_scale() const { return animation_speed_scale; }
void Character::set_animation_speed_scale(float value) { 
    animation_speed_scale = value; 
    update_animation_speed();
}

DEFINE_GETTER_SETTER(Character,uint32_t, reward_money);
DEFINE_GETTER_SETTER(Character, Character::Kind, kind);
DEFINE_GETTER_SETTER(Character, Character::Relationship, relationship);
DEFINE_GETTER_SETTER(Character, Character::UnitType, unit_type);
DEFINE_GETTER_SETTER(Character, Character::AttackType, attack_type);
DEFINE_GETTER_SETTER(Character, bool, has_special_attack);
DEFINE_GETTER_SETTER(Character, Ref<SpecialAttackTurnChecker>, special_attack_turn_checker);
DEFINE_GETTER_SETTER(Character, Ref<Talents>, talents);
DEFINE_GETTER_SETTER(Character, Area2D*, custom_attack_area);
DEFINE_GETTER_SETTER(Character, uint32_t, attack_range);
DEFINE_GETTER_SETTER(Character, bool, is_long_range_attack);
DEFINE_GETTER_SETTER(Character, uint32_t, attack_area);
DEFINE_GETTER_SETTER(Character, float, attack_area_offset);
DEFINE_GETTER_SETTER(Character, uint32_t, attack_area_height);
DEFINE_GETTER_SETTER(Character, float, time_wait_after_attack);
DEFINE_GETTER_SETTER(Character, uint32_t, health);
DEFINE_GETTER_SETTER(Character, uint32_t, knockbacks);
DEFINE_GETTER_SETTER(Character, int8_t, march_direction);
DEFINE_GETTER_SETTER(Character, int8_t, face_direction);
DEFINE_GETTER(Character, uint32_t, max_health);
DEFINE_GETTER(Character, uint32_t, next_knockback_health);
DEFINE_GETTER(Character, uint32_t, level);
DEFINE_GETTER(Character, bool, is_boss);
// DEFINE_GETTER(Character, StatusEffects*, status_effects);
void Character::__set_effect_canvas_group(CanvasGroup* group) { __effect_canvas_group = group; }
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
        _attack_damages[i] = base_damage * status_effects.attack_damage_multiplier;
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

DEFINE_GETTER(Character, String, character_id);
DEFINE_GETTER_SETTER(Character, Ref<AudioStream>, attack_hit_sfx);
DEFINE_GETTER_SETTER(Character, Ref<AudioStream>, attack_sfx);
DEFINE_GETTER_SETTER(Character, Ref<AudioStream>, before_death_sfx);
DEFINE_GETTER_SETTER(Character, Ref<AudioStream>, die_sfx);
void Character::__z_index_modified_by_props() { __z_index_already_set = true; }

// Node references
DEFINE_GETTER(Character, AnimationPlayer*, animation_player);
// DEFINE_GETTER(Character, FSM*, fsm);
DEFINE_GETTER(Character, Node2D*, animation_node);
DEFINE_GETTER(Character, CooldownTimer*, attack_cooldown_timer);
DEFINE_GETTER(Character, CollisionShape2D*, hitbox);
DEFINE_GETTER(Character, Area2D*, danmaku_hitbox_area2d);
DEFINE_GETTER(Character, CollisionShape2D*, danmaku_hitbox);

// Other variables
DEFINE_GETTER_SETTER(Character, Ref<PhysicsShapeQueryParameters2D>, rect_shape_query);
DEFINE_GETTER_SETTER(Character, Vector2, velocity);
DEFINE_GETTER(Character, Vector2, sprite_offset);

#undef DEFINE_GETTER_SETTER

void Character::_super_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    ERR_FAIL_COND_MSG(base_attack_damages.size() == 0, 
                      "Attack damages is empty for character: " + get_character_id());

    // Verify hitbox is a rectangle shape
    ERR_FAIL_COND_MSG(Object::cast_to<RectangleShape2D>(hitbox->get_shape().ptr()) == nullptr, 
                      "ERROR: Character hitbox must always be a rectangle");
    
    // Verify animation node exists
    ERR_FAIL_COND_MSG(animation_node == nullptr, 
                      "ERROR: CharacterAnimation does not have accessed as unique name");
    
    // Verify talents is local to scene
    ERR_FAIL_COND_MSG(!talents->is_local_to_scene(), 
                      "talents must be local to scene");
    
    // Verify special attack state exists if character has special attack
    ERR_FAIL_COND_MSG(has_special_attack && fsm->get_state("SpecialAttackState") == nullptr, 
                      "Character has special attack but does not implement 'SpecialAttack' state");
    
    // Setup status effects
    status_effects.setup(this);
    
    // Set animation node as unique name in owner for shader mouse hover
    animation_node->set_unique_name_in_owner(true);
    
    // Apply sprite offset
    _apply_sprite_offset();
    
    // Set default boss death SFX if not provided
    if (is_boss && before_death_sfx.is_null()) {
        before_death_sfx = _default_boss_die_sfx;
    }
    
    // Setup shape query and update character
    _setup_rect_shape_query();
    _update_charcter_collision_shapes();
    
    // Flip character if moving in opposite direction
    bool is_friend_shaped = relationship == Relationship::FRIEND;
    if ((is_friend_shaped && kind == Kind::CAT) || (!is_friend_shaped && kind == Kind::DOG)) {
        animation_node->set_scale(Vector2(-1.0f, 1.0f));
        Vector2 hitbox_position = hitbox->get_position();
        hitbox->set_position(Vector2(-hitbox_position.x, hitbox_position.y));
    }
    
    // Set animation speed
    animation_player->set_speed_scale(animation_speed_scale);
    
    // Set danmaku hitbox shape to match collision shape if not specified
    Ref<RectangleShape2D> danmaku_rect = Object::cast_to<RectangleShape2D>(danmaku_hitbox->get_shape().ptr());
    if (danmaku_rect.is_valid() && danmaku_rect->get_size() == Vector2()) {
        danmaku_hitbox->set_shape(hitbox->get_shape());
        danmaku_hitbox->set_position(hitbox->get_position());
    }
    
    // Adjust global position
    set_global_position(Vector2(get_global_position().x, 
                               get_global_position().y + (get_global_position().y - get_bottom_global_position().y)));
    
    // Setup talents
    talents->setup(this);
    
    // Initialize special attack turn checker if needed
    if (has_special_attack) {
        bool _;
        GDVIRTUAL_CALL_PTR(special_attack_turn_checker, next, _);
    }
}
