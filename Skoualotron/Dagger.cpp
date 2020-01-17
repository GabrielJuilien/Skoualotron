#include "Dagger.h"

Dagger::Dagger() : MeleeWeapon() {
	m_agility = 0;
	m_defense = 0;
};

Dagger::Dagger(std::string p_name, int p_dmg, int p_crit_chance, int p_durability, int p_health, int p_attack, int p_defense, int p_agility) : MeleeWeapon(p_name, p_dmg, p_crit_chance, p_durability, p_health, p_attack) {
	m_agility = p_agility;
	m_defense = p_defense;
};

void Dagger::loseDurability() {
	subDurability(1);
}

Dagger::~Dagger() {

}