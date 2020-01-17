#pragma once
#include "MeleeWeapon.h"

class Sword : public MeleeWeapon {
private:
	Sword();
public:
	Sword(std::string p_name, int p_dmg, int p_crit_chance, int p_durability, int p_health, int p_attack);
	void loseDurability();
	~Sword();
};