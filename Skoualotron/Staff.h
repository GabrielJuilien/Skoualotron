#pragma once
#include"RangeWeapon.h"

class Staff : public RangeWeapon {
private:
	Staff();
public:
	int m_intelligence;
	int m_cost;
	Staff(std::string p_name, int p_dmg, int p_crit_chance, int p_health, int p_attack, int p_intelligence, int p_cost);

	~Staff();
};