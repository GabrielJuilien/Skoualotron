#pragma once
#include "Modifier.h"
#include "Animation.h"

class Weapon {
protected:
	std::string m_name;
	int m_base_dmg;
	int m_crit_chance;
	int m_health;
	int m_attack;
	
	Animation* m_animation;
	Animation* m_statRecap;
	
	bool m_enchantement;
	
	Weapon();

public:
	Weapon(std::string p_name, int p_base_dmg, int p_crit_chance, int p_health, int p_attack);

	void name(std::string p_name);
	std::string name();

	void base_dmg(int p_base_dmg);
	virtual int base_dmg();

	void crit_chance(int p_crit_chance);
	int crit_chance();

	void health(int p_health);
	int health();

	void attack(int p_attack);
	int attack();

	void initAnimation(std::string p_path, SDL_Renderer* p_renderer, int p_length, int p_currentFrame, bool p_loop, bool p_play, int x_spriteSize, int y_spriteSize, int x_spritePos, int y_spritePos, SIDE p_side);
	void initRecap(SDL_Renderer* p_renderer);
	void renderRecap(int x_pos, int y_pos);

	Animation* animation();

	void isEnchanted(bool p_enchantement);
	bool isEnchanted();

	template <class T>
	static bool is(Weapon& weapon) {
		try {
			dynamic_cast<T&>(weapon);
			return true;
		}
		catch (...) {
			return false;
		}
	}

	virtual ~Weapon();
};

