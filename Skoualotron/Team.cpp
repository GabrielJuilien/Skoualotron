#include "Team.h"

Team::Team() {
	m_characters = std::vector<Character*>();
}

void Team::addMember(Character* p_newCharacter) {
	m_characters.push_back(p_newCharacter);
}

void Team::removeLastMember() {
	m_characters.pop_back();
}

void Team::deleteCharacter(int p_pos) {
	if (at(p_pos)) {
		delete m_characters.at(p_pos);
		m_characters.erase(m_characters.begin() + p_pos);
	}
}

void Team::addSpeed(int p_speed) {
	for (int i = 0; i < m_characters.size(); i++) {
		m_characters.at(i)->addSpeed(p_speed);
	}
}

Character* Team::at(int pos) {
	if (pos < 0 || pos > m_characters.size() - 1) return NULL;
	return m_characters.at(pos);
}

int Team::teamSize() {
	return (int)m_characters.size();
}

bool Team::isDead() {
	if (m_characters.empty()) return true;
	else return false;
}

void Team::render() {
	for (int i = 0; i < teamSize(); i++) {
		m_characters.at(i)->render();
	}
}

Team::~Team() {
	for (int i = 0; i < teamSize(); i++) {
		delete m_characters.at(0);
		m_characters.erase(m_characters.begin());
	}
}