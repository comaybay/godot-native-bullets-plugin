#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include <vector>
using namespace godot;

struct Bullet
{
    Vector2 position;
    Vector2 velocity;
    uint16_t radius;
    int32_t id;
    bool destroyed;  // bullet has collided with something and is no longer active
    bool controlled; // bullet is currently being used by bullet controller

    Bullet(uint64_t id, uint16_t radius);
    Bullet();

    void reset(uint64_t id, uint16_t radius);

    inline bool is_reusable() const { return destroyed && !controlled; }
};

struct CharacterCRect
{
    Vector2 position;
    Vector2 size;
};

template <typename T>
class UnorderedVector
{
private:
    uint32_t actual_size;
    std::vector<T> vector;

public:
    UnorderedVector(uint32_t initial_capacity) : actual_size(0), vector(initial_capacity) {}

    inline uint32_t size() const { return actual_size; }

    inline uint32_t capacity() const { return vector.capacity(); }

    inline T &operator[](uint32_t index)
    {
        return vector[index];
    }

    inline const T &operator[](uint32_t index) const
    {
        return vector[index];
    }

    inline void push_back(const T &value)
    {
        if (actual_size < vector.size())
        {
            vector[actual_size] = value;
        }
        else
        {
            vector.push_back(value);
        }
        actual_size++;
    }

    template <typename... Args>
    inline void emplace_back(Args &&...args)
    {
        vector.emplace_back(std::forward<Args>(args)...);
        actual_size++;
    }

    inline void swap_remove(uint32_t index)
    {
        // move the last active item to the index of the removed item (keeping all active items contiguous)
        vector[index] = vector[actual_size - 1];
        actual_size--;
    }

    inline T pop()
    {
        actual_size--;
        return vector[actual_size];
    }

    inline void clear()
    {
        actual_size = 0;
    }
};

struct UnorderedPackedByteArray
{
private:
    PackedByteArray array;
    uint32_t actual_size;

public:
    UnorderedPackedByteArray(uint32_t initial_capacity) : actual_size(0) { reserve(initial_capacity); }
    inline const PackedByteArray &get_packed_byte_array() const { return array; }
    inline void reserve(uint32_t capacity) { array.resize(capacity); }
    inline const uint8_t &operator[](uint32_t index) const { return array[index]; }
    inline uint32_t size() const { return actual_size; }
    inline void push_back(uint8_t byte) { array.push_back(byte); actual_size++; }
    inline void swap_remove(uint32_t index, uint32_t amount)
    {
        for (uint32_t i = 0; i < amount; i++)
        {
            array[index + i] = array[actual_size - amount + i];
            actual_size--;
        }
    }
};
#endif
