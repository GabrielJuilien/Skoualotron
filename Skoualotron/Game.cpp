#include "Game.h"

Game::Game() {
	m_leftTeam = new Team();
	m_rightTeam = new Team();

	menuAnimationStack = new AnimationStack();
	characterDraftAnimationStack = new AnimationStack();
	weaponDraftAnimationStack = new AnimationStack();
	gameAnimationStack = new AnimationStack();

	m_currentState = game_state::MENU;

	m_currentCharacter = NULL;
	anim_currentCharacter = NULL;
	m_targetCharacter = NULL;
	anim_targetCharacter = NULL;

	logger = NULL;
}

Character* Game::nextCharacterToPickWeapon() {
	if (m_rightTeam->at(2)->weapon()) return NULL;
	else if (m_leftTeam->at(2)->weapon()) return m_rightTeam->at(2);
	else if (m_rightTeam->at(1)->weapon()) return m_leftTeam->at(2);
	else if (m_leftTeam->at(1)->weapon()) return m_rightTeam->at(1);
	else if (m_rightTeam->at(0)->weapon()) return m_leftTeam->at(1);
	else if (m_leftTeam->at(0)->weapon()) return m_rightTeam->at(0);
	else return m_leftTeam->at(0);
}

Character* Game::getCharacterToPlay() {
	Character* character = NULL;
	int maxSpeed = 0, i;

	//Get the character with the highest speed
	for (i = 0; i < m_leftTeam->teamSize(); i++) {
		if (maxSpeed < m_leftTeam->at(i)->speed()) {
			character = m_leftTeam->at(i);
			maxSpeed = character->speed();
		}
	}
	for (i = 0; i < m_rightTeam->teamSize(); i++) {
		if (maxSpeed < m_rightTeam->at(i)->speed()) {
			character = m_rightTeam->at(i);
			maxSpeed = character->speed();
		}
	}

	//Start fastest character turn
	character->startTurn();

	//Add its base speed to all characters
	for (i = 0; i < m_rightTeam->teamSize(); i++) {
		m_rightTeam->addSpeed(character->baseSpeed());
	}
	for (i = 0; i < m_leftTeam->teamSize(); i++) {
		m_leftTeam->addSpeed(character->baseSpeed());
	}

	return character;
}

void Game::setTarget(Character* p_character) {
	m_targetCharacter = p_character;
	try {
		m_currentCharacter->isEnnemy(m_targetCharacter); //If target is an ennemy
		if (m_currentCharacter->animation()->getSide() == SIDE::LEFT) { //If target is in right team
			for (int i = 0; i < m_rightTeam->teamSize(); i++) {
				if (Character::is<Warrior>(*(m_rightTeam->at(i)))) { //If an ennemy is provocative 
					if (m_targetCharacter != m_rightTeam->at(i) && dynamic_cast<Warrior*>(m_rightTeam->at(i))->provocation()) { //If this ennemy isn't already targeted and is provocative
						m_targetCharacter = m_rightTeam->at(i); //Switch target
						throw std::string("data/Text/this_target_is_provocative.png"); //Display message
						return;
					}
				}
			}
		}
		else {
			for (int i = 0; i < m_leftTeam->teamSize(); i++) {
				if (Character::is<Warrior>(*(m_leftTeam->at(i)))) { //If an ennemy is provocative 
					if (m_targetCharacter != m_leftTeam->at(i) && dynamic_cast<Warrior*>(m_leftTeam->at(i))->provocation()) { //If this ennemy isn't already targeted and is provocative
						m_targetCharacter = m_leftTeam->at(i); //Switch target
						throw std::string("data/Text/this_target_is_provocative.png"); //Display message
						return;
					}
				}
			}
		}
	}
	catch (...) {
		//Target is an ally
	}
}

void Game::checkDeath() {
	int i;

	//Delete characters from team if they are dead
	for (i = 0; i < m_leftTeam->teamSize(); i++) {
		if (m_leftTeam->at(i)->isDead()) {
			m_leftTeam->deleteCharacter(i);
		}
	}
	for (i = 0; i < m_rightTeam->teamSize(); i++) {
		if (m_rightTeam->at(i)->isDead()) {
			m_rightTeam->deleteCharacter(i);
		}
	}

	//Check if a team has lost
	if (m_leftTeam->isDead()) {
		switchState(game_state::RIGHT_TEAM_WIN);
	}
	else if (m_rightTeam->isDead()) {
		switchState(game_state::LEFT_TEAM_WIN);
	}
}


game_state Game::gameFinished() {
	if (m_leftTeam->isDead()) return game_state::RIGHT_TEAM_WIN;
	if (m_rightTeam->isDead()) return game_state::LEFT_TEAM_WIN;
	return game_state::ON_PROGRESS;
}

void Game::switchState(game_state p_state) {
	m_currentState = p_state;
}

