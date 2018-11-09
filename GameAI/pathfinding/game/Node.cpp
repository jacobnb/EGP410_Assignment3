#include "Node.h"
#include "Grid.h"
#include "GameApp.h"

Node::Node()
	:mId(BAD_NODE_ID)
{
}

Node::Node(const NODE_ID& id)
	: mId(id)
{
}

Node::~Node()
{
}

void Node::setDir(Node* target)
{
	Grid* tempGrid = static_cast<GameApp*>(gpGame)->getGrid();
	Vector2D pos1 = tempGrid->getULCornerOfSquare(target->getId());
	Vector2D pos2 = tempGrid->getULCornerOfSquare(mId);

	mDir = (pos1 - pos2);
	mDir.normalize();
}