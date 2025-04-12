#ifndef TALENTS_H
#define TALENTS_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>
#include "../macros.h"
#include "shield_talent.h"
#include "survive_talent.h"

class Character;

using namespace godot;

class Talents : public Resource {
    GDCLASS(Talents, Resource);

protected:
    static void _bind_methods();

public:
    Talents();
    ~Talents();
    void setup(Character* character);

    // Getters and setters
    PROPERTY(Ref<ShieldTalent>, shield, nullptr);
    PROPERTY(Ref<SurviveTalent>, survive, nullptr);
    PROPERTY(bool, immune_to_knockback, false);
    PROPERTY(bool, immune_to_slow, false);
};

#endif // TALENTS_H
