#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/node.hpp>
using namespace godot;

struct Bullet {
    real_t rotation;
    Vector2 velocity;
    Vector2 position;
    uint32_t lifetime;
};

struct CharacterCRect {
    Vector2 position;
    Vector2 size;
};

class CharacterCRectList {
  public:
    uint32_t size;
    uint32_t bytes_size;
    // for the sake of performance, bytes_data can contains garbage data if travel pass the specified bytes_size
    PackedByteArray bytes_data; 
    const uint8_t bytes_per_crect = sizeof(real_t) * 4; // position + size of rect
    Dictionary character_rid_to_crect_index;

  private:
    uint32_t bytes_capacity; 

    CharacterCRectList() {
        bytes_data = PackedByteArray();
        bytes_capacity = 400 * bytes_per_crect;
        bytes_data.resize(bytes_capacity);
        bytes_size = 0;
        size = 0;
    }

    void add_crect(const Ref<Node>& character) {
        uint64_t instance_id = character->get_instance_id();
        
        character_rid_to_crect_index[instance_id] = bytes_size / bytes_per_crect;

        if ((bytes_size + bytes_per_crect) < bytes_capacity) {
            bytes_capacity = bytes_capacity * 2;
            bytes_data.resize(bytes_capacity);
        }

        Rect2 collision_rect = character->get("collision_rect");

        const uint8_t unit_size = sizeof(real_t);
        bytes_data.encode_float(bytes_size, collision_rect.position.x);
        bytes_data.encode_float(bytes_size + unit_size, collision_rect.position.y);
        bytes_data.encode_float(bytes_size + unit_size * 2, collision_rect.size.x);
        bytes_data.encode_float(bytes_size + unit_size * 3, collision_rect.size.y);
        
        bytes_size += bytes_per_crect;
        size++;
    }

    void remove_crect(const Ref<Node>& character) {
        uint64_t instance_id = character->get_instance_id();
        int64_t index = character_rid_to_crect_index[instance_id];
        character_rid_to_crect_index.erase(instance_id);

        // erase by moving the last crect bytes to deleted crect
        int64_t last_index = bytes_size - bytes_per_crect;

        for (int i = 0; i < bytes_per_crect; i++) {
            bytes_data[index + i] = bytes_data[last_index + i];
        }

        // reduce the size of the bytes_data (but not the capacity)
        bytes_size -= bytes_per_crect;
        size--;
    }
    
    bool circle_rect_collision(const Vector2& circle_position, float circle_radius, const Rect2& rect) 
{
    // Find the closest point on the rectangle to the circle center
    float closest_x = std::max(rect.position.x, std::min(circle_position.x, rect.position.x + rect.size.x));
    float closest_y = std::max(rect.position.y, std::min(circle_position.y, rect.position.y + rect.size.y));
    
    // Calculate squared distance and compare with squared radius
    float dx = circle_position.x - closest_x;
    float dy = circle_position.y - closest_y;
    
    return (dx * dx + dy * dy) <= (circle_radius * circle_radius);
}
    
};

#endif