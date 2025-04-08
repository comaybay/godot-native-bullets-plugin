#include "talents.h"
#include "../character.h"
#include "shield_talent.h"
#include "survive_talent.h"

Talents::Talents() {
}

Talents::~Talents() {
}

void Talents::_bind_methods() {
    ClassDB::bind_method(D_METHOD("setup", "character"), &Talents::setup);
    
    ClassDB::bind_method(D_METHOD("get_shield"), &Talents::get_shield);
    ClassDB::bind_method(D_METHOD("set_shield", "shield"), &Talents::set_shield);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shield", PROPERTY_HINT_RESOURCE_TYPE, "ShieldTalent"), "set_shield", "get_shield");
    
    ClassDB::bind_method(D_METHOD("get_survive"), &Talents::get_survive);
    ClassDB::bind_method(D_METHOD("set_survive", "survive"), &Talents::set_survive);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "survive", PROPERTY_HINT_RESOURCE_TYPE, "SurviveTalent"), "set_survive", "get_survive");
    
    ClassDB::bind_method(D_METHOD("get_immune_to_knockback"), &Talents::get_immune_to_knockback);
    ClassDB::bind_method(D_METHOD("set_immune_to_knockback", "immune"), &Talents::set_immune_to_knockback);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "immune_to_knockback"), "set_immune_to_knockback", "get_immune_to_knockback");
    
    ClassDB::bind_method(D_METHOD("get_immune_to_slow"), &Talents::get_immune_to_slow);
    ClassDB::bind_method(D_METHOD("set_immune_to_slow", "immune"), &Talents::set_immune_to_slow);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "immune_to_slow"), "set_immune_to_slow", "get_immune_to_slow");
}

void Talents::setup(Character* character) {
    bool all_is_local_to_scene = true;
    
    if (shield.is_valid() && !shield->is_local_to_scene()) {
        all_is_local_to_scene = false;
    }
    
    if (survive.is_valid() && !survive->is_local_to_scene()) {
        all_is_local_to_scene = false;
    }
    
    ERR_FAIL_COND_MSG(!all_is_local_to_scene, "ERROR: one of the talents is not local to scene");
    
    if (shield.is_valid()) {
        shield->setup(character);
    }
}

DEFINE_GETTER_SETTER(Talents, Ref<ShieldTalent>, shield)
DEFINE_GETTER_SETTER(Talents, Ref<SurviveTalent>, survive)
DEFINE_GETTER_SETTER(Talents, bool, immune_to_knockback)
DEFINE_GETTER_SETTER(Talents, bool, immune_to_slow)
