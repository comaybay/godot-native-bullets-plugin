#ifndef CHARACTER_POOL_H
#define CHARACTER_POOL_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include <unordered_map>
#include "structures.h"
#include "character.h"

using namespace godot;

class CharacterCRectList
{
private:
    uint32_t size;
    UnorderedPackedByteArray vec2_position_bytes;
    UnorderedPackedByteArray vec2_size_bytes;
    UnorderedPackedByteArray uint_id_bytes;

public:
    CharacterCRectList(uint32_t initial_capacity);
    void reserve(uint32_t capacity);
    void add(Rect2 collision_rect, uint32_t id);
    void swap_remove(uint32_t index);
};


class CharacterPool
{
public:
    CharacterPool(uint32_t initial_capacity, TypedArray<StringName> character_ids, Node* data_autoload);
    uint32_t size;
    CharacterCRectList character_crects;

    UnorderedVector<Character*> active_characters;
    UnorderedVector<Character*> reusable_characters;

    Character* get(StringName character_id);
    void release(Character* character);

private:
    std::unordered_map<StringName, Ref<PackedScene>> character_scenes;
    Node* data_autoload;

    Ref<PackedScene> load_character_scene(StringName character_id);

    Character* create(StringName character_id);
};
#endif

