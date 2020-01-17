#include "Warrior.h"
#include "Rogue.h"

Warrior::Warrior() : Character() {
	m_parry = 0;

	m_provocation = false;
	m_poisonned = false;

	m_warcryCurrentCooldown = 0;
	m_warcryMaxCooldown = 0;

	anim_repair = NULL;
	icon_repair = NULL;
	anim_warcry = NULL;
	icon_warcry = NULL;
}

Warrior::Warrior(std::string p_name, int p_max_health, int p_dodge_chance, int p_speed, int p_atk, int p_def, int p_agi, int p_intel, int p_parry) : Character(p_name, p_max_health, p_dodge_chance, p_speed, p_atk, p_def, p_agi, p_intel) {
	m_parry = p_parry;

	m_provocation = false;
	m_poisonned = false;

	m_warcryCurrentCooldown = 0;
	m_warcryMaxCooldown = 4;

	anim_repair = NULL;
	icon_repair = NULL;
	anim_warcry = NULL;
	icon_warcry = NULL;
}


bool Warrior::provocation() {
	return m_provocation;
}

bool Warrior::parryRoll() {
	if (rand() % 100 < m_parry) return true;
	else return false;
}

void Warrior::castSkill(int p_pos, Character* p_target) {
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
			canWarcry();
			warcry();
		}
		catch (std::string e) {
			throw e;
		}
		break;
	case 3:
		try {
			canRepair(p_target);
			repair(p_target);
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

bool Warrior::canWarcry() {
	if (m_warcryCurrentCooldown > 0) return false;
	else return true;
}

void Warrior::warcry() {
	m_provocation = true;
	m_warcryCurrentCooldown = m_warcryMaxCooldown;
	anim_warcry->play(true);
	return;
}


bool Warrior::canRepair(Character* p_target) {
	try {
		isAlly(p_target);
	}
	catch (std::string exception) {
		throw exception;
	}

	MeleeWeapon* weapon = NULL;
	if (Weapon::is<Sword>(*(p_target->weapon()))) {
		weapon = dynamic_cast<Sword*>(p_target->weapon());
	}
	else if (Weapon::is<Dagger>(*(p_target->weapon()))) {
		weapon = dynamic_cast<Dagger*>(p_target->weapon());
	}
	else {
		throw std::string("data/Text/not_a_melee_weapon.png");
		return false;
	}

	if (weapon->durability() == weapon->maxDurability()) {
		throw std::string("data/Text/durability_already_at_maximum.png");
	}
	return true;
}

void Warrior::repair(Character* p_target) {
	MeleeWeapon* weapon;
	try {
		weapon = dynamic_cast<MeleeWeapon*>(p_target->weapon());
		if (weapon->durability() == 0) weapon->addDurability(1);
		else weapon->addDurability(rand() % 13 + 3);
		anim_repair->setPos(p_target->anim_idle->get_xPos(), p_target->anim_idle->get_yPos());
		anim_repair->play(true);
		return;
	}
	catch (...) {
		throw std::string("data/Text/not_a_melee_weapon.png");
		return;
	}
}


bool Warrior::canAttackWeapon(Character* p_target) {
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
		return true;
	}

	if (targetIsInvisible) {
		throw std::string("data/Text/target_invisible.png");
	}
	return true;
}

void Warrior::attackWeapon(Character* p_target) {
	anim_weapon->play(true);
	if (p_target->dodge()) throw std::string("data/Text/target_dodged.png");
	float coeff = (rand() % 16) / 100 + 0.85f;
	p_target->takeDmg((int)(coeff * 7.0f * (atk() + weapon()->base_dmg()) / p_target->def()));

	if (Weapon::is<Dagger>(*m_weapon)) {
		dynamic_cast<Dagger*>(m_weapon)->loseDurability();
	}
	else {
		dynamic_cast<Sword*>(m_weapon)->loseDurability();
	}

	return;
}


void Warrior::initRecap(SDL_Renderer* p_renderer) {
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
	if (m_weapon) {
		if (Weapon::is<Sword>(*m_weapon)) {
			text.append("\nDurability: ");
			text.append(std::to_string(dynamic_cast<Sword*>(m_weapon)->durability()));
			text.append("\nParry: ");
			text.append(std::to_string(m_parry));
		}
		else if (Weapon::is<Dagger>(*m_weapon)) {
			text.append("\nDurability: ");
			text.append(std::to_string(dynamic_cast<Dagger*>(m_weapon)->durability()));
		}
		if (provocation()) {
			text.append("\nProvocative");
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

void Warrior::updateRecap() {
	SDL_Renderer* renderer = anim_recap->renderer();
	delete anim_recap;
	initRecap(renderer);
}

void Warrior::render() {
	if (anim_basic && anim_basic->play())
		anim_basic->render();
	else if (anim_repair && anim_repair->play()) {
		anim_repair->render();
		anim_idle->render();
	}
	else if (anim_warcry && anim_warcry->play())
		anim_warcry->render();
	else if (anim_weapon && anim_weapon->play())
		anim_weapon->render();
	else
		anim_idle->render();
	if (anim_hp && anim_hp->play())
		anim_hp->render();
}

void Warrior::renderIcon() {
	icon_basic->render();
	icon_weapon->render();
	icon_repair->render();
	icon_warcry->render();
}


void Warrior::startTurn() {
	if (m_poisonned) {
		m_current_health -= m_max_health / 12;
	}

	if (m_warcryCurrentCooldown > 0) m_warcryCurrentCooldown--;

	if (m_provocation && m_warcryCurrentCooldown == m_warcryMaxCooldown) {
		if (rand() % 2)
			m_provocation = false;
	}
	else if (m_provocation && m_warcryCurrentCooldown == m_warcryMaxCooldown - 1) {
		m_provocation = false;
	}

	m_speed = 0;
}

Warrior::~Warrior() {
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
	if (anim_repair) delete anim_repair;
	if (icon_repair) delete icon_repair;
	if (anim_warcry) delete anim_warcry;
	if (icon_warcry) delete icon_warcry;
}