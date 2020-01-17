#include "Animation.h"

#ifndef FONT_LOADED
#define FONT_LOADED
TTF_Font* font = NULL;
SDL_Color fontColor = { 0xff, 0xff, 0xff }, backgroundColor = { 0x22, 0x22, 0x22 };
#endif

Animation::Animation() {
	m_surface = NULL;
	m_texture = NULL;
	m_source = NULL;
	m_dest = NULL;
	m_renderer = NULL;

	m_length = 0;
	m_currentFrame = 0;
	m_loop = false;
	m_play = false;

	m_side = SIDE::LEFT;
}

Animation::Animation(std::string p_path, SDL_Renderer* p_renderer, int p_length, int p_currentFrame, bool p_loop, bool p_play, int x_spriteSize, int y_spriteSize, int x_spritePos, int y_spritePos, SIDE p_side) {	
	m_surface = IMG_Load(p_path.c_str());

	m_texture = SDL_CreateTextureFromSurface(p_renderer, m_surface);

	//Destroying surface
	SDL_FreeSurface(m_surface);

	m_source = new SDL_Rect();
	m_dest = new SDL_Rect();
	m_renderer = p_renderer;

	m_length = p_length;
	m_currentFrame = p_currentFrame;
	m_loop = p_loop;
	m_play = p_play;

	//Cutting spritesheet. For convenience, spritesheet's number of frames is a multiple of 5
	m_source->w = x_spriteSize;
	m_source->h = y_spriteSize;
	m_source->x = x_spriteSize * (p_currentFrame % 5);
	m_source->y = y_spriteSize * (p_currentFrame / 5);

	m_dest->w = x_spriteSize;
	m_dest->h = y_spriteSize;
	m_dest->x = x_spritePos;
	m_dest->y = y_spritePos;

	m_side = p_side;
}

Animation::Animation(std::string p_path, SDL_Renderer* p_renderer, int length, bool p_loop, bool p_play) {
	m_surface = IMG_Load(p_path.c_str());

	m_texture = SDL_CreateTextureFromSurface(p_renderer, m_surface);

	//Destroying surface
	SDL_FreeSurface(m_surface);

	m_source = new SDL_Rect();
	m_dest = new SDL_Rect();
	m_renderer = p_renderer;

	m_length = length;
	m_currentFrame = 0;
	m_loop = p_loop;
	m_play = p_play;

	//Cutting spritesheet. For convenience, spritesheet's number of frames is a multiple of 5
	m_source->w = 1920;
	m_source->h = 1080;
	m_source->x = 0;
	m_source->y = 0;

	m_dest->w = 1920;
	m_dest->h = 1080;
	m_dest->x = 0;
	m_dest->y = 0;

	m_side = SIDE::LEFT;
}

Animation::Animation(std::string p_text, SDL_Renderer* p_renderer, int p_length) {
	font = TTF_OpenFont("data/NeonFuture.ttf", 30);
	m_surface = TTF_RenderText_Blended_Wrapped(font, p_text.c_str(), { 0xff, 0x00, 0xf7 }, 400);
	m_texture = SDL_CreateTextureFromSurface(p_renderer, m_surface);
	TTF_CloseFont(font);

	//Destroying surface
	SDL_FreeSurface(m_surface);

	m_source = new SDL_Rect();
	m_dest = new SDL_Rect();
	m_renderer = p_renderer;

	m_length = p_length;
	m_currentFrame = 0;
	m_loop = false;
	m_play = false;

	SDL_QueryTexture(m_texture, NULL, NULL, &(m_source->w), &(m_source->h));
	m_source->x = 0;
	m_source->y = 0;

	SDL_QueryTexture(m_texture, NULL, NULL, &(m_dest->w), &(m_dest->h));
	m_dest->x = 0;
	m_dest->y = 0;

	m_side = SIDE::LEFT;
}

Animation::Animation(std::string p_text, SDL_Renderer* p_renderer) {
	font = TTF_OpenFont("data/arial.ttf", 15);
	m_surface = TTF_RenderText_Blended_Wrapped(font, p_text.c_str(), fontColor, 400);
	m_texture = SDL_CreateTextureFromSurface(p_renderer, m_surface);
	TTF_CloseFont(font);

	//Destroying surface
	SDL_FreeSurface(m_surface);

	m_source = new SDL_Rect();
	m_dest = new SDL_Rect();
	m_renderer = p_renderer;

	m_length = 1;
	m_currentFrame = 0;
	m_loop = true;
	m_play = true;

	SDL_QueryTexture(m_texture, NULL, NULL, &(m_source->w), &(m_source->h));
	m_source->x = 0;
	m_source->y = 0;

	SDL_QueryTexture(m_texture, NULL, NULL, &(m_dest->w), &(m_dest->h));
	m_dest->x = 0;
	m_dest->y = 0;

	m_side = SIDE::LEFT;
}

void Animation::play(bool p_play) {
	m_play = p_play;
}

void Animation::loop(bool p_loop) {
	m_loop = p_loop;
}

void Animation::setPos(int x, int y) {
	m_dest->x = x;
	m_dest->y = y;
}

void Animation::flip(SIDE p_side) {
	m_side = p_side;
}

void Animation::changeTexture(std::string p_path) {
	//Destroying old texture
	SDL_DestroyTexture(m_texture);

	//Loading new texture
	SDL_Surface* tmp = IMG_Load(p_path.c_str());
	m_texture = SDL_CreateTextureFromSurface(m_renderer, tmp);

	//destroying surface
	SDL_FreeSurface(tmp);
}

bool Animation::play() {
	return m_play;
}

bool Animation::loop() {
	return m_loop;
}

SDL_Renderer* Animation::renderer() {
	return m_renderer;
}


int Animation::get_xPos() {
	return m_dest->x;
}

int Animation::get_yPos() {
	return m_dest->y;
}

SIDE Animation::getSide() {
	return m_side;
}


void Animation::render() {
	int w, h;
	if (m_play) {
		//Copying texture to renderer
		if (m_side == SIDE::LEFT)
			SDL_RenderCopy(m_renderer, m_texture, m_source, m_dest);
		else
			SDL_RenderCopyEx(m_renderer, m_texture, m_source, m_dest, 0, NULL, SDL_FLIP_HORIZONTAL);

		SDL_QueryTexture(m_texture, NULL, NULL, &w, &h);

		m_currentFrame++;
		if (m_currentFrame == m_length) {
			m_currentFrame = 0;
			if (!m_loop) {
				m_play = false;
				return;
			}
		}
		
		if (m_dest->w == w) { //If static image
			return;
		}
		else if (m_source->h != 1080) {
			//Cutting next sprite
			m_source->x = m_source->w * (m_currentFrame % 5);
			m_source->y = m_source->h * (m_currentFrame / 5);
		}
		else {

			m_source->x = m_source->w * (m_currentFrame % 4);
			m_source->y = m_source->h * (m_currentFrame / 4);
		}
	}
}

bool Animation::hover(int x_mousePos, int y_mousePos) {
	if (x_mousePos > m_dest->x&& x_mousePos < m_dest->x + m_dest->w && y_mousePos > m_dest->y&& y_mousePos < m_dest->y + m_dest->h) {
		return true;
	}
	return false;
}

Animation::~Animation() {
	if (m_texture)
		SDL_DestroyTexture(m_texture);
	if (m_source)
		free(m_source);
	if(m_dest)
		free(m_dest);
}
