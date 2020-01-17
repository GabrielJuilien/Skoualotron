#include "Bow.h"

Bow::Bow() : RangeWeapon() {
	m_arrows = 0;
}

Bow::Bow(std::string p_name, int p_dmg, int p_crit_chance, int p_health, int p_attack, int p_arrows) : RangeWeapon(p_name, p_dmg, p_crit_chance, p_health, p_attack) {
	m_arrows = p_arrows;
}

Bow::~Bow() {

}