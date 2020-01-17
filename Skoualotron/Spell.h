#pragma once
#include "Skill.h"

enum spell_id {
	WARRIOR_WARCRY,
	WARRIOR_REPAIR,
	ARCHER_AIM,
	ARCHER_HEAL_POISON,
	WIZARD_REGENERATE_MANA,
	WIZARD_HEAL,
	WIZARD_ENCHANT,
	ROGUE_SNEAK,
	NB_SPELL
};

enum target_type {
	SELF,
	ALLY,
	ENNEMY,
	NB_TARGET_TYPE
};

class Spell : Skill {
private:
	int m_cooldown;
	int m_current_cooldown;
	spell_id m_spell_id;
	target_type m_target_type;

	Spell();
public:
	Spell(int p_cooldown, spell_id p_spell_id, target_type p_target_type);

	bool cast();

	~Spell();
};