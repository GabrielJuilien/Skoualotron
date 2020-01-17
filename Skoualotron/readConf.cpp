
#include "readConf.h"

Game* loadConf(std::string charactersPath, std::string weaponsPath, SDL_Renderer* renderer) {
	Game* new_game = new Game();
	int nbLoadedChars = 0;
	Animation* loading = new Animation("data/Backgrounds/loading.png", renderer, 25, true, true);
	loading->render();
	SDL_RenderPresent(renderer);

	//Loading default animations and images
	new_game->menuAnimationStack->append(new Animation("data/Backgrounds/title_screen.png", renderer, 10, true, true));
	new_game->menuAnimationStack->append(new Animation("data/Text/new_game.png", renderer, 25, 0, true, true, 320, 180, WINDOW_WIDTH / 2 - 160, WINDOW_HEIGHT / 2, SIDE::LEFT));
	new_game->menuAnimationStack->append(new Animation("data/Text/exit.png", renderer, 25, 0, true, true, 240, 135, WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 3 * 2, SIDE::LEFT));

	loading->render();
	loading->render();
	SDL_RenderPresent(renderer);

	//Loading characters draft animations
	new_game->characterDraftAnimationStack->append(new Animation("data/Backgrounds/pick.png", renderer, 10, true, true));
	new_game->characterDraftAnimationStack->append(new Animation("data/Text/leave_game.png", renderer, 1, 0, true, true, 80, 80, 1800, 40, SIDE::LEFT)); //Exit button

	loading->render();
	loading->render();
	SDL_RenderPresent(renderer);
	
	//Loading weapons draft animations
	new_game->weaponDraftAnimationStack->append(new Animation("data/Backgrounds/pick.png", renderer, 10, true, true));
	new_game->weaponDraftAnimationStack->append(new Animation("data/Text/leave_game.png", renderer, 1, 0, true, true, 80, 80, 1800, 40, SIDE::LEFT)); //Exit button

	loading->render();
	loading->render();
	SDL_RenderPresent(renderer);

	//Loading fight animations
	new_game->gameAnimationStack->append(new Animation("data/Backgrounds/background.png", renderer, 10, true, true));
	new_game->gameAnimationStack->append(new Animation("data/Text/leave_game.png", renderer, 1, 0, true, true, 80, 80, 1800, 40, SIDE::LEFT)); //Exit button
	new_game->gameAnimationStack->append(new Animation("data/Characters/skip.png", renderer, 1, 0, true, true, 64, 64, 26, 116, SIDE::LEFT)); //Skip turn button

	loading->render();
	loading->render();
	SDL_RenderPresent(renderer);

	new_game->anim_currentCharacter = new Animation("data/Text/caster_arrow.png", renderer, 25, 0, true, true, 320, 320, 0, 0, SIDE::LEFT);
	new_game->anim_targetCharacter = new Animation("data/Text/target_arrow.png", renderer, 25, 0, true, true, 320, 320, 0, 0, SIDE::LEFT);
	new_game->logger = new Animation("data/Text/empty_text.png", renderer, 1, 0, true, true, 500, 200, 710, 190, SIDE::LEFT);

	loading->render();
	loading->render();
	SDL_RenderPresent(renderer);

	//Loading victory animations

	std::ifstream characterFileStream;
	std::ifstream weaponFileStream;

	std::string characterClass;
	std::string characterName;
	int characterHealth = 0;
	int characterSpeed = 0;
	int characterAttack = 0;
	int characterDefense = 0;
	int characterIntelligence = 0;
	int characterAgility = 0;
	int characterDodge = 0;
	int characterSpecial = 0;

	std::string line_buffer;
	Archer* tmpArcher;
	Rogue* tmpRogue;
	Warrior* tmpWarrior;
	Wizard* tmpWizard;

	try {
		
		characterFileStream.open(charactersPath, std::ifstream::in);
	}
	catch (...) {
		return NULL;
	}

	while (std::getline(characterFileStream, line_buffer)) {
		if (line_buffer.rfind("\tClass ", 0) == 0) {
			characterClass = line_buffer.substr(7);
		}
		else if (line_buffer.rfind("\tName ", 0) == 0) {
			line_buffer = line_buffer.substr(6);
			characterName = line_buffer;
		}
		else if (line_buffer.rfind("\tSpeed ", 0) == 0) {
			line_buffer = line_buffer.substr(7);
			characterSpeed = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tAttack ", 0) == 0) {
			line_buffer = line_buffer.substr(8);
			characterAttack = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tIntelligence ", 0) == 0) {
			line_buffer = line_buffer.substr(14);
			characterIntelligence = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tHealth ", 0) == 0) {
			line_buffer = line_buffer.substr(8);
			characterHealth = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tDodge ", 0) == 0) {
			line_buffer = line_buffer.substr(7);
			characterDodge = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tDefense ", 0) == 0) {
			line_buffer = line_buffer.substr(9);
			characterDefense = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tAgility ", 0) == 0) {
			line_buffer = line_buffer.substr(9);
			characterAgility = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tSpecial ", 0) == 0) {
			line_buffer = line_buffer.substr(9);
			characterSpecial = atoi(line_buffer.c_str());
		}
		
		if (line_buffer.compare("EndCharacter") == 0) {
			
			if (characterClass.compare("Archer") == 0) {
				tmpArcher = new Archer(characterName, characterHealth, characterDodge, characterSpeed, characterAttack, characterDefense, characterAgility, characterIntelligence);
				new_game->characterDraftPool.push_back(tmpArcher);

				tmpArcher->initRecap(renderer);

				tmpArcher->initAnimation("data/Characters/Archer/archer_idle.png", renderer, 50, 0, true, true, 320, 320, (nbLoadedChars % 5) * 320, (nbLoadedChars / 5) * 320, SIDE::LEFT);
				new_game->characterDraftAnimationStack->append(tmpArcher->animation());
			}
			else if (characterClass.compare("Rogue") == 0) {
				tmpRogue = new Rogue(characterName, characterHealth, characterDodge, characterSpeed, characterAttack, characterDefense, characterAgility, characterIntelligence, characterSpecial);
				new_game->characterDraftPool.push_back(tmpRogue);

				tmpRogue->initRecap(renderer);

				tmpRogue->initAnimation("data/Characters/Rogue/rogue_idle.png", renderer, 50, 0, true, true, 320, 320, (nbLoadedChars % 5) * 320, (nbLoadedChars / 5) * 320, SIDE::LEFT);
				new_game->characterDraftAnimationStack->append(tmpRogue->animation());
			}
			else if (characterClass.compare("Warrior") == 0) {
				tmpWarrior = new Warrior(characterName, characterHealth, characterDodge, characterSpeed, characterAttack, characterDefense, characterAgility, characterIntelligence, characterSpecial);
				new_game->characterDraftPool.push_back(tmpWarrior);

				tmpWarrior->initRecap(renderer);

				tmpWarrior->initAnimation("data/Characters/Warrior/warrior_idle.png", renderer, 50, 0, true, true, 320, 320, (nbLoadedChars % 5) * 320, (nbLoadedChars / 5) * 320, SIDE::LEFT);
				new_game->characterDraftAnimationStack->append(tmpWarrior->animation());
			}
			else if (characterClass.compare("Wizard") == 0) {
				tmpWizard = new Wizard(characterName, characterHealth, characterDodge, characterSpeed, characterAttack, characterDefense, characterAgility, characterIntelligence);
				new_game->characterDraftPool.push_back(tmpWizard);

				tmpWizard->initRecap(renderer);

				tmpWizard->initAnimation("data/Characters/Wizard/wizard_idle.png", renderer, 50, 0, true, true, 320, 320, (nbLoadedChars % 5) * 320 , (nbLoadedChars / 5) * 320, SIDE::LEFT);
				new_game->characterDraftAnimationStack->append(tmpWizard->animation());
			}
			nbLoadedChars++;
		}

		if (characterFileStream.eof()) break;
	}
	characterFileStream.close();

	loading->render();
	SDL_RenderPresent(renderer);

	
	std::string weaponName;
	std::string weaponClass;
	int weaponHealth = 0;
	int weaponCost = 0;
	int weaponIntelligence = 0;
	int weaponCritical = 0;
	int weaponDamage = 0;
	int weaponAttack = 0;
	int weaponDurability = 0;
	int weaponArrows = 0;
	int weaponAgility = 0;
	int weaponDefense = 0;

	int nbLoadedWeapons = 0;

	Bow* tmpBow;
	Dagger* tmpDagger;
	Staff* tmpStaff;
	Sword* tmpSword;

	try {
		weaponFileStream.open(weaponsPath, std::ifstream::in);
	}
	catch (...) {
		return NULL;
	}

	while (std::getline(weaponFileStream, line_buffer)) {
		if (line_buffer.rfind("\tType ", 0) == 0) {
			line_buffer = line_buffer.substr(6);
			weaponClass = line_buffer;
		}
		else if (line_buffer.rfind("\tName ", 0) == 0) {
			line_buffer = line_buffer.substr(6);
			weaponName = line_buffer;
		}
		else if (line_buffer.rfind("\tCritical ", 0) == 0) {
			line_buffer = line_buffer.substr(10);
			weaponCritical = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tAttack ", 0) == 0) {
			line_buffer = line_buffer.substr(8);
			weaponAttack = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tIntelligence ", 0) == 0) {
			line_buffer = line_buffer.substr(14);
			weaponIntelligence = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tHealth ", 0) == 0) {
			line_buffer = line_buffer.substr(8);
			weaponHealth = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tDamage ", 0) == 0) {
			line_buffer = line_buffer.substr(8);
			weaponDamage = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tDurability ", 0) == 0) {
			line_buffer = line_buffer.substr(12);
			weaponDurability = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tArrows ", 0) == 0) {
			line_buffer = line_buffer.substr(8);
			weaponArrows = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tCost ", 0) == 0) {
			line_buffer = line_buffer.substr(6);
			weaponCost = atoi(line_buffer.c_str());
		}
		else if (line_buffer.rfind("\tAgility ", 0) == 0) {
			line_buffer = line_buffer.substr(10);
			weaponAgility = atoi(line_buffer.c_str());
		}
		else if (line_buffer.find("\tDefense ", 0) == 0) {
			line_buffer = line_buffer.substr(9);
			weaponDefense = atoi(line_buffer.c_str());
		}

		if (line_buffer.compare("EndWeapon") == 0) {
			if (weaponClass.compare("Bow") == 0) {
				tmpBow = new Bow(weaponName, weaponDamage, weaponCritical, weaponHealth, weaponAttack, weaponArrows);
				new_game->weaponDraftPool.push_back(tmpBow);

				tmpBow->initRecap(renderer);

				tmpBow->initAnimation("data/Weapons/bow_idle.png", renderer, 50, rand() % 50, true, true, 320, 320, (nbLoadedWeapons % 5) * 320, (nbLoadedWeapons / 5) * 320, SIDE::LEFT);
				new_game->weaponDraftAnimationStack->append(tmpBow->animation());
			}
			else if (weaponClass.compare("Dagger") == 0) {
				tmpDagger = new Dagger(weaponName, weaponDamage, weaponCritical, weaponDurability, weaponHealth, weaponAttack, weaponDefense, weaponAgility);
				new_game->weaponDraftPool.push_back(tmpDagger);

				tmpDagger->initRecap(renderer);

				tmpDagger->initAnimation("data/Weapons/dagger_idle.png", renderer, 50, rand() % 50, true, true, 320, 320, (nbLoadedWeapons % 5) * 320, (nbLoadedWeapons / 5) * 320, SIDE::LEFT);
				new_game->weaponDraftAnimationStack->append(tmpDagger->animation());
			}
			else if (weaponClass.compare("Staff") == 0) {
				tmpStaff = new Staff(weaponName, weaponDamage, weaponCritical, weaponHealth, weaponAttack, weaponIntelligence, weaponCost);
				new_game->weaponDraftPool.push_back(tmpStaff);

				tmpStaff->initRecap(renderer);

				tmpStaff->initAnimation("data/Weapons/staff_idle.png", renderer, 50, rand() % 50, true, true, 320, 320, (nbLoadedWeapons % 5) * 320, (nbLoadedWeapons / 5) * 320, SIDE::LEFT);
				new_game->weaponDraftAnimationStack->append(tmpStaff->animation());
			}
			else if (weaponClass.compare("Sword") == 0) {
				tmpSword = new Sword(weaponName, weaponDamage, weaponCritical, weaponDurability, weaponHealth, weaponAttack);
				new_game->weaponDraftPool.push_back(tmpSword);

				tmpSword->initRecap(renderer);

				tmpSword->initAnimation("data/Weapons/sword_idle.png", renderer, 50, rand() % 50, true, true, 320, 320, (nbLoadedWeapons % 5) * 320, (nbLoadedWeapons / 5) * 320, SIDE::LEFT);
				new_game->weaponDraftAnimationStack->append(tmpSword->animation());
			}

			nbLoadedWeapons++;
			weaponHealth = 0;
			weaponCost = 0;
			weaponIntelligence = 0;
			weaponCritical = 0;
			weaponDamage = 0;
			weaponAttack = 0;
			weaponDurability = 0;
			weaponArrows = 0;
			weaponAgility = 0;
			weaponDefense = 0;
		}

		//if (weaponFileStream.eof()) break;
	}
	weaponFileStream.close();
	delete loading;

	
	return new_game;
}