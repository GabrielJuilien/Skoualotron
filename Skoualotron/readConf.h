#pragma once
#include "Game.h"
#include <fstream>
#include <iostream>

Game* loadConf(std::string charactersPath, std::string weaponsPath, SDL_Renderer* renderer);