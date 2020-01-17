#pragma once
#include "Weapon.h"
#include "Character.h"

class Warrior : public Character {
protected:
	int m_parry;
	bool m_provocation;
	int m_warcryMaxCooldown;
	int m_warcryCurrentCooldown;

	Warrior();
public: 
	Animation* anim_warcry;
	Animation* icon_warcry;
	Animation* anim_repair;
	Animation* icon_repair;

	Warrior(std::string p_name, int p_max_health, int p_dodge_chance, int p_speed, int p_atk, int p_def, int p_agi, int p_intel, int p_parry);

	bool provocation();

	bool parryRoll();

	void castSkill(int p_pos, Character* p_target);

	bool canWarcry();
	void warcry();

	bool canRepair(Character* p_target);
	void repair(Character* p_target);

	bool canAttackWeapon(Character* p_target);
	void attackWeapon(Character* p_character);

	void startTurn();

	void initRecap(SDL_Renderer* p_renderer);
	void updateRecap();

	void render();
	void renderIcon();

	~Warrior();
};