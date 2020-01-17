#pragma once
#include "RangeWeapon.h"

class Bow : public RangeWeapon {
private:
	Bow();

public:
	int m_arrows;

	Bow(std::string p_name, int p_dmg, int p_crit_chance, int p_health, int p_attack, int p_arrows);

	~Bow();
};