#pragma once
#include "Character.h"
#include "Skill.h"

enum attack_type {
	OFF_WEAPON,
	WARRIOR_WEAPON,
	ARCHER_WEAPON,
	WIZARD_WEAPON,
	ROGUE_WEAPON,
	NB_ATTACK_TYPE
};

class Attack : Skill {
private:
	attack_type m_attack_type;

	Attack();
public:
	Attack(attack_type p_attack_type);

	int dmg();

	~Attack();
};