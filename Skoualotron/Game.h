#pragma once
#include "Team.h"
#include "Archer.h"
#include "Rogue.h"
#include "Warrior.h"
#include "Wizard.h"
#include "AnimationStack.h"

enum class game_state {
	MENU,
	CHARACTER_DRAFT,
	WEAPON_DRAFT,
	ON_PROGRESS,
	LEFT_TEAM_WIN,
	RIGHT_TEAM_WIN,
	EXIT
};

class Game {
public:
	Team* m_leftTeam;
	Team* m_rightTeam;

	Character* m_currentCharacter;
	Animation* anim_currentCharacter;

	Character* m_targetCharacter;
	Animation* anim_targetCharacter;

	AnimationStack* menuAnimationStack;
	AnimationStack* characterDraftAnimationStack;
	AnimationStack* weaponDraftAnimationStack;
	AnimationStack* gameAnimationStack;

	std::vector<Character*> characterDraftPool;
	std::vector<Weapon*> weaponDraftPool;

	game_state m_currentState;
	Animation* logger;

	Game();

	Character* nextCharacterToPickWeapon();
	Character* getCharacterToPlay();
	void setTarget(Character* p_character);
	void checkDeath();

	game_state gameFinished();
	void switchState(game_state p_state);
	void startTurn();
	void render(SDL_Renderer* p_renderer, int x_mousePos, int y_mousePos);

	~Game();
};