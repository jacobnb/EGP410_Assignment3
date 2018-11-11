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

	//TODO Mike can you remove whatever vars you don't need here?
	inline void setCost(const float newCost) { mCost = newCost; };
	inline float getCost() { return mCost; };
	inline void setPrevNode(Node* prevNode) { mpPrevNode = prevNode; };
	inline Node* getPrevNode() { return mpPrevNode ? mpPrevNode : NULL; };
	inline void setHeuristic(float cost) { mHeuristicCost = cost; };
	inline float getTotalCost() { return mHeuristicCost + mCost; };
	//For flowfield?
	void setDir(Node* target);
	inline Vector2D getDir() { return mDir; };

private:
	const NODE_ID mId;
	float mCost;
	float mHeuristicCost;
	Node* mpPrevNode;
	Vector2D mDir;
};