#include "Weapon.h"
#include "MeleeWeapon.h"

Weapon::Weapon() {
	m_name = "";
	m_base_dmg = 0;
	m_crit_chance = 0;
}

Weapon::Weapon(std::string p_name, int p_base_dmg, int p_crit_chance, int p_health, int p_attack) {
	m_name = p_name;
	m_base_dmg = p_base_dmg;
	m_crit_chance = p_crit_chance;
	m_health = p_health;
	m_attack = p_attack;
}

void Weapon::name(std::string p_name) {
	m_name = p_name;
}

std::string Weapon::name() {
	return m_name;
}

void Weapon::base_dmg(int p_base_dmg) {
	m_base_dmg = p_base_dmg;
}

int Weapon::base_dmg() {
	MeleeWeapon* weapon;
	if (Weapon::is<MeleeWeapon>(*this)) {
		weapon = dynamic_cast<MeleeWeapon*>(this);
		if (weapon->durability() == 0) {
			if (isEnchanted()) {
				isEnchanted(false);
				return m_base_dmg * 1.33f / 10;
			}
			else return m_base_dmg / 10;
		}
	}

	if (isEnchanted()) {
		isEnchanted(false);
		return m_base_dmg * 1.33f;
	}
	else return m_base_dmg;
}

void Weapon::crit_chance(int p_crit_chance) {
	m_crit_chance = p_crit_chance;
}

int Weapon::crit_chance() {
	return m_crit_chance;
}

void Weapon::health(int p_health) {
	m_health = p_health;
}

int Weapon::health() {
	return m_health;
}

void Weapon::attack(int p_attack) {
	m_attack = p_attack;
}

int Weapon::attack() {
	return m_attack;
}

void Weapon::initAnimation(std::string p_path, SDL_Renderer* p_renderer, int p_length, int p_currentFrame, bool p_loop, bool p_play, int x_spriteSize, int y_spriteSize, int x_spritePos, int y_spritePos, SIDE p_side) {
	try {
		m_animation = new Animation(p_path, p_renderer, p_length, p_currentFrame, p_loop, p_play, x_spriteSize, y_spriteSize, x_spritePos, y_spritePos, p_side);
	}
	catch (...) {
		std::cout << "Could't load image at " << p_path << "\n";
	}
}

void Weapon::initRecap(SDL_Renderer* p_renderer) {
	std::string text = "";
	text.append(m_name);
	text.append("\nDMG: ");
	text.append(std::to_string(m_base_dmg));
	text.append("\nCRT: ");
	text.append(std::to_string(m_crit_chance));
	text.append("\nHP: ");
	text.append(std::to_string(m_health));
	text.append("\nATK: ");
	text.append(std::to_string(m_attack));

	m_statRecap = new Animation(text, p_renderer);
}

void Weapon::renderRecap(int x_pos, int y_pos) {
	m_statRecap->setPos(x_pos, y_pos);
	m_statRecap->render();
}

Animation* Weapon::animation() {
	return m_animation;
}

void Weapon::isEnchanted(bool p_enchantement) {
	m_enchantement = p_enchantement;
}

bool Weapon::isEnchanted() {
	return m_enchantement;
}

Weapon::~Weapon() {
	
}