#include "Archer.h"
#include "Rogue.h"

Archer::Archer() : Character() {
	m_heal_poison_charge = 3;
	m_isAiming = false;
	m_arrows = 0;
	anim_aiming = NULL;
	icon_aiming = NULL;
	anim_healPoison = NULL;
	icon_healPoison = NULL;
}

Archer::Archer(std::string p_name, int p_max_health, int p_dodge_chance, int p_speed, int p_atk, int p_def, int p_agi, int p_intel) : Character(p_name, p_max_health, p_dodge_chance, p_speed, p_atk, p_def, p_agi, p_intel) {
	m_heal_poison_charge = 3;
	m_isAiming = false;
	m_arrows = 0;
	anim_aiming = NULL;
	icon_aiming = NULL;
	anim_healPoison = NULL;
	icon_healPoison = NULL;
}


bool Archer::isAiming() {
	return m_isAiming;
}

int Archer::poisonHealCharge() {
	return m_heal_poison_charge;
}

int Archer::arrows() {
	return m_arrows;
}

void Archer::arrows(int p_arrows) {
	m_arrows = p_arrows;
}

void Archer::castSkill(int p_pos, Character* p_target) {
	if (Weapon::is<Bow>(*m_weapon)) {
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
				canHealPoison(p_target);
				healPoison(p_target);
			}
			catch (std::string e) {
				throw e;
			}
			break;
		case 3:
			try {
				canAim();
				aim();
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
				canHealPoison(p_target);
				healPoison(p_target);
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

bool Archer::canAim() {
	if (Weapon::is<Bow>(*m_weapon)) {
		if (!m_isAiming) return true;
		else throw std::string("data/Text/already_aiming.png");
	}
	else {
		throw std::string("data/Text/can_only_aim_with_a_bow.png");
	}
}

void Archer::aim() {
	m_isAiming = true;
	anim_aiming->play(true);
	return;
}


bool Archer::canHealPoison(Character* p_target) {
	try {
		isAlly(p_target);
	}
	catch (std::string exception) {
		throw exception;
	}

	if (!m_heal_poison_charge) {
		throw std::string("data/Text/no_heal_charge.png");
		return false;
	}

	if (!p_target->poisonned()) {
		throw std::string("data/Text/target_not_poisonned.png");
		return false;
	}

	return true;
}

void Archer::healPoison(Character* p_target) {
	p_target->poisonned(false);
	m_heal_poison_charge--;
	anim_healPoison->setPos(p_target->anim_idle->get_xPos(), p_target->anim_idle->get_yPos());
	anim_healPoison->play(true);
	return;
}


bool Archer::canAttackWeapon(Character* p_target) {
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

	if (Weapon::is<Bow>(*m_weapon)) {
		if (m_arrows < 1) throw std::string("data/Text/no_more_arrows.png");
	}

	if (targetIsInvisible) {
		throw std::string("data/Text/target_invisible.png");
	}

	return true;
}

void Archer::attackWeapon(Character* p_target) {
	anim_weapon->play(true);

	if (p_target->dodge()) throw std::string("data/Text/target_invisible.png");
	float coeff = (rand() % 16) / 100 + 0.85f;

	if (isAiming()) {
		p_target->takeDmg((int)(coeff * 9.0f * 1.33f * (agi() + weapon()->base_dmg()) / p_target->def()));
		m_isAiming = false;
	}
	else {
		p_target->takeDmg((int)(coeff * 9.0f * (agi() + weapon()->base_dmg()) / p_target->def()));
	}

	if (Weapon::is<Bow>(*m_weapon)) {
		m_arrows--;
	}
	if (Weapon::is<Dagger>(*m_weapon)) {
		dynamic_cast<Dagger*>(m_weapon)->loseDurability();
	}

	return;
}

void Archer::startTurn() {
	m_speed = 0;
}


void Archer::initRecap(SDL_Renderer* p_renderer) {
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
		if (Weapon::is<Bow>(*m_weapon)) {
			text.append("\nArrows: ");
			text.append(std::to_string(m_arrows));
		}
		else if (Weapon::is<Dagger>(*m_weapon)) {
			text.append("\nDurability: ");
			text.append(std::to_string(dynamic_cast<Dagger*>(m_weapon)->durability()));
		}
		if (isAiming()) {
			text.append("\nAiming");
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

void Archer::updateRecap() {
	SDL_Renderer* renderer = anim_recap->renderer();
	delete anim_recap;
	initRecap(renderer);
}


void Archer::render() {
	if (anim_aiming && anim_aiming->play()) {
		anim_aiming->render();
	}
	else if (anim_basic && anim_basic->play()) {
		anim_basic->render();

	}
	else if (anim_healPoison && anim_healPoison->play()) {
		anim_healPoison->render();
	}
	else if (anim_weapon && anim_weapon->play()) {
		anim_weapon->render();
	}
	else {
		anim_idle->render();
	}
	if (anim_hp && anim_hp->play()) {
		anim_hp->render();
	}
}

void Archer::renderIcon() {
	icon_basic->render();
	icon_weapon->render();
	if (icon_aiming) icon_aiming->render();
	icon_healPoison->render();
}


Archer::~Archer() {
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
	if (anim_aiming) delete anim_aiming;
	if (icon_aiming) delete icon_aiming;
	if (anim_healPoison) delete anim_healPoison;
	if (icon_healPoison) delete icon_healPoison;
}