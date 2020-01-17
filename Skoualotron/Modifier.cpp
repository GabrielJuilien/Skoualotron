#include "Modifier.h"

Modifier::Modifier() {
	m_id = static_cast<modifier_id>(0);
	m_value = 0;
}

Modifier::Modifier(modifier_id p_id, float p_value) {
	m_id = p_id;
	m_value = p_value;
}

modifier_id Modifier::id() {
	return m_id;
}

float Modifier::value() {
	return m_value;
}

void Modifier::value(float p_value) {
	m_value *= p_value;
}

Modifier::~Modifier() {

}


bool Modifier::operator==(Modifier mod) {
	if (m_id == mod.id()) return true;
	else return false;
}