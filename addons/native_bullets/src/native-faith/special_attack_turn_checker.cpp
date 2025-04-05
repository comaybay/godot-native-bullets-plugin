#include "special_attack_turn_checker.h"

void SpecialAttackTurnChecker::_bind_methods() {
    GDVIRTUAL_BIND(next);
    GDVIRTUAL_BIND(is_true);

    ADD_SIGNAL(MethodInfo("result_changed"));
}

void SpecialAttackTurnChecker::_notification(int p_what)
{
    if (p_what == NOTIFICATION_POSTINITIALIZE) {
        if (!is_local_to_scene()) {
            ERR_PRINT("SpecialAttackTurnChecker must be local to scene");
        }
    }
}
