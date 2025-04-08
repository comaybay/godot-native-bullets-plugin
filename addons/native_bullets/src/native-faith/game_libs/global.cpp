#include "global.h"
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/geometry2d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

Global::Global()
{
}

void Global::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("_ready"), &Global::_ready);
  ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &Global::_physics_process);
  ClassDB::bind_method(D_METHOD("is_host_OS_web"), &Global::is_host_OS_web);
  ClassDB::bind_method(D_METHOD("is_host_OS_web_mobile"), &Global::is_host_OS_web_mobile);
  ClassDB::bind_method(D_METHOD("wait", "wait_time", "immune_to_time_scale"), &Global::wait, DEFVAL(false));
  ClassDB::bind_method(D_METHOD("clean_up"), &Global::clean_up);
  ClassDB::bind_method(D_METHOD("rand_sign"), &Global::rand_sign);
  ClassDB::bind_method(D_METHOD("signfnz", "value"), &Global::signfnz);
  ClassDB::bind_method(D_METHOD("rand_bool"), &Global::rand_bool);
  ClassDB::bind_method(D_METHOD("find_closest_node", "node", "others"), &Global::find_closest_node);
  ClassDB::bind_method(D_METHOD("find_closest_character", "character", "others"), &Global::find_closest_character);
  ClassDB::bind_method(D_METHOD("find_closest_node_to_position", "position", "others"), &Global::find_closest_node_to_position);
  ClassDB::bind_method(D_METHOD("chance", "probability"), &Global::chance);
  ClassDB::bind_method(D_METHOD("weighted_chance", "weighted_choices"), &Global::weighted_chance);
  ClassDB::bind_method(D_METHOD("set_prop", "node", "prop_path", "new_prop_data"), &Global::set_prop);
  ClassDB::bind_method(D_METHOD("get_prop_value", "original_value", "prop_value_data", "base_owner"), &Global::get_prop_value, DEFVAL(nullptr));
  ClassDB::bind_method(D_METHOD("strip_bb_code", "text"), &Global::strip_bb_code);
  ClassDB::bind_method(D_METHOD("strip_custom_data", "text"), &Global::strip_custom_data);
  ClassDB::bind_method(D_METHOD("create_physics_tween"), &Global::create_physics_tween);

  ADD_SIGNAL(MethodInfo("time_scale_changed"));
  BIND_PROPERTY(time_scale, Variant::FLOAT, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE);
}

float Global::get_time_scale() const {
    return time_scale;
}

void Global::set_time_scale(float value) {
    if (time_scale < 3.0f || Math::is_equal_approx(time_scale, 3.0f)) {
        Engine::get_singleton()->set_physics_ticks_per_second(std::max(int(60 * time_scale), 60));
        Engine::get_singleton()->set_max_physics_steps_per_frame(std::max(int(8 * time_scale), 8));
    } else {
        Engine::get_singleton()->set_physics_ticks_per_second(60);
        Engine::get_singleton()->set_max_physics_steps_per_frame(8);
    }
    
    time_scale = value;
    emit_signal("time_scale_changed");
}

Global* Global::get_singleton()
{
    return _singleton;
}

void Global::_ready() {
    _singleton = this;
    VIEWPORT_SIZE = Vector2(
        ProjectSettings::get_singleton()->get_setting("display/window/size/viewport_width"),
        ProjectSettings::get_singleton()->get_setting("display/window/size/viewport_height")
    );
    tree = get_tree();
    _special_chars.push_back(".");
    _special_chars.push_back(",");
    _special_chars.push_back(" ");
    _special_chars.push_back("?");
    _special_chars.push_back("!");
    _special_chars.push_back("~");
}

void Global::_physics_process(float delta) {
    _delta_timer_pool.physics_process(delta);
}

bool Global::is_host_OS_web() {
    return OS::get_singleton()->get_name() == "WEB";
}

bool Global::is_host_OS_web_mobile() {
    return OS::get_singleton()->has_feature("web_android") || OS::get_singleton()->has_feature("web_ios");
}

DeltaTimer& Global::get_delta_timer(float wait_time, bool immune_to_time_scale) {
    return _delta_timer_pool.get_timer(
        wait_time, 
        immune_to_time_scale
    );
}

Signal Global::wait(float wait_time, bool immune_to_time_scale) {
    return Signal(&get_delta_timer(wait_time, immune_to_time_scale), "timeout");
}

void Global::clean_up() {
    _delta_timer_pool.clear();
}

int Global::rand_sign() {
    return Math::sign(2 * UtilityFunctions::randi_range(0, 1) - 1);
}

float Global::signfnz(float value) {
    float s = Math::sign(value);
    return Math::is_zero_approx(s) ? 1.0f : s;
}

bool Global::rand_bool() {
    return UtilityFunctions::randi_range(0, 1) == 1;
}

Node2D* Global::find_closest_node(Node2D* node, const Array& others) {
    return find_closest_node_to_position(node->get_global_position(), others);
}

Character* Global::find_closest_character(Character* character, const Array& others) {
    Character* closest = nullptr;
    float min_distance = FLT_MAX;
    
    Vector2 char_pos = character->get_global_position();
    for (int i = 0; i < others.size(); i++) {
        Character* other = Object::cast_to<Character>(others[i]);
        if (other) {
            float distance = char_pos.distance_squared_to(other->get_global_position());
            if (min_distance > distance) {
                min_distance = distance;
                closest = other;
            }
        }
    }
    
    return closest;
}

