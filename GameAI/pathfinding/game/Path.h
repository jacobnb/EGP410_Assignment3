#pragma once

#include <vector>
#include <Trackable.h>

class Node;

//==Wrapper for a vector that describes the path found by the pathfinders==//
class Path:public Trackable
{
public:
	Path();
	~Path();

	Node* peekNode( int index ) const;
	Node* peekNextNode() const;
	Node* getAndRemoveNextNode();
	bool containsNode( Node* pNode );
	int getNumNodes() const { return mNodes.size(); };
	void removeNode(Node* pNode);
	void addNode( Node* pNode );

private:
	std::vector<Node*> mNodes;

};