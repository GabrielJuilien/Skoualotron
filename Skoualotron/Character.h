#pragma once
#include "Bow.h"
#include "Dagger.h"
#include "Staff.h"
#include "Sword.h"

#include "utils.h"
#include "AnimationStack.h"
#include <vector>



class Character {
protected:
	std::string m_name;

	int m_current_health;
	int m_max_health;
	bool m_poisonned;
	int m_dodge_chance;
	int m_baseSpeed;
	int m_speed;
	int m_atk;
	int m_def;
	int m_agi;
	int m_intel;

	Weapon* m_weapon;

	std::vector<Modifier*> m_mods;

	Character();
public:
	Animation* anim_idle;
	Animation* anim_basic;
	Animation* icon_basic;
	Animation* anim_weapon;
	Animation* icon_weapon;
	Animation* anim_recap;
	Animation* anim_hp;

	Character(std::string p_name, int p_max_health, int p_dodge_chance, int p_speed, int p_atk, int p_def, int p_agi, int p_intel);

	void initAnimation(std::string p_path, SDL_Renderer* p_renderer, int p_length, int p_currentFrame, bool p_loop, bool p_play, int x_spriteSize, int y_spriteSize, int x_spritePos, int y_spritePos, SIDE p_side);
	virtual void render();
	
	virtual void initRecap(SDL_Renderer* p_renderer);
	virtual void updateRecap();
	void renderRecap(int x_pos, int y_pos);

	virtual void renderIcon();

	//Setters
	void poisonned(bool p_poison);
	void dodge_chance(int p_dodge_chance);
	void speed(int p_speed);
	void addSpeed(int p_speed);
	void atk(int p_atk);
	void def(int p_def);
	void agi(int p_agi);
	void intel(int p_intel);
	void setMaxHealth(int p_health);

	//Getters
	int health();
	bool poisonned();
	int dodge_chance();
	int baseSpeed();
	int speed();
	int atk();
	int def();
	int agi();
	int intel();
	std::string name();
	Animation* animation(); //Returns the default (idle) animation
	void isAlly(Character* p_target);
	void isEnnemy(Character* p_target);
	void isSelf(Character* p_target);

	//Health management
	void takeDmg(int p_dmg);
	void heal(int p_heal);
	bool isDead();

	//Turn management
	virtual void startTurn();
	virtual void castSkill(int p_pos, Character* p_target);

	//Weapon management
	bool equip(Weapon* p_weapon);
	Weapon* weapon();

	//Mods management
	std::vector<Modifier*> mods();
	Modifier* mods(modifier_id mod_id);

	//Dodge management
	bool dodge();

	//Default attack
	bool canAttack(Character* p_target);
	void attack(Character* p_target);

	template <class T>
	static bool is(Character& character) {
		try {
			dynamic_cast<T&>(character);
			return true;
		}
		catch (...) {
			return false;
		}
	}

	//Destroyer
	virtual ~Character();
};

