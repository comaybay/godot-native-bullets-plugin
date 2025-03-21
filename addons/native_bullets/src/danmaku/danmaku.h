#ifndef DANMAKU_H
#define DANMAKU_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
using namespace godot;

class Danmaku : public Node {
	GDCLASS(Danmaku, Node)

private:
    std::vector<std::unique_ptr<Bullet>> dog_bullets;
    std::vector<std::unique_ptr<Bullet>> cat_bullets;

    std::vector<CharacterCRect> cat_rects;
    std::vector<CharacterCRect> dog_rects;

public:
	Danmaku();
	~Danmaku();
};

#endif