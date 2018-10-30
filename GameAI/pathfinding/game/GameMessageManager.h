#pragma once

#include <list>
#include "Trackable.h"

class GameMessage;

//==Game messaging system manager==//
class GameMessageManager: public Trackable
{
public:
	GameMessageManager();
	~GameMessageManager();

	void addMessage( GameMessage* pMessage, int delay );//note - message manager will now "own" the message and will delete it at its pleasure
	void processMessagesForThisframe();

private:

	std::list<GameMessage*> mMessages;
};

