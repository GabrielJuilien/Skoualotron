#pragma once
#include "Weapon.h"

class RangeWeapon : public Weapon {
protected:
	RangeWeapon();

public:
	RangeWeapon(std::string p_name, int p_dmg, int p_crit_chance, int p_health, int p_attack);
	
	virtual ~RangeWeapon();
};