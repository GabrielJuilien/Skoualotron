#include "Spell.h"
#include "Warrior.h"
#include "Archer.h"
#include "Wizard.h"

Spell::Spell() : Skill() {
	m_cooldown = 0;
	m_current_cooldown = 0;
	m_spell_id = static_cast<spell_id>(0);
	m_target_type = static_cast<target_type>(0);
}

Spell::Spell(int p_cooldown, spell_id p_spell_id, target_type p_target_type) : Skill() {
	m_cooldown = p_cooldown;
	m_current_cooldown = 0;
	m_spell_id = p_spell_id;
	m_target_type = p_target_type;
}

bool Spell::cast() {
	switch (m_spell_id)
	{
	case WARRIOR_WARCRY:
		try {
			dynamic_cast<Warrior*>(m_source)->provocation(true);
			return true;
		}
		catch (std::exception const& e) {
			std::cout << "Warcry can only be cast by a warrior.\n";
			return false;
		}
		break;
	case WARRIOR_REPAIR:
		try {
			dynamic_cast<Warrior*>(m_source);
			try {
				dynamic_cast<MeleeWeapon*>(m_target->weapon())->addDurability(rand() % 14 + 2);
				return true;
			}
			catch (std::exception const& e) {
				std::cout << "Repair can only be cast on a melee weapon\n";
				return false;
			}
		}
		catch (std::exception const& e) {
			std::cout << "Repair can only be cast by a warrior.\n";
			return false;
		}
		break;
	case ARCHER_AIM:
		try {
			if (dynamic_cast<Bow*>(dynamic_cast<Archer*>(m_source)->weapon()) && !dynamic_cast<Archer*>(m_source)->isAiming()) { //L'archer utilise un arc et n'est pas en train de viser
				dynamic_cast<Archer*>(m_source)->isAiming(true);
				return true;
			}
			else {
				std::cout << "Aim skill can only be used by an archer using a bow, and not already aiming.\n";
				return false;
			}
		}
		catch (std::exception const& e) {
			std::cout << "Aim skill can only be used by an archer with a bow.\n";
			return false;
		}
		break;
	case ARCHER_HEAL_POISON:
		try {
			if (dynamic_cast<Archer*>(m_source)->poisonHealCharge() > 0) {
				if (m_target->poisonned()) {
					m_target->poisonned(false);
					dynamic_cast<Archer*>(m_source)->removePoisonHealCharge();
					return true;
				}
				else {
					std::cout << "Target is not poisonned.\n";
					return false;
				}
			}
		}
		catch (std::exception const& e) {
			std::cout << "Heal poison spell can only be cast by an archer.\n";
			return false;
		}
		break;
	case WIZARD_REGENERATE_MANA:
		try {
			dynamic_cast<Wizard*>(m_source)->regenMana(rand() % 4 + 2);
			return true;
		}
		catch (std::exception const& e) {
			std::cout << "Only a wizard can regenerate its mana.\n";
			return false;
		}
		break;
	case WIZARD_HEAL:
		try {
			Wizard* wzrd = dynamic_cast<Wizard*>(m_source); //Check if source is a wizard
			try {
				dynamic_cast<Staff*>(m_source->weapon());
			}
			catch (std::exception const& e) {
				std::cout << "Heal can only be cast with a staff equiped.\n";
				return false;
			}
			if (wzrd->loseMana(5)) { //If the wizard have enough mana
				//Apply heal between 10 and 20
				m_target->heal(rand() % 11 + 10);
				return true;
			}
			else {
				std::cout << "Not enough mana.\n";
				return false;
			}
		}
		catch (std::exception const& e) {
			std::cout << "Heal can only be cast by a wizard.\n";
			return false;
		}
		break;
	case WIZARD_ENCHANT:
		try {
			if (dynamic_cast<Wizard*>(m_source)->loseMana(8)) {
				Wizard* wzrd = dynamic_cast<Wizard*>(m_source);
				m_target->weapon()->isEnchanted(true);
				return true;
			}
			else {
				std::cout << "Not enough mana.\n";
				return false;
			}
		}
		catch (std::exception const& e) {
			return false;
		}
		break;
	case ROGUE_SNEAK:
		break;
	default:
		break;
	}
	return true;
}

Spell::~Spell() {

}