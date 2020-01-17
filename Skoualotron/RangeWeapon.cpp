#include "RangeWeapon.h"

RangeWeapon::RangeWeapon() : Weapon() {

}

RangeWeapon::RangeWeapon(std::string p_name, int p_dmg, int p_crit_chance, int p_health, int p_attack) : Weapon(p_name, p_dmg, p_crit_chance, p_health, p_attack) {

}

RangeWeapon::~RangeWeapon() {

}