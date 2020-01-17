#include "MeleeWeapon.h"

MeleeWeapon::MeleeWeapon():Weapon() {
	m_durability = 0;
	m_initialDurability = 0;
}

MeleeWeapon::MeleeWeapon(std::string p_name, int p_dmg, int p_crit_chance, int p_durability, int p_health, int p_attack): Weapon(p_name, p_dmg, p_crit_chance, p_health, p_attack) {
	m_durability = p_durability;
	m_initialDurability = p_durability;
}


void MeleeWeapon::addDurability(int p_durability) {
	if (durability() + p_durability < m_initialDurability) {
		m_durability += p_durability;
		return;
	}
	m_durability = p_durability;
}

void MeleeWeapon::subDurability(int p_durability) {
	if (durability() - p_durability > 0) {
		m_durability -= p_durability;
	}
	else m_durability = 0;
}

void MeleeWeapon::durability(int p_durability) {
	m_durability = p_durability;
}

int MeleeWeapon::durability() {
	return m_durability;
}

int MeleeWeapon::maxDurability() {
	return m_initialDurability;
}

bool MeleeWeapon::isBroken() {
	if (m_durability > 0) return false;
	else return true;
}


int MeleeWeapon::dmg() {
	if (!isBroken()) return base_dmg();
	else return base_dmg() / 10;
}

MeleeWeapon::~MeleeWeapon() {

}