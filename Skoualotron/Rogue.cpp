#include "Rogue.h"
#include "Archer.h"

Rogue::Rogue() : Character() {

	m_poisonChance = 0;
	m_poisonned = false;

	m_isInvisible = false;
	m_invisibilityCurrentCooldown = 0;
	m_invisibilityMaxCooldown = 0;

	m_arrows = 0;

	anim_sneak = NULL;
	icon_sneak = NULL;
}

Rogue::Rogue(std::string p_name, int p_max_health, int p_dodge_chance, int p_speed, int p_atk, int p_def, int p_agi, int p_intel, int p_poisonChance) : Character(p_name, p_max_health, p_dodge_chance, p_speed, p_atk, p_def, p_agi, p_intel) {
	m_poisonChance = p_poisonChance;
	m_poisonned = false;

	m_isInvisible = false;
	m_invisibilityCurrentCooldown = 0;
	m_invisibilityMaxCooldown = 4;

	m_arrows = 0;

	anim_sneak = NULL;
	icon_sneak = NULL;
}

bool Rogue::isInvisible() {
	if (this)
		return m_isInvisible;
	else return false;
}

bool Rogue::poisonRoll() {
	if (rand() % 101 >= m_poisonChance) return true;
	else return false;
}

int Rogue::arrows() {
	return m_arrows;
}

void Rogue::arrows(int p_arrows) {
	m_arrows = p_arrows;
}

void Rogue::castSkill(int p_pos, Character* p_target) {
	switch (p_pos)
	{
	case 0:
		try {
			canAttack(p_target);
			attack(p_target);
		}
		catch (std::string e) {
			throw e;
		}
		break;
	case 1:
		try {
			canAttackWeapon(p_target);
			attackWeapon(p_target);
		}
		catch (std::string e) {
			throw e;
		}
		break;
	case 2:
		try {
			canSneak();
			sneak();
		}
		catch (std::string e) {
			throw e;
		}
		break;
	default:
		throw std::string("data/Text/undefined_spell.png");
		break;
	}
}


bool Rogue::canSneak() {
	if (m_invisibilityCurrentCooldown > 0) {
		throw std::string("data/Text/spell_on_cooldown.png");
		return false;
	}
	else return true;
}

void Rogue::sneak() {
	m_isInvisible = true;
	m_invisibilityCurrentCooldown = m_invisibilityMaxCooldown;
	anim_sneak->play(true);
	return;
}


bool Rogue::canAttackWeapon(Character* p_target) {
	try {
		isEnnemy(p_target);
	}
	catch (std::string exception) {
		throw exception;
	}

	bool targetIsInvisible = false;
	try { //If target is a rogue
		targetIsInvisible = dynamic_cast<Rogue*>(p_target)->isInvisible();
	}
	catch (...) {

	}

	if (targetIsInvisible) {
		throw std::string("data/Text/target_invisible.png");
	}

	if (Weapon::is<Bow>(*m_weapon)) {
		if (m_arrows < 1) throw std::string("data/Text/no_more_arrows.png");
	}

	return true;
}

void Rogue::attackWeapon(Character* p_target) {
	anim_weapon->play(true);
	if (p_target->dodge()) throw std::string("data/Text/target_dodged.png");
	float coeff = (rand() % 16) / 100 + 0.85f;
	p_target->takeDmg((int)(coeff * 9.0f * agi() * weapon()->base_dmg() / p_target->def()));

	if (Weapon::is<Bow>(*m_weapon)) {
		m_arrows--;
	}
	else {
		if (!Character::is<Archer>(*p_target) && poisonRoll()) p_target->poisonned(true);
		dynamic_cast<Dagger*>(m_weapon)->loseDurability();
	}

	return;
}


void Rogue::initRecap(SDL_Renderer* p_renderer) {
	std::string text = "";
	text.append(name());
	text.append("\nHP: ");
	text.append(std::to_string(m_current_health));
	text.append("/");
	text.append(std::to_string(m_max_health));
	text.append("\nATK: ");
	text.append(std::to_string(atk()));
	text.append("\nDEF: ");
	text.append(std::to_string(def()));
	text.append("\nAGI: ");
	text.append(std::to_string(agi()));
	text.append("\nINT: ");
	text.append(std::to_string(intel()));
	text.append("\nSPD: ");
	text.append(std::to_string(speed()));
	text.append("\nPoison: ");
	text.append(std::to_string(m_poisonChance));
	if (isInvisible()) {
		text.append("\nSneaky");
	}
	if (m_weapon) {
		if (Weapon::is<Bow>(*m_weapon)) {
			text.append("\nArrows: ");
			text.append(std::to_string(m_arrows));
		}
		else if (Weapon::is<Dagger>(*m_weapon)) {
			text.append("\nDurability: ");
			text.append(std::to_string(dynamic_cast<Dagger*>(m_weapon)->durability()));
		}
		if (m_weapon->isEnchanted()) {
			text.append("\nEnchanted");
		}
		if (poisonned()) {
			text.append("\nPoisonned");
		}
	}

	anim_recap = new Animation(text, p_renderer);
}

void Rogue::updateRecap() {
	SDL_Renderer* renderer = anim_recap->renderer();
	delete anim_recap;
	initRecap(renderer);
}


void Rogue::startTurn() {
	if (m_poisonned) {
		m_current_health -= m_max_health / 12;
	}

	if (m_isInvisible && m_invisibilityCurrentCooldown == m_invisibilityMaxCooldown) {
		if (rand() % 2)
			m_isInvisible = false;
	}
	else if (m_isInvisible && m_invisibilityCurrentCooldown == m_invisibilityMaxCooldown - 1) {
		m_isInvisible = false;
	}

	if (m_invisibilityCurrentCooldown > 0) m_invisibilityCurrentCooldown--;

	m_speed = 0;
}

void Rogue::render() {
	if (anim_basic && anim_basic->play())
		anim_basic->render();
	else if (anim_sneak && anim_sneak->play())
		anim_sneak->render();
	else if (anim_weapon && anim_weapon->play())
		anim_weapon->render();
	else
		anim_idle->render();
	if (anim_hp && anim_hp->play())
		anim_hp->render();
}

void Rogue::renderIcon() {
	icon_basic->render();
	icon_weapon->render();
	icon_sneak->render();
}

Rogue::~Rogue() {
	for (int i = 0; i < m_mods.size(); i++) {
		delete m_mods.at(i);
	}
	m_mods.clear();
	m_mods.~vector();
	delete m_weapon;

	delete anim_recap;
	delete anim_idle;

	delete anim_basic;
	delete icon_basic;

	delete anim_weapon;
	delete icon_weapon;
	if (anim_sneak)	delete anim_sneak;
	if (icon_sneak) delete icon_sneak;
}