void Game::startTurn() {
	m_currentCharacter = getCharacterToPlay();
	m_targetCharacter = NULL;
}

void Game::render(SDL_Renderer* p_renderer, int x_mousePos, int y_mousePos) {
	SDL_RenderClear(p_renderer);
	int i;
	switch (m_currentState)
	{
	case game_state::MENU:
		menuAnimationStack->renderStack();
		break;
	case game_state::CHARACTER_DRAFT:
		//Render static elements
		characterDraftAnimationStack->renderStack();

		//Render characters
		m_leftTeam->render();
		m_rightTeam->render();

		//Render stats recaps on hover
		for (i = 0; i < characterDraftPool.size(); i++) {
			if (characterDraftAnimationStack->animation.at((size_t)i + 2)->hover(x_mousePos, y_mousePos)) {
				characterDraftPool.at(i)->renderRecap(x_mousePos, y_mousePos + 20);
			}
		}
		for (i = 0; i < m_leftTeam->teamSize(); i++) {
			if (m_leftTeam->at(i)->animation()->hover(x_mousePos, y_mousePos))
				m_leftTeam->at(i)->renderRecap(x_mousePos, y_mousePos + 20);
		}
		for (i = 0; i < m_rightTeam->teamSize(); i++) {
			if (m_rightTeam->at(i)->animation()->hover(x_mousePos, y_mousePos))
				m_rightTeam->at(i)->renderRecap(x_mousePos, y_mousePos + 20);
		}

		break;
	case game_state::WEAPON_DRAFT:
		//Render static elements
		weaponDraftAnimationStack->renderStack();

		//Render characters
		m_leftTeam->render();
		m_rightTeam->render();

		//Render recaps on hover
		for (i = 0; i < m_leftTeam->teamSize(); i++) {
			if (m_leftTeam->at(i)->animation()->hover(x_mousePos, y_mousePos))
				m_leftTeam->at(i)->renderRecap(x_mousePos, y_mousePos + 20);
		}
		for (i = 0; i < m_rightTeam->teamSize(); i++) {
			if (m_rightTeam->at(i)->animation()->hover(x_mousePos, y_mousePos))
				m_rightTeam->at(i)->renderRecap(x_mousePos, y_mousePos + 20);
		}
		for (i = 0; i < weaponDraftPool.size(); i++) {
			if (weaponDraftAnimationStack->animation.at((size_t)i + 2)->hover(x_mousePos, y_mousePos)) {
				weaponDraftPool.at(i)->renderRecap(x_mousePos, y_mousePos + 20);
			}
		}

		break;
	case game_state::ON_PROGRESS:
		//Render static elements
		gameAnimationStack->renderStack();

		//Render characters
		m_leftTeam->render();
		m_rightTeam->render();

		//Render current character icons
		m_currentCharacter->renderIcon();

		//Render arrow above current character
		anim_currentCharacter->setPos(m_currentCharacter->animation()->get_xPos(), m_currentCharacter->animation()->get_yPos() - 320);
		anim_currentCharacter->render();

		//Render arrow above target
		if (m_targetCharacter && m_targetCharacter != m_currentCharacter) {
			anim_targetCharacter->setPos(m_targetCharacter->animation()->get_xPos(), m_targetCharacter->animation()->get_yPos() - 320);
			anim_targetCharacter->render();
		}

		//Render recaps on hover
		for (i = 0; i < m_leftTeam->teamSize(); i++) {
			if (m_leftTeam->at(i)->animation()->hover(x_mousePos, y_mousePos))
				m_leftTeam->at(i)->renderRecap(x_mousePos, y_mousePos + 20);
		}
		for (i = 0; i < m_rightTeam->teamSize(); i++) {
			if (m_rightTeam->at(i)->animation()->hover(x_mousePos, y_mousePos))
				m_rightTeam->at(i)->renderRecap(x_mousePos, y_mousePos + 20);
		}

		//Display error message, or no text
		logger->render();

		break;
	case game_state::LEFT_TEAM_WIN:
		//Render static elements
		gameAnimationStack->renderStack();
		break;
	case game_state::RIGHT_TEAM_WIN:
		//Render static elements
		gameAnimationStack->renderStack();
		break;
	default:
		break;
	}
}

Game::~Game() {
	//Deleting teams
	delete m_leftTeam;
	delete m_rightTeam;

	//Deleting arrows
	if (anim_currentCharacter)
		delete anim_currentCharacter;
	if (anim_targetCharacter)
		delete anim_targetCharacter;

	//Deleting static elements
	if (menuAnimationStack)
		delete menuAnimationStack;
	if (characterDraftAnimationStack)
		delete characterDraftAnimationStack;
	if (weaponDraftAnimationStack)
		delete weaponDraftAnimationStack;
	if (gameAnimationStack)
		delete gameAnimationStack;

	//Deleting error message
	delete logger;
}