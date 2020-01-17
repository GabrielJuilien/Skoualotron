#include "Wizard.h"
#include "Rogue.h"


Wizard::Wizard() : Character() {
	m_max_mana = 0;
	m_current_mana = 0;

	m_healMaxCooldown = 0;
	m_healCurrentCooldown = 0;

	m_enchantMaxCooldown = 0;
	m_enchantCurrentCooldown = 0;

	anim_enchant = NULL;
	icon_enchant = NULL;
	anim_heal = NULL;
	icon_heal = NULL;
	anim_regenMana = NULL;
	icon_regenMana = NULL;
}

Wizard::Wizard(std::string p_name, int p_max_health, int p_dodge_chance, int p_speed, int p_atk, int p_def, int p_agi, int p_intel) : Character(p_name, p_max_health, p_dodge_chance, p_speed, p_atk, p_def, p_agi, p_intel) {
	m_max_mana = 10 * p_intel;
	m_current_mana = m_max_mana;

	m_healMaxCooldown = 4;
	m_healCurrentCooldown = 0;

	m_enchantMaxCooldown = 3;
	m_enchantCurrentCooldown = 0;

	anim_enchant = NULL;
	icon_enchant = NULL;
	anim_heal = NULL;
	icon_heal = NULL;
	anim_regenMana = NULL;
	icon_regenMana = NULL;
}


int Wizard::mana() {
	return m_current_mana;
}

bool Wizard::loseMana(int p_amount) {
	if (p_amount > m_current_mana) return false;
	m_current_mana -= p_amount;
	return true;
}

void Wizard::castSkill(int p_pos, Character* p_target) {
	if (Weapon::is<Sword>(*m_weapon)) {
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
				canEnchant(p_target);
				enchant(p_target);
			}
			catch (std::string e) {
				throw e;
			}
			break;
		case 3:
			try {
				canRegenMana();
				regenMana();
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
	else {
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
				canEnchant(p_target);
				enchant(p_target);
			}
			catch (std::string e) {
				throw e;
			}
			break;
		case 3:
			try {
				canRegenMana();
				regenMana();
			}
			catch (std::string e) {
				throw e;
			}
			break;
		case 4:
			try {
				canHeal(p_target);
				heal(p_target);
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
}

bool Wizard::canHeal(Character* p_target) {

	try {
		isAlly(p_target);
	}
	catch (std::string exception) {
		throw exception;
	}

	if (!Weapon::is<Staff>(*m_weapon))throw std::string("data/Text/can_only_heal_with_a_staff.png");

	if (m_healCurrentCooldown > 0) throw std::string("data/Text/spell_on_cooldown.png");

	if (mana() < 5) throw std::string("data/Text/not_enough_mana.png");

	return true;
}

void Wizard::heal(Character* p_target) {
	p_target->heal(rand() % 11 + 10);
	loseMana(5);
	m_healCurrentCooldown = m_healMaxCooldown;
	anim_heal->setPos(p_target->anim_idle->get_xPos(), p_target->anim_idle->get_yPos());
	anim_heal->play(true);
}


bool Wizard::canEnchant(Character* p_target) {
	try {
		isAlly(p_target);
	}
	catch (std::string exception) {
		throw exception;
	}

	if (m_enchantCurrentCooldown > 0) throw std::string("data/Text/spell_on_cooldown.png");
	
	if (p_target->weapon()->isEnchanted()) throw std::string("data/Text/already_enchanted.png");

	if (mana() < 7) throw std::string("data/Text/not_enough_mana.png");

	return true;
}

void Wizard::enchant(Character* p_target) {
	p_target->weapon()->isEnchanted(true);
	loseMana(7);
	m_enchantCurrentCooldown = m_enchantMaxCooldown;
	anim_enchant->setPos(p_target->anim_idle->get_xPos(), p_target->anim_idle->get_yPos());
	anim_enchant->play(true);
}


bool Wizard::canRegenMana() {
	if (m_current_mana == m_max_mana) throw std::string("data/Text/mana_already_at_maximum.png");

	return true;
}

void Wizard::regenMana() {
	m_current_mana += rand() % 6 + 2;
	if (m_current_mana > m_max_mana) m_current_mana = m_max_mana;
	anim_regenMana->play(true);
}


bool Wizard::canAttackWeapon(Character* p_target) {
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

void Wizard::attackWeapon(Character* p_target) {
	anim_weapon->play(true);
	if (p_target->dodge()) throw std::string("data/Text/target_dodged.png");
	float coeff = (rand() % 16) / 100 + 0.85f;
	p_target->takeDmg((int)(coeff * 5.0f * intel() * weapon()->base_dmg() / p_target->def()));

	if (Weapon::is<Sword>(*m_weapon)) {
		dynamic_cast<Sword*>(m_weapon)->loseDurability();
	}

	return;
}


void Wizard::startTurn() {
	if (m_poisonned) {
		m_current_health -= m_max_health / 12;
	}

	if (m_healCurrentCooldown > 0) m_healCurrentCooldown--;

	if (m_enchantCurrentCooldown > 0) m_enchantCurrentCooldown--;

	m_speed = 0;
}


void Wizard::initRecap(SDL_Renderer* p_renderer) {
	std::string text = "";
	text.append(name());
	text.append("\nHP: ");
	text.append(std::to_string(m_current_health));
	text.append("/");
	text.append(std::to_string(m_max_health));
	text.append("\nMana: ");
	text.append(std::to_string(m_current_mana));
	text.append("/");
	text.append(std::to_string(m_max_mana));
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
		if (Weapon::is<Staff>(*m_weapon)) {
			text.append("\nCost: ");
			text.append(std::to_string(dynamic_cast<Staff*>(m_weapon)->m_cost));
		}
		else if (Weapon::is<Sword>(*m_weapon)) {
			text.append("\nDurability: ");
			text.append(std::to_string(dynamic_cast<Sword*>(m_weapon)->durability()));
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

void Wizard::updateRecap() {
	SDL_Renderer* renderer = anim_recap->renderer();
	delete anim_recap;
	initRecap(renderer);
}


void Wizard::render() {
	if (anim_basic && anim_basic->play())
		anim_basic->render();
	else if (anim_enchant && anim_enchant->play()) {
		anim_enchant->render();
		anim_idle->render();
	}
	else if (anim_heal && anim_heal->play()) {
		anim_heal->render();
		anim_idle->render();
	}
	else if (anim_regenMana && anim_regenMana->play()) {
		anim_regenMana->render();
		anim_idle->render();
	}
	else if (anim_weapon && anim_weapon->play())
		anim_weapon->render();
	else
		anim_idle->render();
	if (anim_hp && anim_hp->play())
		anim_hp->render();
}

void Wizard::renderIcon() {
	icon_basic->render();
	icon_weapon->render();
	icon_regenMana->render();
	icon_enchant->render();
	if (icon_heal) icon_heal->render();
}


Wizard::~Wizard() {
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
	if (anim_enchant) delete anim_enchant;
	if (icon_enchant) delete icon_enchant;
	if (anim_heal) delete anim_heal;
	if (icon_heal) delete icon_heal;
	if (anim_regenMana)	delete anim_regenMana;
	if (icon_regenMana) delete icon_regenMana;
}