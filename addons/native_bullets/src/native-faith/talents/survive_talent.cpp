#include "survive_talent.h"
#include <godot_cpp/core/class_db.hpp>
#include "../game_libs/global.h"
#include "../character.h"

void SurviveTalent::_bind_methods() {
    ADD_SIGNAL(MethodInfo("nexted"));
    
    ClassDB::bind_method(D_METHOD("survived"), &SurviveTalent::survived);
    ClassDB::bind_method(D_METHOD("will_survived"), &SurviveTalent::will_survived);
    ClassDB::bind_method(D_METHOD("next"), &SurviveTalent::next);
    
    ClassDB::bind_method(D_METHOD("set_survive_chance", "value"), &SurviveTalent::set_survive_chance);
    ClassDB::bind_method(D_METHOD("get_survive_chance"), &SurviveTalent::get_survive_chance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "survive_chance"), "set_survive_chance", "get_survive_chance");
}

void SurviveTalent::set_survive_chance(float value) {
    survive_chance = value;
    _will_survive = UtilityFunctions::randf() < value;
    _survived = _will_survive;
}

float SurviveTalent::get_survive_chance() const { 
    return survive_chance; 
}

bool SurviveTalent::survived() const {
    return _survived;
}

bool SurviveTalent::will_survived() const {
    return _will_survive;
}

bool SurviveTalent::next() {
    _survived = _will_survive;
    _will_survive = UtilityFunctions::randf() < survive_chance;
    emit_signal("nexted");
    return _survived;
}
