#include "KeydownMessage.h"
#include "Game.h"
#include "GameMessageManager.h"
#include "GameApp.h"
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
		//Dijkstra's
		static_cast<GameApp*>(gpGame)->changeToDijkstra();
		break;
	}
	case KEY_ESCAPE: {
		static_cast<GameApp*>(gpGame)->markForExit();
		break;
	}
	case KEY_F:
	{
		//DFS
		static_cast<GameApp*>(gpGame)->changeToDFS();
		break;
	}
	case KEY_A: {
		//A*
		static_cast<GameApp*>(gpGame)->changeToAStar();
		break;
	}
	}
}
