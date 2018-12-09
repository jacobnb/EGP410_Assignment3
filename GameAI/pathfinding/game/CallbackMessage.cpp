#include "CallbackMessage.h"
#include "Unit.h"
CallbackMessage::CallbackMessage(void (Unit::*functionPointer)(void))
	:GameMessage(CALLBACK_MESSAGE)
	, callBack(functionPointer)
{
}

CallbackMessage::~CallbackMessage()
{
}

void CallbackMessage::process()
{
	callBack();
}
