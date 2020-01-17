#pragma once

#include "Character.h"

class Wizard : public Character {
protected:
	int m_max_mana;
	int m_current_mana;
	int m_healMaxCooldown;
	int m_healCurrentCooldown;
	int m_enchantMaxCooldown;
	int m_enchantCurrentCooldown;

	Wizard();
public:
	Animation* anim_heal;
	Animation* icon_heal;
	Animation* anim_regenMana;
	Animation* icon_regenMana;
	Animation* anim_enchant;
	Animation* icon_enchant;

	Wizard(std::string p_name, int p_max_health, int p_dodge_chance, int p_speed, int p_atk, int p_def, int p_agi, int p_intel);

	int mana();
	bool loseMana(int p_amount);

	void castSkill(int p_pos, Character* p_target);

	bool canHeal(Character* p_target);
	void heal(Character* p_target);

	bool canEnchant(Character* p_target);
	void enchant(Character* p_target);

	bool canRegenMana();
	void regenMana();

	bool canAttackWeapon(Character* p_target);
	void attackWeapon(Character* p_target);

	void startTurn();

	void initRecap(SDL_Renderer* p_renderer);
	void updateRecap();

	void render();
	void renderIcon();

	~Wizard();
};