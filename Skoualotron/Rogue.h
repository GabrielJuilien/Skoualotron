#pragma once
#include "Character.h"


class Rogue : public Character {
protected:
	int m_poisonChance;
	bool m_isInvisible;
	int m_invisibilityMaxCooldown;
	int m_invisibilityCurrentCooldown;
	int m_arrows;

	Rogue();
public:
	Animation* anim_sneak;
	Animation* icon_sneak;
	Rogue(std::string p_name, int p_max_health, int p_dodge_chance, int p_speed, int p_atk, int p_def, int p_agi, int p_intel, int p_poisonChance);

	bool isInvisible();

	bool poisonRoll();

	int arrows();
	void arrows(int p_arrows);

	void castSkill(int p_pos, Character* p_target);

	bool canSneak();
	void sneak();

	bool canAttackWeapon(Character* p_target);
	void attackWeapon(Character* p_target);

	void startTurn();

	void initRecap(SDL_Renderer* p_renderer);
	void updateRecap();

	void render();
	void renderIcon();

	~Rogue();
};