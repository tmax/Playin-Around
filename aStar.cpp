#include "Astar.h"
using namespace std;

Anode::Anode()
{
	this->node = NULL;
	this->Fvalue = 0;
	this->Gvalue = 0;
	this->Hvalue = 0;
	this->parent = NULL;
}

Anode::Anode(GridNode *n, int f, int g, int h, Anode* p)
{
	this->node = n;		//initilize 
	this->Fvalue = f;
	this->Gvalue = g;
	this->Hvalue = h;
	this->parent = p;
}

void Anode::setFvalue(int f)
{
	this->Fvalue = f;
}

void Anode::setGvalue(int g)
{
	this->Gvalue = g;
}

void Anode::setHvalue(int h)
{
	this->Hvalue = h;
}

int Anode::getFvalue()
{
	return this->Fvalue;
}

int Anode::getGvalue()
{
	return this->Gvalue;
}

int Anode::getHvalue()
{
	return this->Hvalue;
}

Astar::Astar(Grid* g, GridNode* startNode, GridNode* goalNode)
{
	this->grid = g;
	this-> start = startNode;
	this->goal = goalNode;
}

void Astar::setGrid(Grid* g)
{
	this->grid = g;
}

void Astar::setStartNode(GridNode* node)
{
	this->start = node;
}

void Astar::setGoalNode(GridNode* node)
{
	this->goal = node;
}

