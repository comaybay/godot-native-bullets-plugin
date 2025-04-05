
#include "character_pool.h"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/string.hpp>

CharacterPool::CharacterPool(uint32_t initial_capacity, TypedArray<StringName> character_ids, Node *data_autoload)
    : character_crects(initial_capacity),
      active_characters(initial_capacity),
      reusable_characters(initial_capacity)
{
    this->data_autoload = data_autoload;

    for (int i = 0; i < character_ids.size(); i++)
    {
        StringName character_id = character_ids[i];
        Ref<PackedScene> character_scene = load_character_scene(character_id);

        character_scenes[character_id] = character_scene;
    }
}

Character *CharacterPool::get(StringName character_id)
{
    Character *character;

    if (reusable_characters.size() > 0)
    {
        character = reusable_characters.pop();
        character->reset();
    }
    else
    {
        character = create(character_id);
    }

    character->index_in_pool = active_characters.size();
    active_characters.push_back(character);
    return character;
}

void CharacterPool::release(Character *character)
{
    uint32_t index = character->index_in_pool;
    active_characters.swap_remove(index);

    // update the index of the character since we have move it to new index
    active_characters[index]->index_in_pool = index;

    reusable_characters.push_back(character);
}

Ref<PackedScene> CharacterPool::load_character_scene(StringName character_id)
{
    bool is_cat = data_autoload->get("cat_info").operator Dictionary().has(character_id);
    String path;
    if (is_cat)
    {
        path = "res://src/characters/cats/" + character_id + "/" + character_id + ".tscn";
    }
    else
    {
        path = "res://src/characters/dogs/" + character_id + "/" + character_id + ".tscn";
    }

    return ResourceLoader::get_singleton()->load(path);
}

Character *CharacterPool::create(StringName character_id)
{
    Ref<PackedScene> character_scene = character_scenes[character_id];
    return Object::cast_to<Character>(character_scene->instantiate());
}

CharacterCRectList::CharacterCRectList(uint32_t initial_capacity)
    : vec2_position_bytes(initial_capacity),
      vec2_size_bytes(initial_capacity),
      uint_id_bytes(initial_capacity), size(0) {}

void CharacterCRectList::reserve(uint32_t capacity)
{
    vec2_position_bytes.reserve(capacity);
    vec2_size_bytes.reserve(capacity);
    uint_id_bytes.reserve(capacity);
}

void CharacterCRectList::add(Rect2 collision_rect, uint32_t id)
{
    vec2_position_bytes.push_back(collision_rect.position.x);
    vec2_position_bytes.push_back(collision_rect.position.y);
    vec2_size_bytes.push_back(collision_rect.size.x);
    vec2_size_bytes.push_back(collision_rect.size.y);
    uint_id_bytes.push_back(id);
    size++;
}

void CharacterCRectList::swap_remove(uint32_t index)
{
    vec2_position_bytes.swap_remove(index, 8);
    vec2_size_bytes.swap_remove(index, 8);
    uint_id_bytes.swap_remove(index, 4);
    size--;
}

// glsl
/**
// Rectangle data
layout(std430, binding = 0) buffer RectPositions {
    vec2 rectPositions[];
};
layout(std430, binding = 1) buffer RectSizes {
    vec2 rectSizes[];
};
layout(std430, binding = 2) buffer RectIDs {
    uint rectIDs[];  // IDs for each rectangle
};

// Circle data
layout(std430, binding = 3) buffer CirclePositions {
    vec2 circlePositions[];
};
layout(std430, binding = 4) buffer CircleRadii {
    float circleRadii[];
};
layout(std430, binding = 5) buffer CircleIDs {
    uint circleIDs[];  // IDs for each circle
};

// Output collision pairs
layout(std430, binding = 6) buffer CollisionPairs {
    uint count;  // Number of collisions found
    CollisionPair pairs[];  // Array of collision pairs
};

struct CollisionPair {
    uint circleID;
    uint rectID;
};

// Check collision and record pair if found
void checkAndRecordCollision(uint circleIndex, uint rectIndex) {
    vec2 circlePos = circlePositions[circleIndex];
    float radius = circleRadii[circleIndex];
    vec2 rectPos = rectPositions[rectIndex];
    vec2 rectSize = rectSizes[rectIndex];

    // Find the closest point on the rectangle to the circle center
    float closestX = max(rectPos.x, min(circlePos.x, rectPos.x + rectSize.x));
    float closestY = max(rectPos.y, min(circlePos.y, rectPos.y + rectSize.y));
    vec2 closest = vec2(closestX, closestY);
    vec2 diff = circlePos - closest;

    // If collision detected, record the pair
    if (dot(diff, diff) <= (radius * radius)) {
        uint pairIndex = atomicAdd(count, 1);
        // Prevent buffer overflow
        if (pairIndex < pairs.length()) {
            pairs[pairIndex].circleID = circleIDs[circleIndex];
            pairs[pairIndex].rectID = rectIDs[rectIndex];
        }
    }
}
 */
