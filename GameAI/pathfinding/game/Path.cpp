#include "Path.h"
#include "Node.h"
Path::Path()
{
}

Path::~Path()
{
}

Node* Path::peekNode(int index) const
{
	if ((unsigned int)index < mNodes.size())
	{
		return mNodes[index];
	}
	else
	{
		return NULL;
	}
}

Node* Path::peekNextNode() const
{
	if (mNodes.size() > 0)
	{
		return mNodes.back();
	}
	else
	{
		return NULL;
	}
}

Node* Path::getAndRemoveNextNode()
{
	if (mNodes.size() > 0)
	{
		Node* pNode = mNodes.back();
		mNodes.pop_back();
		return pNode;
	}
	else
	{
		return NULL;
	}
}

void Path::removeNode(Node * pNode)
{
	auto nodeIter = mNodes.begin();
	while (nodeIter != mNodes.end()) {
		if ((*nodeIter)->getId() == pNode->getId()) {
			mNodes.erase(nodeIter);
			return;
		}
		++nodeIter;
	}
}

void Path::addNode(Node* pNode)
{
	mNodes.push_back(pNode);
}

bool Path::containsNode(Node* pNode)
{
	bool retVal = false;

	for (unsigned int i = 0; i < mNodes.size(); i++)
	{
		if (mNodes[i]->getId() == pNode->getId())
		{
			retVal = true;
			break;
		}
	}
	return retVal;
}