void Astar::addNodesToOpen(Anode* node)//adds all the adjacent nodes of the current node to the openlist
{
	//Take the node passed in and get its corresponding GridNode in the Grid
	//Then, get the surrounding nodes, one by one, and make a new Anode for each one, linking the GridNode to the newly made Anode
	//Then link all of the newly made Anodes with their parent, the original Anod that was passed into the method and store them in OpenList

	OpenListType::iterator it;//get our variables reddy
	Anode* anode;
	GridNode* adjnode;
	int manhattan;
	int G;
	int F;

	//now we must set up each possible surrounding node
	//they are N,E,S,W,NE,SE,SW,NW so...let's check 'em

	GridNode* north = grid->getNorthNode(node->getGridNode());

	if(north != NULL &&	north->isClear())
		//does the node exist if so, is that node clear?
	{
		it = openList.find(north->getID());	//if so...look for that node in openlist		
		
		if(it == openList.end())
		{//if iterator has reached the end of the list, that means it AINT in the list
			//and in that case make a new node and put it in the list, 
			//along with some other necessary pieces of info
		adjnode = north;//set the adjacent node to be this node
		G = node->getGvalue() + grid->getHeight();//get the node's gvalue and also add the height since it is a N/E/S/W node
		manhattan = grid->getManhattan(north, this->goal);
		F = G + manhattan;
		anode = new Anode(adjnode, F, G, manhattan, node);
		openList.insert(make_pair(anode->getGridNode()->getID(), anode));
		}
	}

	//now, check for the rest

	GridNode* east = grid->getEastNode(node->getGridNode());

	if(east != NULL &&//does the node exist
		east->isClear())//if so, is that node clear?
	{
		it = openList.find(east->getID());	//if so...look for that node in openlist		
		
		if(it == openList.end())
		{//if iterator has reached the end of the list, that means it AINT in the list
			//and in that case make a new node and put it in the list, 
			//along with some other necessary pieces of info
		adjnode = east;//set the adjacent node to be this node
		G = node->getGvalue() + grid->getHeight();//get the node's gvalue and also add the height since it is a N/E/S/W node
		manhattan = grid->getManhattan(east, this->goal);
		F = G + manhattan;
		anode = new Anode(adjnode, F, G, manhattan, node);
		openList.insert(make_pair(anode->getGridNode()->getID(), anode));
		}
	}

	GridNode* south = grid->getSouthNode(node->getGridNode());

	if(south != NULL &&//does the node exist
		south->isClear())//if so, is that node clear?
	{
		it = openList.find(south->getID());	//if so...look for that node in openlist		
		
		if(it == openList.end())
		{//if iterator has reached the end of the list, that means it AINT in the list
			//and in that case make a new node and put it in the list, 
			//along with some other necessary pieces of info
		adjnode = south;//set the adjacent node to be this node
		G = node->getGvalue() + grid->getHeight();//get the node's gvalue and also add the height since it is a N/E/S/W node
		manhattan = grid->getManhattan(south, this->goal);
		F = G + manhattan;
		anode = new Anode(adjnode, F, G, manhattan, node);
		openList.insert(make_pair(anode->getGridNode()->getID(), anode));
		}
	}

	GridNode* west = grid->getWestNode(node->getGridNode());

	if(west != NULL &&//does the node exist
		west->isClear())//if so, is that node clear?
	{
		it = openList.find(west->getID());	//if so...look for that node in openlist		
		
		if(it == openList.end())
		{//if iterator has reached the end of the list, that means it AINT in the list
			//and in that case make a new node and put it in the list, 
			//along with some other necessary pieces of info
		adjnode = west;//set the adjacent node to be this node
		G = node->getGvalue() + grid->getHeight();//get the node's gvalue and also add the height since it is a N/E/S/W node
		manhattan = grid->getManhattan(west, this->goal);
		F = G + manhattan;
		anode = new Anode(adjnode, F, G, manhattan, node);
		openList.insert(make_pair(anode->getGridNode()->getID(), anode));
		}
	}

	GridNode* ne = grid->getNENode(node->getGridNode());

	if(ne != NULL &&//does the node exist
		ne->isClear())//if so, is that node clear?
	{
		it = openList.find(north->getID());	//if so...look for that node in openlist		
		
		if(it == openList.end())
		{//if iterator has reached the end of the list, that means it AINT in the list
			//and in that case make a new node and put it in the list, 
			//along with some other necessary pieces of info
		adjnode = ne;//set the adjacent node to be this node
		G = node->getGvalue() + grid->getHeight();//get the node's gvalue and also add the height since it is a N/E/S/W node
		manhattan = grid->getManhattan(ne, this->goal);
		F = G + manhattan;
		anode = new Anode(adjnode, F, G, manhattan, node);
		openList.insert(make_pair(anode->getGridNode()->getID(), anode));
		}
	}

	GridNode* se = grid->getSENode(node->getGridNode());

	if(se != NULL &&//does the node exist
		se->isClear())//if so, is that node clear?
	{
		it = openList.find(se->getID());	//if so...look for that node in openlist		
		
		if(it == openList.end())
		{//if iterator has reached the end of the list, that means it AINT in the list
			//and in that case make a new node and put it in the list, 
			//along with some other necessary pieces of info
		adjnode = se;//set the adjacent node to be this node
		G = node->getGvalue() + grid->getHeight();//get the node's gvalue and also add the height since it is a N/E/S/W node
		manhattan = grid->getManhattan(se, this->goal);
		F = G + manhattan;
		anode = new Anode(adjnode, F, G, manhattan, node);
		openList.insert(make_pair(anode->getGridNode()->getID(), anode));
		}
	}

	GridNode* sw = grid->getSWNode(node->getGridNode());

	if(sw != NULL &&//does the node exist
		sw->isClear())//if so, is that node clear?
	{
		it = openList.find(sw->getID());	//if so...look for that node in openlist		
		
		if(it == openList.end())
		{//if iterator has reached the end of the list, that means it AINT in the list
			//and in that case make a new node and put it in the list, 
			//along with some other necessary pieces of info
		adjnode = sw;//set the adjacent node to be this node
		G = node->getGvalue() + grid->getHeight();//get the node's gvalue and also add the height since it is a N/E/S/W node
		manhattan = grid->getManhattan(sw, this->goal);
		F = G + manhattan;
		anode = new Anode(adjnode, F, G, manhattan, node);
		openList.insert(make_pair(anode->getGridNode()->getID(), anode));
		}
	}

	GridNode* nw = grid->getNWNode(node->getGridNode());

	if(nw != NULL &&//does the node exist
		nw->isClear())//if so, is that node clear?
	{
		it = openList.find(nw->getID());	//if so...look for that node in openlist		
		
		if(it == openList.end())
		{//if iterator has reached the end of the list, that means it AINT in the list
			//and in that case make a new node and put it in the list, 
			//along with some other necessary pieces of info
		adjnode = nw;//set the adjacent node to be this node
		G = node->getGvalue() + grid->getHeight();//get the node's gvalue and also add the height since it is a N/E/S/W node
		manhattan = grid->getManhattan(nw, this->goal);
		F = G + manhattan;
		anode = new Anode(adjnode, F, G, manhattan, node);
		openList.insert(make_pair(anode->getGridNode()->getID(), anode));
		}
	}
	//this->openList.add(node);
}

