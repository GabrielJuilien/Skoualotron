#pragma once
#include "Animation.h"
#include <vector>

class AnimationStack {
public:
	std::vector<Animation*> animation;

	AnimationStack();

	void append(Animation* p_animation);
	
	void renderStack();

	~AnimationStack();
};