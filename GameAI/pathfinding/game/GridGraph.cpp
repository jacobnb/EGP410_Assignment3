#include "GridGraph.h"
#include "Connection.h"
#include "Node.h"
#include "Grid.h"
#include "Game.h"
#include <vector>

GridGraph::GridGraph(Grid* pGrid)
	:Graph()
	, mpGrid(pGrid)
{
}

GridGraph::~GridGraph()
{
}

void GridGraph::init()
{
	if (mpGrid != NULL)
	{
		int numGridIndices = mpGrid->getGridWidth() * mpGrid->getGridHeight();

		//size the node vector
		mNodes.resize(numGridIndices, NULL);

		//go through all elements of the grid and make Nodes for them - also add them to Node vector
		for (int i = 0; i < numGridIndices; i++)
		{
			//create a Node for this location
			Node* pNode = new Node(i);
			//add to NodeVector
			mNodes[i] = pNode;
		}

		//go through all elements of the grid - get adjacencies and create connections for each
		for (int i = 0; i < numGridIndices; i++)
		{
			//get the Node for this location
			Node* pFromNode = mNodes[i];

			//get all the adjacencies
			std::vector<int> adjacencies = mpGrid->getAdjacentIndices(i);

			//create a vector of Connections
			std::vector<Connection*> connections;

			//go through all adjacent indices, get the nodes for them, create connections, and add to connections map
			for (unsigned int adjIndex = 0; adjIndex < adjacencies.size(); adjIndex++)
			{
				//check for blocking terrain
				if (mpGrid->getValueAtIndex(adjacencies[adjIndex]) != BLOCKING_VALUE)
				{
					Node* pToNode = mNodes[adjacencies[adjIndex]];//find to node

					//Check if node is diagonal, if it is set cost to root(2);
					//Could get node positions and check line direction.
					auto fromNodeLoc = mpGrid->getULCornerOfSquare(pFromNode->getId());
					auto toNodeLoc = mpGrid->getULCornerOfSquare(pToNode->getId());
					Connection* pConnection;
					//if nodes are on same x or y axis
					//This is comparing floats, but they should be exact.
					if (fromNodeLoc.getX() == toNodeLoc.getX()
						|| fromNodeLoc.getY() == toNodeLoc.getY()) {
						pConnection = new Connection(pFromNode, pToNode, mConnectionCost);
						//create a connection
					}
					//if nodes are diagonal.
					else {
						pConnection = new Connection(pFromNode, pToNode, sqrt(2*mConnectionCost));
					}

					//add connection to connection vectors
					mConnections.push_back(pConnection);
					connections.push_back(pConnection);
				}
			}

			//put temp connection vector into map
			mConnectionMap[i] = connections;
		}
	}
}
