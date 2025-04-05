#include "fsm_state.h"

void FSMState::_bind_methods()
{
  GDVIRTUAL_BIND(setup);
  GDVIRTUAL_BIND(enter);
  GDVIRTUAL_BIND(exit);
  GDVIRTUAL_BIND(update, "delta");
  GDVIRTUAL_BIND(physics_update, "delta");
  GDVIRTUAL_BIND(input, "event");

  BIND_PROPERTY(state_name, Variant::STRING_NAME, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
}