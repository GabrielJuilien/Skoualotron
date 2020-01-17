#pragma once
#include "Character.h"


class Team {
private:
	std::vector<Character*> m_characters;

public:
	Team();

	void addMember(Character* p_newCharacter);
	void removeLastMember();
	void deleteCharacter(int p_pos);

	void addSpeed(int p_speed);
	Character* at(int pos);

	int teamSize();
	bool isDead();
	
	void render();

	~Team();
};