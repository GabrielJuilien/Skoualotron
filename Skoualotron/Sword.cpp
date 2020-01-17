#include "Sword.h"

Sword::Sword() : MeleeWeapon() {

}

Sword::Sword(std::string p_name, int p_dmg, int p_crit_chance, int p_durability, int p_health, int p_attack) : MeleeWeapon(p_name, p_dmg, p_crit_chance, p_durability, p_health, p_attack) {

}

void Sword::loseDurability() {
	subDurability(rand() % 4 + 2);
}

Sword::~Sword() {

}
