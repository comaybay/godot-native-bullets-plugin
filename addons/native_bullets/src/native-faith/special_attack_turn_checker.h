#ifndef SPECIAL_ATTACK_TURN_CHECKER_H
#define SPECIAL_ATTACK_TURN_CHECKER_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class Character;

class SpecialAttackTurnChecker : public Resource
{
    GDCLASS(SpecialAttackTurnChecker, Resource);

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    GDVIRTUAL0R_REQUIRED(bool, next);
    GDVIRTUAL0R_REQUIRED(bool, is_true);
};

#endif 
