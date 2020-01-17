#pragma once

#include "Character.h"
#include "utils.h"

class Archer : public Character {
protected:
	int m_heal_poison_charge;
	int m_arrows;
	bool m_isAiming;
	
	Archer();
public:
	Animation* anim_aiming;
	Animation* icon_aiming;
	Animation* anim_healPoison;
	Animation* icon_healPoison;

	Archer(std::string p_name, int p_max_health, int p_dodge_chance, int p_speed, int p_atk, int p_def, int p_agi, int p_intel);

	bool isAiming();

	int poisonHealCharge();

	int arrows();
	void arrows(int p_arrows);

	void castSkill(int p_pos, Character* p_target);

	bool canAim();
	void aim();

	bool canHealPoison(Character* p_target);
	void healPoison(Character* p_target);

	bool canAttackWeapon(Character* p_target);
	void attackWeapon(Character* p_target);

	void startTurn();

	void initRecap(SDL_Renderer* p_renderer);
	void updateRecap();

	void render();
	void renderIcon();

	~Archer();
};