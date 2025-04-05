#include "status_effect.h"

void StatusEffect::_bind_methods() {
    BIND_PROPERTY(id, STRING_NAME, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(attack_damage_strength, FLOAT, PROPERTY_HINT_RANGE, "1,1000", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(movement_speed_strength, FLOAT, PROPERTY_HINT_RANGE, "1,1000", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(attack_cooldown_strength, FLOAT, PROPERTY_HINT_RANGE, "1,1000", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(attack_speed_strength, FLOAT, PROPERTY_HINT_RANGE, "1,1000", PROPERTY_USAGE_DEFAULT);
    BIND_PROPERTY(defense_strength, FLOAT, PROPERTY_HINT_RANGE, "1,1000", PROPERTY_USAGE_DEFAULT);
}

DEFINE_GETTER_SETTER(StatusEffect, StringName, id);
DEFINE_GETTER_SETTER(StatusEffect, float, attack_damage_strength);
DEFINE_GETTER_SETTER(StatusEffect, float, movement_speed_strength);
DEFINE_GETTER_SETTER(StatusEffect, float, attack_cooldown_strength);
DEFINE_GETTER_SETTER(StatusEffect, float, attack_speed_strength);
DEFINE_GETTER_SETTER(StatusEffect, float, defense_strength);


