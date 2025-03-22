#ifndef DANMAKU_H
#define DANMAKU_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>
#include "structures.h"

using namespace godot;

class Danmaku : public Node {
	GDCLASS(Danmaku, Node)

private:
    BulletPool dog_bullets;
    BulletPool cat_bullets;

    CharacterCRectList cat_rects;
    CharacterCRectList dog_rects;

public:
	Danmaku();
	~Danmaku();
};

#endif