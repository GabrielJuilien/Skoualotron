#include "EventHandlers.h"

Game* handlerLeftTeamWin(Game* game, SDL_Event e, SDL_Renderer* renderer) {
	int x_mousePos = 0;
	int y_mousePos = 0;

	SDL_GetMouseState(&x_mousePos, &y_mousePos);

	switch (e.type)
	{
	case SDL_QUIT:
		game->switchState(game_state::EXIT);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			//Leave button
			if (game->gameAnimationStack->animation.at(1)->hover(x_mousePos, y_mousePos)) {
				delete game;
				game = loadConf("data/characters.jdc", "data/weapons.jdc", renderer);
				game->m_currentState = game_state::MENU;
			}
		default:
			break;
		}
	}
	return game;
}

Game* handlerRightTeamWin(Game* game, SDL_Event e, SDL_Renderer* renderer) {
	int x_mousePos = 0;
	int y_mousePos = 0;

	SDL_GetMouseState(&x_mousePos, &y_mousePos);

	switch (e.type)
	{
	case SDL_QUIT:
		game->switchState(game_state::EXIT);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			game->logger->changeTexture("data/Text/empty_text.png");
			//Leave button
			if (game->gameAnimationStack->animation.at(1)->hover(x_mousePos, y_mousePos)) {
				delete game;
				game = loadConf("data/characters.jdc", "data/weapons.jdc", renderer);
				game->m_currentState = game_state::MENU;
			}
		}
	}
	return game;
}

Game* handlerGameOnProgress(Game* game, SDL_Event e, SDL_Renderer* renderer) {
	int x_mousePos = 0;
	int y_mousePos = 0;
	int i;

	SDL_GetMouseState(&x_mousePos, &y_mousePos);

	switch (e.type)
	{
	case SDL_QUIT:
		game->switchState(game_state::EXIT);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			//Reset error message
			game->logger->changeTexture("data/Text/empty_text.png");

			//Leave button
			if (game->gameAnimationStack->animation.at(1)->hover(x_mousePos, y_mousePos)) {
				delete game;
				game = loadConf("data/characters.jdc", "data/weapons.jdc", renderer);
				game->m_currentState = game_state::MENU;
			}

			//Select target
			for (i = 0; i < game->m_leftTeam->teamSize(); i++) {
				if (game->m_leftTeam->at(i)->animation()->hover(x_mousePos, y_mousePos)) {
					try {
						game->setTarget(game->m_leftTeam->at(i));
					}
					catch (std::string exception) {
						game->logger->changeTexture(exception);
					}
				}
			}
			for (i = 0; i < game->m_rightTeam->teamSize(); i++) {
				if (game->m_rightTeam->at(i)->animation()->hover(x_mousePos, y_mousePos)) {
					try {
						game->setTarget(game->m_rightTeam->at(i));
					}
					catch (std::string exception) {
						game->logger->changeTexture(exception);
					}
				}
			}

			//If a target is selected, detect if a spell is clicked
			if (game->m_targetCharacter) {
				for (i = 0; i < 5; i++) {
					if (x_mousePos > 110 * i && x_mousePos < 110 * (i + 1) && y_mousePos > 0 && y_mousePos < 110) {
						try {
							//Trying to cast the skill
							game->m_currentCharacter->castSkill(i, game->m_targetCharacter);

							//Updating recaps
							game->m_targetCharacter->updateRecap();
							game->m_currentCharacter->updateRecap();

							//Checking if a character is dead
							game->checkDeath();

							//Check if game is finished
							if (game->m_currentState == game_state::LEFT_TEAM_WIN) {
								game->gameAnimationStack->append(new Animation("data/Text/left_team_win.png", renderer, 1, true, true));
							}
							else if (game->m_currentState == game_state::RIGHT_TEAM_WIN) {
								game->gameAnimationStack->append(new Animation("data/Text/right_team_win.png", renderer, 1, true, true));
							}
							else game->startTurn();

						}
						catch (std::string reason) {
							//Else display why the skil couldn't be played
							game->logger->changeTexture(reason);
							if (!std::strcmp(reason.c_str(), "data/Text/target_dodged.png")) {
								//Updating recaps
								game->m_targetCharacter->updateRecap();
								game->m_currentCharacter->updateRecap();

								//Checking if a character is dead
								game->checkDeath();

								//Selecting next character to play
								game->startTurn();
							}
						}
					}
				}
			}
			else {
				//Display message
				game->logger->changeTexture("data/Text/select_a_target_first.png");
			}
			break;
		default:
			break;
		}
	}
	return game;
}

