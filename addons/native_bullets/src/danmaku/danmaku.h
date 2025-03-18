#ifndef DANMAKU_H
#define DANMAKU_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>

using namespace godot;

struct Bullet {
    float rotation;
    Vector2 velocity;
    uint32_t lifetime;
};

class Danmaku : public Node {
	GDCLASS(Danmaku, Node)

private:
    std::vector<std::unique_ptr<Bullet>> dog_bullets;
    std::vector<std::unique_ptr<Bullet>> cat_bullets;

public:
	Danmaku();
	~Danmaku();
};

#endif