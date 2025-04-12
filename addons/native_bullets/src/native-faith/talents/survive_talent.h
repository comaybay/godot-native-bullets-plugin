#ifndef SURVIVE_TALENT_H
#define SURVIVE_TALENT_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>
#include "../game_libs/global.h"

class Character;

using namespace godot;

class SurviveTalent : public Resource {
    GDCLASS(SurviveTalent, Resource);

private:
    float survive_chance = 0.0f;
    bool _will_survive = false;
    bool _survived = false;

protected:
    static void _bind_methods();

public:
    void set_survive_chance(float value);
    float get_survive_chance() const;
    
    bool survived() const;
    bool will_survived() const;
    bool next();
};

#endif // SURVIVE_TALENT_H