Node2D* Global::find_closest_node_to_position(const Vector2& position, const Array& others) {
    Node2D* closest = nullptr;
    float min_distance = FLT_MAX;
    
    for (int i = 0; i < others.size(); i++) {
        Node2D* other = Object::cast_to<Node2D>(others[i]);
        if (other) {
            float distance = position.distance_squared_to(other->get_global_position());
            if (min_distance > distance) {
                min_distance = distance;
                closest = other;
            }
        }
    }
    
    return closest;
}

bool Global::chance(float probability) {
    return UtilityFunctions::randf() < probability;
}

int Global::weighted_chance(const Array& weighted_choices) {
    int sum = 0;
    for (int i = 0; i < weighted_choices.size(); i++) {
        sum += (int)weighted_choices[i];
    }
    
    int rnd = UtilityFunctions::randi_range(0, sum - 1);
    for (int i = 0; i < weighted_choices.size(); i++) {
        if (rnd < (int)weighted_choices[i]) {
            return i;
        }
        
        rnd -= (int)weighted_choices[i];
    }
    
    ERR_FAIL_V_MSG(-1, "Should never get here");
    return -1;
}

void Global::set_prop(Node* node, const String& prop_path, const Variant& new_prop_data) {
    PackedStringArray prop_path_parts = prop_path.split(":");
    String prop_name = prop_path_parts[prop_path_parts.size() - 1];
    Variant old_prop_val = node;
    Node* prop_owner = nullptr;
    
    bool is_material_shader = prop_path_parts.size() > 1 && prop_path_parts[prop_path_parts.size() - 2] == "material";
    
    for (int i = 0; i < prop_path_parts.size(); i++) {
        prop_owner = Object::cast_to<Node>(old_prop_val);
        old_prop_val = prop_owner->get(prop_path_parts[i]);
    }
    if (old_prop_val == Variant(nullptr) && !is_material_shader) {
        return;
    }
    
    Variant prop_value = prop_owner->get(prop_name);
    if (is_material_shader) {
        ShaderMaterial* material = Object::cast_to<ShaderMaterial>(prop_owner);
        prop_value = material->get_shader_parameter(prop_name);
    }
    
    Variant new_val = get_prop_value(prop_value, new_prop_data, node);
    
    if (is_material_shader) {
        ShaderMaterial* material = Object::cast_to<ShaderMaterial>(prop_owner);
        material->set_shader_parameter(prop_name, new_val);
    } else {
        prop_owner->set(prop_name, new_val);
    }
}

Variant Global::get_prop_value(const Variant& original_value, const Variant& prop_value_data, Node* base_owner) {
    Variant new_value = original_value;
    
    if (prop_value_data.get_type() == Variant::DICTIONARY) {
        Dictionary dict = prop_value_data;
        Array keys = dict.keys();
        
        for (int i = 0; i < keys.size(); i++) {
            String set_type = keys[i];
            Variant set_value = dict[set_type];
            
            if (set_type == "value") {
                if (set_value.get_type() == Variant::ARRAY) {
                    Array arr = set_value;
                    new_value = UtilityFunctions::randf_range((float)arr[0], (float)arr[1]);
                } else {
                    new_value = set_value;
                }
            }
            
            if (set_type == "scale") {
                if (set_value.get_type() == Variant::ARRAY) {
                    Array arr = set_value;
                    new_value = (float)new_value * UtilityFunctions::randf_range((float)arr[0], (float)arr[1]);
                } else {
                    new_value = (float)new_value * (float)set_value;
                }
            }
            
            if (set_type == "expression") {
                Ref<Expression> expression;
                expression.instantiate();
                Error error = expression->parse(set_value, Array());
                if (error != OK) {
                    ERR_PRINT(expression->get_error_text());
                    return Variant();
                }
                
                new_value = expression->execute(Array(), base_owner);
            }
        }
    } else if (prop_value_data.get_type() == Variant::ARRAY) {
        Array arr = prop_value_data;
        new_value = UtilityFunctions::randf_range((float)arr[0], (float)arr[1]);
    } else {
        new_value = prop_value_data;
    }
    
    if (new_value.get_type() == Variant::STRING) {
        String str = new_value;
        if (str.strip_edges().begins_with("#")) {
            new_value = Color(str);
        }
    }
    
    return new_value;
}

String Global::strip_bb_code(const String& text) {
    String result;
    int from = 0;
    
    while (true) {
        int lb_pos = text.find("[", from);
        if (lb_pos == -1) {
            break;
        }
        
        int rb_pos = text.find("]", lb_pos + 1);
        if (rb_pos == -1) {
            break;
        }
        
        result += text.substr(from, lb_pos - from);
        from = rb_pos + 1;
    }
    
    result += text.substr(from);
    return result.replace("[", "").replace("]", "");
}

String Global::strip_custom_data(const String& text) {
    String result;
    int from = 0;
    
    while (true) {
        int lb_pos = text.find("{", from);
        if (lb_pos == -1) {
            break;
        }
        
        int rb_pos = text.find("}", lb_pos + 1);
        if (rb_pos == -1) {
            break;
        }
        
        result += text.substr(from, lb_pos - from);
        from = rb_pos + 1;
    }
    
    result += text.substr(from);
    result = result.replace("{", "").replace("}", "");
    
    for (int i = 0; i < _special_chars.size(); i++) {
        result = result.replace(_special_chars[i], "");
    }
    
    return result;
}

Ref<Tween> Global::create_physics_tween() {
    return tree->create_tween()->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS);
}
