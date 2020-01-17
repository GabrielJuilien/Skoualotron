#pragma once
#include "Weapon.h"

class MeleeWeapon: public Weapon {
protected:
	int m_durability;
	int m_initialDurability;

	MeleeWeapon();
public:
	MeleeWeapon(std::string p_name, int p_dmg, int p_crit_chance, int p_durability, int p_health, int p_attack);

	void addDurability(int p_durability);
	void subDurability(int p_durability);

	void durability(int p_durability);
	int durability();

	int maxDurability();

	bool isBroken();

	int dmg();

	virtual ~MeleeWeapon();
};