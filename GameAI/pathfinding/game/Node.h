#pragma once

#include <Trackable.h>

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
	inline void setPrevNode(Node* prevNode) { mpPrevNode = prevNode; };
	inline Node* getPrevNode() { return mpPrevNode ? mpPrevNode : NULL; };
	inline void setHeuristic(float cost) { mHeuristicCost = cost; };
	inline float getTotalCost() { return mHeuristicCost + mCost; };

private:
	const NODE_ID mId;
	float mCost;
	float mHeuristicCost;
	Node* mpPrevNode;
};