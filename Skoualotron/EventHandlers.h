#include "Game.h"
#include "utils.h"
#include "readConf.h"

Game* handlerLeftTeamWin(Game* game, SDL_Event e, SDL_Renderer* renderer);
Game* handlerRightTeamWin(Game* game, SDL_Event e, SDL_Renderer* renderer);
Game* handlerGameOnProgress(Game* game, SDL_Event e, SDL_Renderer* renderer);
Game* handlerWeaponDraft(Game* game, SDL_Event e, SDL_Renderer* renderer);
Game* handlerCharacterDraft(Game* game, SDL_Event e, SDL_Renderer* renderer);
Game* handlerTitleScreen(Game* game, SDL_Event e, SDL_Renderer* renderer);