Game* handlerWeaponDraft(Game* game, SDL_Event e, SDL_Renderer* renderer) {
	int x_mousePos = 0;
	int y_mousePos = 0;
	int i;

	Character* nextCharacterToPickWeapon = NULL;

	SDL_GetMouseState(&x_mousePos, &y_mousePos);

	switch (e.type)
	{
	case SDL_QUIT:
		game->switchState(game_state::EXIT);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			if (game->weaponDraftAnimationStack->animation.at(1)->hover(x_mousePos, y_mousePos)) {
				delete game;
				game = loadConf("data/characters.jdc", "data/weapons.jdc", renderer);
				game->m_currentState = game_state::MENU;
				break;
			}

			nextCharacterToPickWeapon = game->nextCharacterToPickWeapon();
			if (nextCharacterToPickWeapon) {
				for (i = 0; i < game->weaponDraftPool.size(); i++) {
					if (game->weaponDraftAnimationStack->animation.at((size_t)i + 2)->hover(x_mousePos, y_mousePos)) {
						if (nextCharacterToPickWeapon->equip(game->weaponDraftPool.at(i))) {

							delete game->weaponDraftAnimationStack->animation.at((size_t)i + 2);
							game->weaponDraftAnimationStack->animation.erase(game->weaponDraftAnimationStack->animation.begin() + i + 2);

							game->weaponDraftPool.erase(game->weaponDraftPool.begin() + i);

							if (!game->nextCharacterToPickWeapon()) {
								game->weaponDraftAnimationStack->append(new Animation("data/Text/next.png", renderer, 25, 0, true, true, 240, 135, 1680, 450, SIDE::LEFT));
							}
						}
					}
				}
			}
			else if (!nextCharacterToPickWeapon) {
				if (game->weaponDraftAnimationStack->animation.at(1)->hover(x_mousePos, y_mousePos)) {
					delete game->weaponDraftAnimationStack;
					game->weaponDraftAnimationStack = NULL;
					game->m_currentState = game_state::WEAPON_DRAFT;
				}
				else if (game->weaponDraftAnimationStack->animation.at(game->weaponDraftAnimationStack->animation.size() - 1)->hover(x_mousePos, y_mousePos)) {
					delete game->weaponDraftAnimationStack;
					game->weaponDraftAnimationStack = NULL;
					game->startTurn();
					game->m_currentState = game_state::ON_PROGRESS;
				}
			}
			break;
		default:
			break;
		}
	default:
		break;
	}
	return game;
}

Game* handlerCharacterDraft(Game* game, SDL_Event e, SDL_Renderer* renderer) {
	int x_mousePos = 0;
	int y_mousePos = 0;
	int i;

	SDL_GetMouseState(&x_mousePos, &y_mousePos);

	switch (e.type)
	{
	case SDL_QUIT:
		game->switchState(game_state::EXIT);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			if (game->characterDraftAnimationStack->animation.at(1)->hover(x_mousePos, y_mousePos)) {
				delete game;
				game = loadConf("data/characters.jdc", "data/weapons.jdc", renderer);
			}
			else if (game->m_rightTeam->teamSize() < 3) { //While teams haven't both picked 3 characters
				for (i = 0; i < game->characterDraftPool.size(); i++) {
					if (game->characterDraftAnimationStack->animation.at((size_t)i + 2)->hover(x_mousePos, y_mousePos)) {

						//Adding character to teams and modifying character animation position
						if (game->m_rightTeam->teamSize() != game->m_leftTeam->teamSize()) {
							game->characterDraftAnimationStack->animation.at((size_t)i + 2)->setPos(WINDOW_WIDTH / 2 + game->m_rightTeam->teamSize() * 320, WINDOW_HEIGHT - 360);
							game->characterDraftPool.at(i)->animation()->flip(SIDE::RIGHT);
							game->m_rightTeam->addMember(game->characterDraftPool.at(i));
						}
						else {
							game->m_leftTeam->addMember(game->characterDraftPool.at(i));
							game->characterDraftAnimationStack->animation.at((size_t)i + 2)->setPos(WINDOW_WIDTH / 2 - game->m_leftTeam->teamSize() * 320, WINDOW_HEIGHT - 360);
						}

						//Removing animation from draft AnimationStack
						game->characterDraftAnimationStack->animation.erase(game->characterDraftAnimationStack->animation.begin() + i + 2);

						//Removing character from draft pool
						game->characterDraftPool.erase(game->characterDraftPool.begin() + i);

						//Adding "Next" button
						if (game->m_rightTeam->teamSize() == 3) {
							game->characterDraftAnimationStack->append(new Animation("data/Text/next.png", renderer, 25, 0, true, true, 240, 135, 1680, 450, SIDE::LEFT));
						}
					}
				}
			}
			else if (game->m_rightTeam->teamSize() == 3) {
				if (game->characterDraftAnimationStack->animation.at(game->characterDraftAnimationStack->animation.size() - 1)->hover(x_mousePos, y_mousePos)) {
					delete game->characterDraftAnimationStack;
					game->characterDraftAnimationStack = NULL;
					game->m_currentState = game_state::WEAPON_DRAFT;
				}
			}
			break;
		default:
			break;
		}
	default:
		break;
	}
	return game;
}
Game* handlerTitleScreen(Game* game, SDL_Event e, SDL_Renderer* renderer) {
	int x_mousePos = 0;
	int y_mousePos = 0;
	int i;

	SDL_GetMouseState(&x_mousePos, &y_mousePos);

	switch (e.type)
	{
	case SDL_QUIT:
		game->switchState(game_state::EXIT);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			if (game->menuAnimationStack->animation.at(1)->hover(x_mousePos, y_mousePos)) {
				delete game->menuAnimationStack;
				game->menuAnimationStack = NULL;
				game->switchState(game_state::CHARACTER_DRAFT);
			}
			else if (game->menuAnimationStack->animation.at(2)->hover(x_mousePos, y_mousePos)) game->switchState(game_state::EXIT);
		default:
			break;
		}
	default:
		break;
	}
	return game;
}