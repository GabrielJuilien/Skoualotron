#include "Skill.h"

Skill::Skill() {
	m_source = NULL;
	m_target = NULL;
}

Skill::Skill(Character* p_source, Character* p_target) {
	m_source = p_source;
	m_target = p_target;
}

Skill::~Skill() {

}