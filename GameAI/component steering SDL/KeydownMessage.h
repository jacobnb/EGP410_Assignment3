#pragma once

#include "GameMessage.h"
#include "Vector2D.h"
#include "SDL_scancode.h"

enum KeyType { //using this because Dean required it last year for abstraction;
	KEY_A = SDL_SCANCODE_A,
	KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
	KEY_1 = SDL_SCANCODE_1,
	KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
	KEY_ENTER = SDL_SCANCODE_RETURN,
	KEY_ESCAPE, KEY_BACKSPACE, KEY_TAB, KEY_SPACE, KEY_MINUS, KEY_EQUALS, KEY_LEFTBRACKET, KEY_RIGHTBRACKET, KEY_BACKSLASH,
};
class KeydownMessage :public GameMessage
{
public:
	KeydownMessage(const KeyType& key);
	~KeydownMessage();

	void process();

private:
	KeyType mKey;
};