#include "readConf.h" 
#include "eventHandlers.h"

enum class ePosition {
	DOWN,
	LEFT,
	UP,
	RIGHT,
	NB_POSITION
};

int main(int argc, char** argv) {
	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint32 timeStep = 1000 / FRAMERATE;
	Uint32 timeElapsed = 0;

	std::vector<Animation*>::iterator tmp;

	int currentPick = 0;

	SDL_Event e;

	int x_mousePos = 0;
	int y_mousePos = 0;
	int i;


	ePosition pos = ePosition::DOWN;

	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cerr << "[-] ERROR - Failed to initialise SDL (" << SDL_GetError() << ")\n";
			return EXIT_FAILURE;
		}
	}

	if (TTF_Init()) {
		std::cerr << "Failed to init TTF.\n";
		return EXIT_FAILURE;
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
		std::cerr << "[-] ERROR - Failed to initialise SDL_Image (" << SDL_GetError() << ")\n";
		return EXIT_FAILURE;
	}

	window = SDL_CreateWindow("Skoualotron", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cerr << "[-] ERROR - Failed to create SDL window (" << SDL_GetError() << ")\n";
		return EXIT_FAILURE;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Game* game = loadConf("data/characters.jdc", "data/weapons.jdc", renderer);

	Uint32 lastFrameTime = SDL_GetTicks();

	while (game->m_currentState != game_state::EXIT)
	{
		while (SDL_PollEvent(&e))
		{
			switch (game->m_currentState)
			{
			case game_state::MENU:
				game = handlerTitleScreen(game, e, renderer);
				break;
			case game_state::CHARACTER_DRAFT:
				game = handlerCharacterDraft(game, e, renderer);
				break;
			case game_state::WEAPON_DRAFT:
				game = handlerWeaponDraft(game, e, renderer);
				break;
			case game_state::ON_PROGRESS:
				game = handlerGameOnProgress(game, e, renderer);
				break;
			case game_state::LEFT_TEAM_WIN:
				game = handlerLeftTeamWin(game, e, renderer);
				break;
			case game_state::RIGHT_TEAM_WIN:
				game = handlerRightTeamWin(game, e, renderer);
				break;
			default:
				break;
			}
		}

		SDL_GetMouseState(&x_mousePos, &y_mousePos);

		timeElapsed = SDL_GetTicks() - lastFrameTime;
		if (timeElapsed > timeStep) {
			while (timeElapsed > timeStep) { //Get back to real-time
				game->render(renderer, x_mousePos, y_mousePos);
				SDL_RenderPresent(renderer);
				timeElapsed -= timeStep;
				lastFrameTime = SDL_GetTicks();
			}
		}
		else { //Wait to save UC
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	//Destroying game
	if (game)
		delete game;
	
	SDL_DestroyRenderer(renderer);

	//Destroying window
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
