#pragma once

#include <string>
#include <Trackable.h>

//==Base Debug Content class==//
class DebugContent:public Trackable
{
public:
	DebugContent(){};
	virtual ~DebugContent(){};

	virtual std::string getDebugString() = 0;
};