#include "Character.h"

#include "Archer.h"
#include "Rogue.h"
#include "Warrior.h"
#include "Wizard.h"

Character::Character() {
	m_name = "";
	m_max_health = 0;
	m_current_health = 0;
	m_dodge_chance = 0;
	m_baseSpeed = 0;
	m_speed = 0;
	m_atk = 0;
	m_def = 0;
	m_agi = 0;
	m_intel = 0;

}

Character::Character(std::string p_name, int p_max_health, int p_dodge_chance, int p_speed, int p_atk, int p_def, int p_agi, int p_intel) {
	m_name = p_name;
	m_max_health = p_max_health;
	m_current_health = p_max_health;
	m_dodge_chance = p_dodge_chance;
	m_baseSpeed = p_speed;
	m_speed = p_speed;
	m_atk = p_atk;
	m_def = p_def;
	m_agi = p_agi;
	m_intel = p_intel;

	for (int i = 0; i < NB_MODS; i++) {
		m_mods.push_back(new Modifier(static_cast<modifier_id>(i), 1.0f));
	}
}

//Setters
void Character::poisonned(bool p_poison) {
	m_poisonned = p_poison;
}

void Character::dodge_chance(int p_dodge_chance) {
	m_dodge_chance = p_dodge_chance;
}

void Character::speed(int p_speed) {
	m_speed = p_speed;
}

void Character::addSpeed(int p_speed) {
	m_speed += p_speed;
}

void Character::atk(int p_atk) {
	m_atk = p_atk;
}

void Character::def(int p_def) {
	m_def = p_def;
}

void Character::agi(int p_agi) {
	m_agi = p_agi;
}

void Character::intel(int p_intel) {
	m_intel = p_intel;
}

void Character::setMaxHealth(int p_health) {
	m_max_health = p_health;
	m_current_health = p_health;
}

std::string Character::name() {
	return m_name;
}


//Getters
int Character::health() {
	return m_current_health;
}

bool Character::poisonned() {
	return m_poisonned;
}

int Character::dodge_chance() {
	return (int)m_dodge_chance * (int)m_mods.at(DODGE)->value();
}

int Character::baseSpeed() {
	return m_baseSpeed;
}

int Character::speed() {
	return (int)m_speed * (int)m_mods.at(SPEED)->value();
}

int Character::atk() {
	return(int)m_atk * (int)m_mods.at(ATK)->value();
}

int Character::def() {
	return (int)m_def * (int)m_mods.at(DEF)->value();
}

int Character::agi() {
	return (int)m_agi * (int)m_mods.at(AGI)->value();
}

int Character::intel() {
	return (int)m_intel * (int)m_mods.at(INT)->value();
}

Animation* Character::animation() {
	return anim_idle;
}

void Character::isAlly(Character* p_target) {
	if (p_target->animation()->getSide() != animation()->getSide()) {
		throw std::string("data/Text/target_is_not_an_ally.png");
	}
}

void Character::isEnnemy(Character* p_target) {
	if (p_target->animation()->getSide() == animation()->getSide()) {
		throw std::string("data/Text/target_is_not_an_ennemy.png");
	}
}

void Character::isSelf(Character* p_target) {
	if (p_target != this) {
		throw std::string("data/Text/target_is_not_the_caster.png");
	}
}


//Health management
void Character::takeDmg(int p_dmg) {
	m_current_health -= p_dmg;
	if (anim_hp) {
		delete anim_hp;
	}
	anim_hp = new Animation(std::string("-").append(std::to_string(p_dmg)), anim_idle->renderer(), 40);
	anim_hp->setPos(anim_idle->get_xPos() + 140, anim_idle->get_yPos() - 50);
	anim_hp->play(true);
}

void Character::heal(int p_heal) {
	m_current_health += p_heal;
	if (m_current_health > m_max_health) m_current_health = m_max_health; 
	if (anim_hp) {
		delete anim_hp;
	}
	anim_hp = new Animation(std::string("+").append(std::to_string(p_heal)), anim_idle->renderer(), 40);
	anim_hp->setPos(anim_idle->get_xPos() + 140, anim_idle->get_yPos() - 50);
	anim_hp->play(true);
}

bool Character::isDead() {
	if (m_current_health <= 0) return true;
	else return false;
}


//Turn management
void Character::startTurn() {

}

void Character::castSkill(int p_pos, Character* p_target) {

}

