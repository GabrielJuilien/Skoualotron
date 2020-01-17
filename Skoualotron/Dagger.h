#pragma once
#include "MeleeWeapon.h"

class Dagger : public MeleeWeapon {
private:
	Dagger();
public:
	int m_defense;
	int m_agility;
	Dagger(std::string p_name, int p_dmg, int p_crit_chance, int p_durability, int p_health, int p_attack, int p_defense, int p_agility);

	void loseDurability();

	~Dagger();
};