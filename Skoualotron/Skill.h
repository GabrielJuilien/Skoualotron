#pragma once
#include "Character.h"

class Skill {
protected:
	Character* m_source;
	Character* m_target;
	
	Skill();
public:
	Skill(Character* p_source, Character* p_target);

	~Skill();
};
