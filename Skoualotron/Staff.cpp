#include "Staff.h"

Staff::Staff() : RangeWeapon() {
	m_intelligence = 0;
	m_cost = 0;
}

Staff::Staff(std::string p_name, int p_dmg, int p_crit_chance, int p_health, int p_attack, int p_intelligence, int p_cost) : RangeWeapon(p_name, p_dmg, p_crit_chance, p_health, p_attack) {
	m_cost = p_cost;
	m_intelligence = p_intelligence;
}

Staff::~Staff() {

}