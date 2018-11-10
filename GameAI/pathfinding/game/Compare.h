#pragma once


//This is for the priority queue
//https://stackoverflow.com/questions/16749723/how-i-can-find-value-in-priority-queue
//===Comparison class to use with the priority queue for Dijkstra===//
class Compare {
public:
	//priority queue prioritizes largest. 
	//compare by default returns lhs < rhs, we want the opposite.
	inline bool operator() (Node* node1, Node* node2)
	{ //return LHS > RHS
		return node1->getCost() > node2->getCost();
	};
};

//===Comparison class to use with the priority queue for A*===//
class CompareAStar {
public:
	inline bool operator() (Node* lhs, Node* rhs) {
		return lhs->getTotalCost() > rhs->getTotalCost();
	}
};

//===Comparison class to use with the priority queue for node structs with Dijkstra===//
class CompareDijkstraStruct {
public:
	inline bool operator() (GridPathfinder::NodeStruct* lhs, GridPathfinder::NodeStruct* rhs) {
		return lhs->mCost > rhs->mCost;
	}
};

//===Comparison class to use with the priority queue for node struct with A*===//
class CompareAStarStruct {
public:
	inline bool operator() (GridPathfinder::NodeStruct* lhs, GridPathfinder::NodeStruct* rhs) {
		return lhs->totalCost() > rhs->totalCost();
	}
};