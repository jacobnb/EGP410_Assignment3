#include "InputSystem.h"
#include <stdio.h>
#include <assert.h>

#include <sstream>
#include <SDL.h>

#include "GameMessage.h"
#include "Vector2D.h"
#include "Defines.h"
#include "Game.h"
#include "KeydownMessage.h"
#include "GameMessageManager.h"
#include "GameApp.h"
#include "PathToMessage.h"
InputSystem::InputSystem()
{
	mPrevX = 0;
	mPrevY = 0;
}

InputSystem::~InputSystem()
{

}

void InputSystem::update()
{
	SDL_Event nextEvent;
	updateMouseEvents();
	while (true)
	{
		if (!SDL_PollEvent(&nextEvent)) {
			return;
		}
		GameMessage * pMessage = NULL;
		switch (nextEvent.type)
		{
		case SDL_KEYDOWN:
			pMessage = new KeydownMessage(KeyType(nextEvent.key.keysym.scancode));
			static_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
			break;
		case SDL_KEYUP:
			break;
		case SDL_QUIT: //clicks on red X
			pMessage = new KeydownMessage(KeyType(SDL_SCANCODE_ESCAPE));
			static_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
			break;
		default:
			break;

		}
	}
}

void InputSystem::clearQueue()
{
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_APP_LOWMEMORY, SDL_LASTEVENT);
}


void InputSystem::updateMouseEvents()
{
	if (SDL_GetMouseState(&mXMouse, &mYMouse) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		if (mXMouse != mPrevX && mYMouse != mPrevY) {

			Vector2D pos(mXMouse, mYMouse);
			Vector2D prevPos(mPrevX, mPrevY);

			GameMessage* pMessage = new PathToMessage(prevPos, pos);
			static_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
			mPrevX = mXMouse;
			mPrevY = mYMouse;
		}
	}

}

