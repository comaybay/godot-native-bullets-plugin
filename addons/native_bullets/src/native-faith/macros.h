#ifndef MACROS_H
#define MACROS_H

#define DECLARE_GETTER_SETTER(type, member_name) \
  inline type get_##member_name() const; \
  inline void set_##member_name(type value);

#define PROPERTY(type, member_name, default_value) \
  DECLARE_GETTER_SETTER(type, member_name) \
  type member_name = default_value;

#define PROPERTY_GETONLY(type, member_name, default_value) \
  inline type get_##member_name() const; \
  type member_name = default_value;

// Define helper macros for property binding
#define BIND_PROPERTY_GETONLY(prop_name, prop_type, hint, hint_str, usage) \
    ClassDB::bind_method(D_METHOD("get_" #prop_name), &get_##prop_name); \
    ADD_PROPERTY(PropertyInfo(Variant::prop_type, #prop_name, hint, hint_str, usage), "", "get_" #prop_name)

#define BIND_PROPERTY(prop_name, prop_type, hint, hint_str, usage) \
    ClassDB::bind_method(D_METHOD("set_" #prop_name, "value"), &set_##prop_name); \
    ClassDB::bind_method(D_METHOD("get_" #prop_name), &get_##prop_name); \
    ADD_PROPERTY(PropertyInfo(Variant::prop_type, #prop_name, hint, hint_str, usage), "set_" #prop_name, "get_" #prop_name)

#define BIND_METHOD_0(method_name) \
    ClassDB::bind_method(D_METHOD(#method_name), &method_name)

#define BIND_METHOD(method_name, ...) \
    ClassDB::bind_method(D_METHOD(#method_name, __VA_ARGS__), &method_name)

#define BIND_STATIC_METHOD(class_name, method_name, ...) \
    ClassDB::bind_static_method(#class_name, D_METHOD(#method_name, __VA_ARGS__), &method_name)

#define DEFINE_GETTER(class_name, type, member_name) \
    type class_name::get_##member_name() const { return member_name; }

#define DEFINE_GETTER_SETTER(class_name, type, member_name) \
    DEFINE_GETTER(class_name, type, member_name) \
    void class_name::set_##member_name(type value) { member_name = value; }

// For 1 argument methods with default values
#define BIND_METHOD_1D1(method_name, param1, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1), &method_name, def1)

// For 2 argument methods with default values
#define BIND_METHOD_2D1(method_name, param1, param2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2), &method_name, def1)

#define BIND_METHOD_2D2(method_name, param1, param2, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2), &method_name, def1, def2)

// For 3 argument methods with default values
#define BIND_METHOD_3D1(method_name, param1, param2, param3, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3), &method_name, def1)

#define BIND_METHOD_3D2(method_name, param1, param2, param3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3), &method_name, def1, def2)

#define BIND_METHOD_3D3(method_name, param1, param2, param3, def3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3), &method_name, def1, def2, def3)

// For 4 argument methods with default values
#define BIND_METHOD_4D1(method_name, param1, param2, param3, param4, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4), &method_name, def1)

#define BIND_METHOD_4D2(method_name, param1, param2, param3, param4, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4), &method_name, def1, def2)

#define BIND_METHOD_4D3(method_name, param1, param2, param3, param4, def3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4), &method_name, def1, def2, def3)

#define BIND_METHOD_4D4(method_name, param1, param2, param3, param4, def4, def3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4), &method_name, def1, def2, def3, def4)

// For 5 argument methods with default values
#define BIND_METHOD_5D1(method_name, param1, param2, param3, param4, param5, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5), &method_name, def1)

#define BIND_METHOD_5D2(method_name, param1, param2, param3, param4, param5, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5), &method_name, def1, def2)

#define BIND_METHOD_5D3(method_name, param1, param2, param3, param4, param5, def3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5), &method_name, def1, def2, def3)

#define BIND_METHOD_5D4(method_name, param1, param2, param3, param4, param5, def4, def3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5), &method_name, def1, def2, def3, def4)

#define BIND_METHOD_5D5(method_name, param1, param2, param3, param4, param5, def5, def4, def3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5), &method_name, def1, def2, def3, def4, def5)

// For 6 argument methods with default values
#define BIND_METHOD_6D1(method_name, param1, param2, param3, param4, param5, param6, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5, #param6), &method_name, def1)

#define BIND_METHOD_6D2(method_name, param1, param2, param3, param4, param5, param6, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5, #param6), &method_name, def1, def2)

#define BIND_METHOD_6D3(method_name, param1, param2, param3, param4, param5, param6, def3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5, #param6), &method_name, def1, def2, def3)

#define BIND_METHOD_6D4(method_name, param1, param2, param3, param4, param5, param6, def4, def3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5, #param6), &method_name, def1, def2, def3, def4)

#define BIND_METHOD_6D5(method_name, param1, param2, param3, param4, param5, param6, def5, def4, def3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5, #param6), &method_name, def1, def2, def3, def4, def5)

#define BIND_METHOD_6D6(method_name, param1, param2, param3, param4, param5, param6, def6, def5, def4, def3, def2, def1) \
    ClassDB::bind_method(D_METHOD(#method_name, #param1, #param2, #param3, #param4, #param5, #param6), &method_name, def1, def2, def3, def4, def5, def6)

// Add more as needed for additional parameters
#endif // MACROS_H