void Astar::moveNodeToClosed(Anode* node)//puts an 'already-considered' node into the closed list
{
	//as we proceed through the openlist, we add onto the
	//closedlist items that have been considered
	//it is with teh closed list that we will eventually
	//build our PATH...
	
	OpenListType::iterator it;//iterator to go over the OL
	it = openList.find(node->getGridNode()->getID());//get the id of the node since that is the key
	if(it != openList.end())//if our search is fruitful
	{
		openList.erase(it); //remove that thang from the openlist
		//then insert that joint into the closed list!!!
		closedList.insert(closedList.end(), node);
		//remember closedlist is a list<Anode*>
		//and we're sticking things in at the end
	}

	//this->closedList.insert(node);
}

Anode* Astar::getNextNode()//gets next Anode for consideration
{
	// method for retrieval of next node is per the algorithm:
	// we inspect all avaiable nodes on the openlist
	// find node with LOWEST f score and add its ajeacent nodes
	// make these new adjacent nodes have a parent of current node
	// BUT----if there are adjacents that are already on the openlist
	// check if the g value is lower to go thru current node
	// and if so, make that node's paren the current node

	OpenListType::iterator iter = openList.begin();//get an OLT iterator 

	Anode* nextAnode = NULL;//initally set to null
	int currentMin = INT_MAX;//initally set the min to the largest possible int
	//loop through the openlist
	//for(iter = openList.begin(); iter != openList.end(); ++iter)
	while(iter != openList.end())
	{
		//remember openList is a map (key : value)
		//if its second value is less than INT_MAX (used for 
		//comparison purposes)
		//set the next node to be that node
		if(iter->second->getFvalue() < currentMin)//if we have a new min
		{
			nextAnode = iter->second;//then we're goin here next
			currentMin = nextAnode->getFvalue();
		}
		iter++;
	}
	
	//after finding lowest F score, return the next Node;
	
	return nextAnode;

}


bool Astar::findPath()//is there a walkable path from start to goal?
{
	//what we need to do is start from the start_node and keep inspecting nodes until we reach the goal_node and it is added to the closed list

	Anode* current = new Anode(this->start); 
	//since this is the starting node, we must manually do the base case of adding it to the openlist

	this->openList.insert(make_pair(current->getGridNode()->getID(), current));
	
	bool goalReached = false;
	if(!(this->goal->isClear()))
		return false;

	while(!goalReached)//we loop until we add the goal node into the closed list
	{
		if(current->getGridNode() == NULL)//if current node is NULL break out
			break;
		if(current->getGridNode() == this->goal)//if we have reached the goal, set our flag to true
			goalReached = true;

		moveNodeToClosed(current);//if we haven't fallen into the above conditionals, move the current node into closed list

		addNodesToOpen(current);//add all the surrounding nodes of current into the openlist

		current = getNextNode();//then get the next best node and we loop again
	}

	//now if we have made it out of the while loop, we seem to have inserted the goal node onto the closed list;
	//we need to now follow the goal's parent pointers until we reach start

	//if(closedList.size() < 1)//if there are not any elements in the closed list then return false b/c there is no path apparently
	//	return false;

	//now go into the closed list and extract the gridnodes in the order they should be traveled, so start from the back

	current = closedList.back();//this SHOULD be the goal node; we want to follow its parent all the way to the start
	path.clear();
	while(current != NULL && current->getGridNode()->getID() != this->start->getID())
	{
		//path.insert(path.end(), current->getGridNode());//add it to path
		path.insert(path.begin(), current->getGridNode());
		current = current->getParent();//get the parent
	}

	if(current->getGridNode()->getID() != this->start->getID())
		return false;
	
	return true;

}

GridNode* Astar::getPath(int which)//
{
	if(which < path.size())
		return path[which];
	else
		return NULL;
}