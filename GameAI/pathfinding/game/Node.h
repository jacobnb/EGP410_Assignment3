#pragma once

#include <Trackable.h>
#include "Vector2D.h"

const int BAD_NODE_ID = -1;
#define NODE_ID int

//==Node class that defines nodes in the graph==//
class Node:public Trackable
{
public:
	Node( const NODE_ID& id );
	Node();
	~Node();

	const NODE_ID& getId() const {return mId;};
	inline void setCost(const float newCost) { mCost = newCost; };
	inline float getCost() { return mCost; };
	
	void setDir(Node* target);

	//units utilizing flow field get this for movement
	inline Vector2D getDir() { return mDir; };

private:
	const NODE_ID mId;
	float mCost;
	float mHeuristicCost;
	Node* mpPrevNode;
	Vector2D mDir;
};