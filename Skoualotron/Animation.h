#pragma once

#include <string>
#include "../librairies/SDL2-2.0.6/include/SDL.h"
#include "../librairies/SDL2-2.0.6/include/SDL_image.h"
#include "../librairies/SDL2-2.0.6/include/SDL_ttf.h"




enum class SIDE { LEFT, RIGHT };

class Animation {
private:
	SDL_Surface* m_surface;
	SDL_Texture* m_texture;
	SDL_Rect* m_source;
	SDL_Rect* m_dest;
	SDL_Renderer* m_renderer;
	
	int m_length; //Total number of frames
	int m_currentFrame;
	bool m_loop; //Animation is looping
	bool m_play; //Animation is playing
	SIDE m_side;

	Animation();
public:
	Animation(std::string p_path, SDL_Renderer* p_renderer, int p_length, int p_currentFrame, bool p_loop, bool p_play, int x_spriteSize, int y_spriteSize, int x_spritePos, int y_spritePos, SIDE p_side);
	Animation(std::string p_path, SDL_Renderer* p_renderer, int length, bool p_loop, bool p_play);
	Animation(std::string p_text, SDL_Renderer* p_renderer, int p_length);
	Animation(std::string p_text, SDL_Renderer* p_renderer);

	//Setters
	void play(bool p_play);
	void loop(bool p_loop);
	void changeTexture(std::string p_path);
	void setPos(int x, int y);

	void flip(SIDE p_side);

	//Getters
	bool play();
	bool loop();
	SDL_Renderer* renderer();
	int get_xPos();
	int get_yPos();
	SIDE getSide();

	void render();
	bool hover(int x_mousePos, int y_mousePos);

	~Animation();
};