//Weapon management
bool Character::equip(Weapon* p_weapon) {
	if (Character::is<Archer>(*this)) {
		Archer* tmp_archer = dynamic_cast<Archer*>(this);
		if (Weapon::is<Bow>(*p_weapon)) {
			m_weapon = p_weapon;
			tmp_archer->setMaxHealth(tmp_archer->health() + m_weapon->health());
			tmp_archer->atk(tmp_archer->atk() + m_weapon->attack());
			tmp_archer->arrows(dynamic_cast<Bow*>(m_weapon)->m_arrows);
			tmp_archer->updateRecap();

			tmp_archer->animation()->changeTexture("data/Characters/Archer/archer_bow_idle.png");

			tmp_archer->anim_basic = new Animation("data/Characters/Archer/archer_bow_basic.png", tmp_archer->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_archer->animation()->get_xPos(), tmp_archer->animation()->get_yPos(), tmp_archer->animation()->getSide());
			tmp_archer->icon_basic = new Animation("data/Characters/Archer/basic_attack.png", tmp_archer->animation()->renderer(), 1, 0, true, true, 64, 64, 25, 26, SIDE::LEFT);

			tmp_archer->anim_weapon = new Animation("data/Characters/Archer/archer_bow_weapon.png", tmp_archer->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_archer->animation()->get_xPos(), tmp_archer->animation()->get_yPos(), tmp_archer->animation()->getSide());
			tmp_archer->icon_weapon = new Animation("data/Characters/Archer/weapon_attack.png", tmp_archer->animation()->renderer(), 1, 0, true, true, 64, 64, 141, 26, SIDE::LEFT);

			tmp_archer->anim_healPoison = new Animation("data/Characters/heal_poison.png", tmp_archer->animation()->renderer(), 25, 0, false, false, 320, 320, 0, 0, SIDE::LEFT);
			tmp_archer->icon_healPoison = new Animation("data/Characters/Archer/archer_heal_poison.png", tmp_archer->animation()->renderer(), 1, 0, true, true, 64, 64, 257, 26, SIDE::LEFT);

			tmp_archer->anim_aiming = new Animation("data/Characters/Archer/archer_bow_aiming.png", tmp_archer->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_archer->animation()->get_xPos(), tmp_archer->animation()->get_yPos(), tmp_archer->animation()->getSide());
			tmp_archer->icon_aiming = new Animation("data/Characters/Archer/archer_aim.png", tmp_archer->animation()->renderer(), 1, 0, true, true, 64, 64, 363, 26, SIDE::LEFT);

			
			return true;
		}
		else if (Weapon::is<Dagger>(*p_weapon)) {
			m_weapon = p_weapon;
			tmp_archer->setMaxHealth(tmp_archer->health() + m_weapon->health());
			tmp_archer->atk(tmp_archer->atk() + m_weapon->attack());
			tmp_archer->def(tmp_archer->def() + dynamic_cast<Dagger*>(m_weapon)->m_defense);
			tmp_archer->agi(tmp_archer->agi() + dynamic_cast<Dagger*>(m_weapon)->m_agility);
			tmp_archer->updateRecap();

			tmp_archer->animation()->changeTexture("data/Characters/Archer/archer_dagger_idle.png");

			tmp_archer->anim_basic = new Animation("data/Characters/Archer/archer_dagger_basic.png", tmp_archer->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_archer->animation()->get_xPos(), tmp_archer->animation()->get_yPos(), tmp_archer->animation()->getSide());
			tmp_archer->icon_basic = new Animation("data/Characters/Archer/basic_attack.png", tmp_archer->animation()->renderer(), 1, 0, true, true, 64, 64, 25, 26, SIDE::LEFT);

			tmp_archer->anim_weapon = new Animation("data/Characters/Archer/archer_dagger_weapon.png", tmp_archer->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_archer->animation()->get_xPos(), tmp_archer->animation()->get_yPos(), tmp_archer->animation()->getSide());
			tmp_archer->icon_weapon = new Animation("data/Characters/Archer/weapon_attack.png", tmp_archer->animation()->renderer(), 1, 0, true, true, 64, 64, 141, 26, SIDE::LEFT);

			tmp_archer->anim_healPoison = new Animation("data/Characters/heal_poison.png", tmp_archer->animation()->renderer(), 25, 0, false, false, 320, 320, 0, 0, SIDE::LEFT);
			tmp_archer->icon_healPoison = new Animation("data/Characters/Archer/archer_heal_poison.png", tmp_archer->animation()->renderer(), 1, 0, true, true, 64, 64, 257, 26, SIDE::LEFT);

			return true;
		}
		else return false;
	}
	else if (Character::is<Rogue>(*this)) {
		Rogue* tmp_rogue = dynamic_cast<Rogue*>(this);
		if (Weapon::is<Bow>(*p_weapon)) {
			m_weapon = p_weapon;
			tmp_rogue->setMaxHealth(tmp_rogue->health() + m_weapon->health());
			tmp_rogue->atk(tmp_rogue->atk() + m_weapon->attack());
			tmp_rogue->arrows(dynamic_cast<Bow*>(m_weapon)->m_arrows);
			tmp_rogue->updateRecap();

			tmp_rogue->animation()->changeTexture("data/Characters/Rogue/rogue_bow_idle.png");

			tmp_rogue->anim_basic = new Animation("data/Characters/Rogue/rogue_bow_basic.png", tmp_rogue->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_rogue->animation()->get_xPos(), tmp_rogue->animation()->get_yPos(), tmp_rogue->animation()->getSide());
			tmp_rogue->icon_basic = new Animation("data/Characters/Rogue/basic_attack.png", tmp_rogue->animation()->renderer(), 1, 0, true, true, 64, 64, 25, 26, SIDE::LEFT);

			tmp_rogue->anim_weapon = new Animation("data/Characters/Rogue/rogue_bow_weapon.png", tmp_rogue->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_rogue->animation()->get_xPos(), tmp_rogue->animation()->get_yPos(), tmp_rogue->animation()->getSide());
			tmp_rogue->icon_weapon = new Animation("data/Characters/Rogue/weapon_attack.png", tmp_rogue->animation()->renderer(), 1, 0, true, true, 64, 64, 141, 26, SIDE::LEFT);

			tmp_rogue->anim_sneak = new Animation("data/Characters/Rogue/rogue_bow_sneak.png", tmp_rogue->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_rogue->animation()->get_xPos(), tmp_rogue->animation()->get_yPos(), tmp_rogue->animation()->getSide());
			tmp_rogue->icon_sneak = new Animation("data/Characters/Rogue/rogue_sneak.png", tmp_rogue->animation()->renderer(), 1, 0, true, true, 64, 64, 257, 26, SIDE::LEFT);

			return true;
		}
		else if (Weapon::is<Dagger>(*p_weapon)) {
			m_weapon = p_weapon;
			tmp_rogue->setMaxHealth(tmp_rogue->health() + m_weapon->health());
			tmp_rogue->atk(tmp_rogue->atk() + m_weapon->attack());
			tmp_rogue->def(tmp_rogue->def() + dynamic_cast<Dagger*>(m_weapon)->m_defense);
			tmp_rogue->agi(tmp_rogue->agi() + dynamic_cast<Dagger*>(m_weapon)->m_agility);
			tmp_rogue->updateRecap();

			tmp_rogue->animation()->changeTexture("data/Characters/Rogue/rogue_dagger_idle.png");

			tmp_rogue->anim_basic = new Animation("data/Characters/Rogue/rogue_dagger_basic.png", tmp_rogue->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_rogue->animation()->get_xPos(), tmp_rogue->animation()->get_yPos(), tmp_rogue->animation()->getSide());
			tmp_rogue->icon_basic = new Animation("data/Characters/Rogue/basic_attack.png", tmp_rogue->animation()->renderer(), 1, 0, true, true, 64, 64, 25, 26, SIDE::LEFT);

			tmp_rogue->anim_weapon = new Animation("data/Characters/Rogue/rogue_dagger_weapon.png", tmp_rogue->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_rogue->animation()->get_xPos(), tmp_rogue->animation()->get_yPos(), tmp_rogue->animation()->getSide());
			tmp_rogue->icon_weapon = new Animation("data/Characters/Rogue/weapon_attack.png", tmp_rogue->animation()->renderer(), 1, 0, true, true, 64, 64, 141, 26, SIDE::LEFT);

			tmp_rogue->anim_sneak = new Animation("data/Characters/Rogue/rogue_dagger_sneak.png", tmp_rogue->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_rogue->animation()->get_xPos(), tmp_rogue->animation()->get_yPos(), tmp_rogue->animation()->getSide());
			tmp_rogue->icon_sneak = new Animation("data/Characters/Rogue/rogue_sneak.png", tmp_rogue->animation()->renderer(), 1, 0, true, true, 64, 64, 257, 26, SIDE::LEFT);

			return true;
		}
		else return false;
	}
	else if (Character::is<Warrior>(*this)) {
		Warrior* tmp_warrior = dynamic_cast<Warrior*>(this);
		if (Weapon::is<Sword>(*p_weapon)) {
			m_weapon = p_weapon;
			tmp_warrior->setMaxHealth(tmp_warrior->health() + m_weapon->health());
			tmp_warrior->atk(tmp_warrior->atk() + m_weapon->attack());
			tmp_warrior->updateRecap();

			tmp_warrior->animation()->changeTexture("data/Characters/Warrior/warrior_sword_idle.png");

			tmp_warrior->anim_basic = new Animation("data/Characters/Warrior/warrior_sword_basic.png", tmp_warrior->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_warrior->animation()->get_xPos(), tmp_warrior->animation()->get_yPos(), tmp_warrior->animation()->getSide());
			tmp_warrior->icon_basic = new Animation("data/Characters/Warrior/basic_attack.png", tmp_warrior->animation()->renderer(), 1, 0, true, true, 64, 64, 25, 26, SIDE::LEFT);

			tmp_warrior->anim_weapon = new Animation("data/Characters/Warrior/warrior_sword_weapon.png", tmp_warrior->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_warrior->animation()->get_xPos(), tmp_warrior->animation()->get_yPos(), tmp_warrior->animation()->getSide());
			tmp_warrior->icon_weapon = new Animation("data/Characters/Warrior/weapon_attack.png", tmp_warrior->animation()->renderer(), 1, 0, true, true, 64, 64, 141, 26, SIDE::LEFT);

			tmp_warrior->anim_warcry = new Animation("data/Characters/Warrior/warrior_sword_provocation.png", tmp_warrior->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_warrior->animation()->get_xPos(), tmp_warrior->animation()->get_yPos(), tmp_warrior->animation()->getSide());
			tmp_warrior->icon_warcry = new Animation("data/Characters/Warrior/warrior_provocation.png", tmp_warrior->animation()->renderer(), 1, 0, true, true, 64, 64, 257, 26, SIDE::LEFT);

			tmp_warrior->anim_repair = new Animation("data/Characters/weapon_repair.png", tmp_warrior->animation()->renderer(), 25, 0, false, false, 320, 320, 0, 0, SIDE::LEFT);
			tmp_warrior->icon_repair = new Animation("data/Characters/Warrior/warrior_repair.png", tmp_warrior->animation()->renderer(), 1, 0, true, true, 64, 64, 363, 26, SIDE::LEFT);

			return true;
		}
		else if (Weapon::is<Dagger>(*p_weapon)) {
			m_weapon = p_weapon;
			tmp_warrior->setMaxHealth(tmp_warrior->health() + m_weapon->health());
			tmp_warrior->atk(tmp_warrior->atk() + m_weapon->attack());
			tmp_warrior->def(tmp_warrior->def() + dynamic_cast<Dagger*>(m_weapon)->m_defense);
			tmp_warrior->agi(tmp_warrior->agi() + dynamic_cast<Dagger*>(m_weapon)->m_agility);
			tmp_warrior->updateRecap();

			tmp_warrior->animation()->changeTexture("data/Characters/Warrior/warrior_dagger_idle.png");

			tmp_warrior->anim_basic = new Animation("data/Characters/Warrior/warrior_dagger_basic.png", tmp_warrior->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_warrior->animation()->get_xPos(), tmp_warrior->animation()->get_yPos(), tmp_warrior->animation()->getSide());
			tmp_warrior->icon_basic = new Animation("data/Characters/Warrior/basic_attack.png", tmp_warrior->animation()->renderer(), 1, 0, true, true, 64, 64, 25, 26, SIDE::LEFT);

			tmp_warrior->anim_weapon = new Animation("data/Characters/Warrior/warrior_dagger_weapon.png", tmp_warrior->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_warrior->animation()->get_xPos(), tmp_warrior->animation()->get_yPos(), tmp_warrior->animation()->getSide());
			tmp_warrior->icon_weapon = new Animation("data/Characters/Warrior/weapon_attack.png", tmp_warrior->animation()->renderer(), 1, 0, true, true, 64, 64, 141, 26, SIDE::LEFT);

			tmp_warrior->anim_warcry = new Animation("data/Characters/Warrior/warrior_dagger_provocation.png", tmp_warrior->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_warrior->animation()->get_xPos(), tmp_warrior->animation()->get_yPos(), tmp_warrior->animation()->getSide());
			tmp_warrior->icon_repair = new Animation("data/Characters/Warrior/warrior_provocation.png", tmp_warrior->animation()->renderer(), 1, 0, true, true, 64, 64, 257, 26, SIDE::LEFT);

			tmp_warrior->anim_repair = new Animation("data/Characters/weapon_repair.png", tmp_warrior->animation()->renderer(), 25, 0, false, false, 320, 320, 0, 0, SIDE::LEFT);
			tmp_warrior->icon_repair = new Animation("data/Characters/Warrior/warrior_repair.png", tmp_warrior->animation()->renderer(), 1, 0, true, true, 64, 64, 363, 26, SIDE::LEFT);

			return true;
		}
		else return false;
	}
	else if (Character::is<Wizard>(*this)) {
		Wizard* tmp_wizard = dynamic_cast<Wizard*>(this);
		if (Weapon::is<Sword>(*p_weapon)) {
			m_weapon = p_weapon;
			tmp_wizard->setMaxHealth(tmp_wizard->health() + m_weapon->health());
			tmp_wizard->atk(tmp_wizard->atk() + m_weapon->attack());
			tmp_wizard->updateRecap();

			tmp_wizard->animation()->changeTexture("data/Characters/Wizard/wizard_sword_idle.png");

			tmp_wizard->anim_basic = new Animation("data/Characters/Wizard/wizard_sword_basic.png", tmp_wizard->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_wizard->animation()->get_xPos(), tmp_wizard->animation()->get_yPos(), tmp_wizard->animation()->getSide());
			tmp_wizard->icon_basic = new Animation("data/Characters/Wizard/basic_attack.png", tmp_wizard->animation()->renderer(), 1, 0, true, true, 64, 64, 25, 26, SIDE::LEFT);

			tmp_wizard->anim_weapon = new Animation("data/Characters/Wizard/wizard_sword_weapon.png", tmp_wizard->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_wizard->animation()->get_xPos(), tmp_wizard->animation()->get_yPos(), tmp_wizard->animation()->getSide());
			tmp_wizard->icon_weapon = new Animation("data/Characters/Wizard/weapon_attack.png", tmp_wizard->animation()->renderer(), 1, 0, true, true, 64, 64, 141, 26, SIDE::LEFT);

			tmp_wizard->anim_enchant = new Animation("data/Characters/weapon_enchant.png", tmp_wizard->animation()->renderer(), 25, 0, false, false, 320, 320, 0, 0, SIDE::LEFT);
			tmp_wizard->icon_enchant = new Animation("data/Characters/Wizard/wizard_enchant.png", tmp_wizard->animation()->renderer(), 1, 0, true, true, 64, 64, 257, 26, SIDE::LEFT);

			tmp_wizard->anim_regenMana = new Animation("data/Characters/mana_regen.png", tmp_wizard->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_wizard->animation()->get_xPos(), tmp_wizard->animation()->get_yPos(), SIDE::LEFT);
			tmp_wizard->icon_regenMana = new Animation("data/Characters/Wizard/wizard_mana.png", tmp_wizard->animation()->renderer(), 1, 0, true, true, 64, 64, 363, 26, SIDE::LEFT);

			return true;
		}
		else if (Weapon::is<Staff>(*p_weapon)) {
			m_weapon = p_weapon;
			tmp_wizard->setMaxHealth(tmp_wizard->health() + m_weapon->health());
			tmp_wizard->atk(tmp_wizard->atk() + m_weapon->attack());
			tmp_wizard->intel(tmp_wizard->intel() + dynamic_cast<Staff*>(m_weapon)->m_intelligence);
			tmp_wizard->updateRecap();

			tmp_wizard->animation()->changeTexture("data/Characters/Wizard/wizard_staff_idle.png");

			tmp_wizard->anim_basic = new Animation("data/Characters/Wizard/wizard_staff_basic.png", tmp_wizard->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_wizard->animation()->get_xPos(), tmp_wizard->animation()->get_yPos(), tmp_wizard->animation()->getSide());
			tmp_wizard->icon_basic = new Animation("data/Characters/Wizard/basic_attack.png", tmp_wizard->animation()->renderer(), 1, 0, true, true, 64, 64, 25, 26, SIDE::LEFT);

			tmp_wizard->anim_weapon = new Animation("data/Characters/Wizard/wizard_staff_weapon.png", tmp_wizard->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_wizard->animation()->get_xPos(), tmp_wizard->animation()->get_yPos(), tmp_wizard->animation()->getSide());
			tmp_wizard->icon_weapon = new Animation("data/Characters/Wizard/weapon_attack.png", tmp_wizard->animation()->renderer(), 1, 0, true, true, 64, 64, 141, 26, SIDE::LEFT);

			tmp_wizard->anim_enchant = new Animation("data/Characters/weapon_enchant.png", tmp_wizard->animation()->renderer(), 25, 0, false, false, 320, 320, 0, 0, SIDE::LEFT);
			tmp_wizard->icon_enchant = new Animation("data/Characters/Wizard/wizard_enchant.png", tmp_wizard->animation()->renderer(), 1, 0, true, true, 64, 64, 257, 26, SIDE::LEFT);

			tmp_wizard->anim_regenMana = new Animation("data/Characters/mana_regen.png", tmp_wizard->animation()->renderer(), 25, 0, false, false, 320, 320, tmp_wizard->animation()->get_xPos(), tmp_wizard->animation()->get_yPos(), SIDE::LEFT);
			tmp_wizard->icon_regenMana = new Animation("data/Characters/Wizard/wizard_mana.png", tmp_wizard->animation()->renderer(), 1, 0, true, true, 64, 64, 363, 26, SIDE::LEFT);

			tmp_wizard->anim_heal = new Animation("data/Characters/heal.png", tmp_wizard->animation()->renderer(), 25, 0, false, false, 320, 320, 0, 0, SIDE::LEFT);
			tmp_wizard->icon_heal = new Animation("data/Characters/Wizard/wizard_heal.png", tmp_wizard->animation()->renderer(), 1, 0, true, true, 64, 64, 476, 26, SIDE::LEFT);

			return true;
		}
		else return false;
	}
	else return false;
}

