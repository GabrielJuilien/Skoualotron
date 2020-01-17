#pragma once
#include <string>
#include <vector>
#include <exception>
#include <iostream>

enum modifier_id { DODGE, SPEED, ATK, DEF, INT, AGI, NB_MODS };

class Modifier {
protected:
	modifier_id m_id;
	float m_value;

	Modifier();

public:
	Modifier(modifier_id p_id, float p_value);

	modifier_id id();
	float value();
	void value(float p_value);

	bool operator==(Modifier mod);

	~Modifier();
};