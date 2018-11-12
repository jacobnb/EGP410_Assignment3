#pragma once
#include "Trackable.h"
#include <queue>
class Node;
class PathfindManager : public Trackable {
public:
	PathfindManager();
	~PathfindManager();
	void update(float timeToRun);
	void addPathToFind(Node* pFromNode, Node* pToNode, int mIdNum);

private:
	struct PathRequest {
		PathRequest(Node* pFromNode, Node* pToNode, int mIdNum) {
			mId = mIdNum;
			pFrom = pFromNode;
			pTo = pToNode;
		}
		int mId;
		Node* pFrom;
		Node* pTo;
		~PathRequest() {};
	};
	std::queue<PathRequest*> mPathFindRequests;
	PathRequest* currentRequest;
};