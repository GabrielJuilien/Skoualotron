#include "Attack.h"

Attack::Attack() : Skill() {
	m_attack_type = static_cast <attack_type>(0);
}

Attack::Attack(attack_type p_attack_type) : Skill() {
	m_attack_type = p_attack_type;
}

int Attack::dmg() {
	float coeff;
	switch (m_attack_type)
	{
	case OFF_WEAPON:
		coeff = rand() % 101 / 1000.0f + 0.95f;
		return (int)(16 * coeff * m_source->atk() / m_target->def());
		break;
	case WARRIOR_WEAPON:
		coeff = rand() % 151 / 1000.0f + 0.85f;
		return (int)(7 * coeff * m_source->atk() * m_source->weapon()->base_dmg() / m_target->def());
		break;
	case ARCHER_WEAPON:
		coeff = rand() % 151 / 1000.0f + 0.85f;
		return (int)(9 * coeff * m_source->agi() * m_source->weapon()->base_dmg() / m_target->def());
		break;
	case WIZARD_WEAPON:
		coeff = rand() % 151 / 1000.0f + 0.85f;
		return (int)(5 * coeff * m_source->intel() * m_source->weapon()->base_dmg() / m_target->def());
		break;
	case ROGUE_WEAPON:
		coeff = rand() % 151 / 1000.0f + 0.85f;
		return (int)(7 * coeff * m_source->agi() * m_source->weapon()->base_dmg() / m_target->def());
		break;
	default:
		std::cout << "Invalid attck type.\n";
		return 0;
		break;
	}
}

Attack::~Attack() {

}