Weapon* Character::weapon() {
	return m_weapon;
}


//Mods management
std::vector<Modifier*> Character::mods() {
	return m_mods;
}

Modifier* Character::mods(modifier_id mod_id) {
	return m_mods.at(mod_id);
}


//Dodge management
bool Character::dodge() {
	if (rand() % 101 < m_dodge_chance) return true;
	else return false;
}


//Default attack
bool Character::canAttack(Character* p_target) {
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
	catch (std::exception bad_cast) {

	}

	if (targetIsInvisible) {
		throw std::string("data/Text/target_invisible.png");
	}
	return true;
}

void Character::attack(Character* p_target) {
	anim_basic->play(true);
	if (p_target->dodge()) throw std::string("data/Text/target_dodged.png");
	float coeff = (rand() % 11) / 100 + 0.95f;
	p_target->takeDmg((int)(coeff * 16.0f * atk() / p_target->def()));
	return;
}


//Render management
void Character::initAnimation(std::string p_path, SDL_Renderer* p_renderer, int p_length, int p_currentFrame, bool p_loop, bool p_play, int x_spriteSize, int y_spriteSize, int x_spritePos, int y_spritePos, SIDE p_side) {
	try {
		anim_idle = new Animation(p_path, p_renderer, p_length, p_currentFrame, p_loop, p_play, x_spriteSize, y_spriteSize, x_spritePos, y_spritePos, p_side);
	}
	catch (...) {
		std::cout << "Couldn't load image at " << p_path << "\n";
	}
}

void Character::initRecap(SDL_Renderer* p_renderer) {
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

	anim_recap = new Animation(text, p_renderer);
}

void Character::updateRecap() {
	SDL_Renderer* renderer = anim_recap->renderer();
	delete anim_recap;
	initRecap(renderer);
}

void Character::render() {

}

void Character::renderRecap(int x_pos, int y_pos) {
	anim_recap->setPos(x_pos, y_pos);
	anim_recap->render();
}

void Character::renderIcon() {

}

//Destroyer
Character::~Character() {
	
}