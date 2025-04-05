#include "cooldown_timer.h"
#include "../global.h"

DEFINE_GETTER_SETTER(CooldownTimer, float, wait_time);

void CooldownTimer::set_speed_scale(float speed_scale)
{
    _speed_scale = speed_scale;
    if (_delta_timer != nullptr) {
        _delta_timer->set_speed_scale(speed_scale);
    }
}

float CooldownTimer::get_time_left() const
{
      return _delta_timer != nullptr ? _delta_timer->get_time_left() : 0.0f;
}

bool CooldownTimer::is_stopped() const
{
    return _delta_timer == nullptr;
}

void CooldownTimer::start()
{
  _delta_timer = &Global::get_singleton()->get_delta_timer(wait_time, _immune_to_time_scale);
  _delta_timer->set_speed_scale(_speed_scale);
  _delta_timer->connect("timeout", callable_mp(this, &CooldownTimer::_on_timeout), CONNECT_ONE_SHOT);
}

void CooldownTimer::_on_timeout() {
  _delta_timer = nullptr;
  emit_signal("timeout");
}

void CooldownTimer::_bind_methods()
{
  BIND_PROPERTY(wait_time, Variant::FLOAT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
  ADD_SIGNAL(MethodInfo("timeout"));
}

void CooldownTimer::set_immune_to_time_scale(bool enabled)
{
    _immune_to_time_scale = enabled;
    if (_delta_timer != nullptr) {
        _delta_timer->set_immune_to_time_scale(enabled);
    }
}
