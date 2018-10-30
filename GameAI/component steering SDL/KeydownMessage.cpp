#include "KeydownMessage.h"
#include "Game.h"
#include "UnitManager.h"
#include "DataLoader.h"
#include "FlockMessage.h"
#include "GameMessageManager.h"
KeydownMessage::KeydownMessage(const KeyType& key)
	:GameMessage(KEYDOWN_MESSAGE)
	, mKey(key)
{
}

KeydownMessage::~KeydownMessage()
{
}

void KeydownMessage::process()
{
	switch (mKey) {
		case KEY_D: {
			//should also delete a random boid
			gpGame->deleteRandomEnemyUnit();
			break;
		}
		case KEY_ESCAPE: {
			gpGame->getDataLoader()->writeData();
			gpGame->quitLoop();
			break;
		}
		case KEY_ENTER:
		{
			gpGame->spawnEnemyAtRandomLoc();
			break;
		}
		case KEY_A: {
			gpGame->spawnTenBoids();
			break;
		}
	//==Flocking Message Cases==//
		case KEY_R: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::COHESION_FACTOR, false);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break; 
		}case KEY_T: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::COHESION_FACTOR, true);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break; 
		}case KEY_F: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::SEPARATION_FACTOR, false);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_G: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::SEPARATION_FACTOR, true);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_V: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::ALIGNMENT_FACTOR, false);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_B: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::ALIGNMENT_FACTOR, true);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_Y: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::WANDER_FACTOR, false);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_U: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::WANDER_FACTOR, true);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_H: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::COHESION_RADIUS, false);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_J: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::COHESION_RADIUS, true);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_N: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::ALIGNMENT_RADIUS, false);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_M: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::ALIGNMENT_RADIUS, true);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_I: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::SEPARATION_RADIUS, false);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}case KEY_O: {
			GameMessage* pMessage = new FlockMessage(FlockParameter::SEPARATION_RADIUS, true);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			break;
		}
	}
}
