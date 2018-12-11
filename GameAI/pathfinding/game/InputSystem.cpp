#include "InputSystem.h"
#include <stdio.h>
#include <assert.h>

#include <sstream>
#include <SDL.h>
#include <vector>

#include "GameMessage.h"
#include "Vector2D.h"
#include "Defines.h"
#include "Game.h"
#include "KeydownMessage.h"
#include "GameMessageManager.h"
#include "GameApp.h"
#include "PathToMessage.h"
#include "pathfindingMessage.h"
#include "UnitManager.h"

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
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (SDL_GetMouseState(&mXMouse, &mYMouse) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		if(gpGame->getAIFight()){
			return;
		}
		if (mXMouse != mPrevX && mYMouse != mPrevY)
		{
			Vector2D pos(mXMouse, mYMouse);

			if (pGame->checkFlow())
			{
				GameMessage* pMessage = new PathfindingMessage(pos, 0);
				static_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
			}
			else
			{
				Unit* player = gpGame->getUnitManager()->getPlayerUnit();
				Vector2D unitPos;
				if(player->getNextPos() != NULL){
					unitPos = player->getNextPos();
				}
				else {
					unitPos = player->getPositionComponent()->getPosition();
				}
				GameMessage* pMessage = new PathToMessage(unitPos, pos, 0);
				static_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
				mPrevX = mXMouse;
				mPrevY = mYMouse;
			}		
		}
	}

}

