#include "AnimationStack.h"

AnimationStack::AnimationStack() {

}

void AnimationStack::append(Animation* p_animation) {
	animation.push_back(p_animation);
}


void AnimationStack::renderStack() {
	int i;
	size_t length = animation.size();

	for (i = 0; i < length; i++) {
		animation.at(i)->render();
	}
	return;
}

AnimationStack::~AnimationStack() {
	if (animation.size()) {
		for (int i = 0; i < animation.size(); i++) {
			delete animation.at(i);
			animation.at(i) = NULL;
		}
		animation.clear();
	}
}