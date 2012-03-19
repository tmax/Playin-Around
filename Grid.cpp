	//void setID(int id);		// set the node id
	//int getID(){return nodeID;};			// get the node ID
	//void setX(int x);		// set the x coordinate
	//void setY(int y);		// set the y coordinate
	//int getX();				// get the x and y coordinate of the node
	//int getY();
	//Ogre::Vector3 getPosition();	// return the position of this node
	//void setClear();		// set the node as walkable
	//void setOccupied();		// set the node as occupied
	//bool isClear();			// is the node walkable

#include "Grid.h"
/* 
	
	#################################GRID NODE METHODS ##################################################

*/
class Agent;
class Grid;
std::stringstream xx;

GridNode::GridNode(int nID, int x, int y, bool isC)
{
	this->nodeID = nID;
	this->xCoord = x;
	this->yCoord = y;
	this->clear = isC;
	//this->itemList = new std::vector<Ogre::Entity*>();
	this->itemList = new std::queue<Ogre::Entity*>();
}

GridNode::GridNode()
{
	this->nodeID = 0;
	this->xCoord = 0;
	this->yCoord = 0;
	this->clear = true;	
	this->itemList = new std::queue<Ogre::Entity*>();
}

void GridNode::addE( Ogre::Entity* e ) {
	itemList->push( e );
}

GridNode::~GridNode(){}



void GridNode::setID(int id)
{
	this->nodeID = id;
}



void GridNode::setX(int x)//set the x GRID-COORD of this node...NOT to be confused with the pixel coords.  it is the GRID-LOCATION of the node
{
	this->xCoord = x;
}

void GridNode::setY(int y)//remember GRID-RELATIVE Y COORDINATE
{
	this->yCoord = y;
}

int GridNode::getX()//return the X GRID-COORD of the node....REMEMBER:::::: it is where the node is relative to the grid coordinate system
{
	return this->xCoord;
}

int GridNode::getY()//GRID-RELATIVE Y COORDINATE
{
	return this->yCoord;
}

Ogre::Vector3 GridNode::getPosition()
{
	/*
	float x = this->xCoord;
	float y = this->yCoord;

	x = -45 + x*10;//the real pixel coordinates of the xcoord and ycoords
	y = -45 + y*10;
	
	
	
	Ogre::Vector3 *vect = new Ogre::Vector3(x, 5, y);//make a new vector with these values

	return *vect;	
	*/
	
	//...ok, so this seems stupid at the time being, but I'm just going to return a Vector3 (remember <X, Y , Z>) with the X component
	// set to teh X-GRIDNODE-COMPONENT and teh Z component set to the Y-GRIDNODE-COMPONENT.....don't ask me why, but I AM DOING IT!!!!
	//Oh, yeah.  It's because I DON'T KNOW HOW WIDE/HIGH A CELL IS SUPPOSED TO BE!!!! NOW I REMEMBER! MAYBE WE'LL FILL THAT IN LATER
	// but then I say to myself, why don't I just use that stupid retarded getX() and getY() funtion from this and just do that when
	//I really do need getPosition()??? I don't know. ..silly me.

	float xGridNodeCoord = this->xCoord; //x coord of this node how it lies on the grid; (x, y) remember
	float yGridNodeCoord = this->yCoord; //same but for the y of the grid;  remember this will be placed into the Z argument of the vector3

	
	
	int x, y, z;		
	
	//now, we are referencing parent of the gridnode, but when did we say which grid is indeed the parent of this gridnode????

	x = (this->parent->getHeight() * xGridNodeCoord);
	y = 0;
	z = (this->parent->getHeight() * yGridNodeCoord);
	
	
	float xoffset = ((this->parent->getRows() * this->parent->getHeight()) - (this->parent->getHeight()))/2;
	float zoffset = ((this->parent->getCols() * this->parent->getHeight()) - (this->parent->getHeight()))/2;



	//Ogre::Vector3 stupidvector = Ogre::Vector3(x-90, y, z-90);//i put a 0 for y cuz i can't know how high the actor is
	
	Ogre::Vector3 stupidvector = Ogre::Vector3(x - xoffset, y, z - zoffset);//i put a 0 for y cuz i can't know how high the actor is

	return stupidvector;




	}
bool GridNode::isClear()
{
	return this->clear;
}

void GridNode::setClear()
{
	this->clear = true;
	this->contains = '.';
}

void GridNode::setOccupied()
{
	this->clear = false;
	this->contains = 'B';
}



/*
				#################################GRID METHODS#################################
*/


Grid::Grid(int numRows, int numCols, int nodeHeight, int nodeDiagonal)//the Grid constructor. what fun!
{
	this->rows = numRows;//set the number of rows, columns, the height of A CELL IN THE GRID, the diagonal, and what's that last thing???
	this->cols = numCols;
	this->height = nodeHeight;
	this->diag = nodeDiagonal;	//what is that below??? hmmm i dunooo
	this->data.resize(numRows, GridRow(numCols));//let's remember now; 'data' is a vector of gridrows ('data' is a GREAT name btw)
												//we are 'resize'-ing it to be of size numRows (originally it was never initialized so yeah)
												//we are then constructing a GridRow with numCols # of GridNodes and putting it in data
												//but remember, all GridRow(numCols) does is resize the 'data' vector of the GridRow
												//it does not put any nodes in it; we do that below 


		/*  maybe it'd be a good idea to set IDs like so...???

			 0_ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9
			0|00
			1|01 11  21  31  41 . . .
			2|02 12  22
			3|03 13  23  33
			4|.             .
			5|.               .
			6|.                 .
			7|
			8|
			9|

		*/

	std::ofstream file;
	file.open("GridVisualization.txt");
	for(int i = 0; i<numRows;/*data.size();*/ i++)//go through each GridRow in data, so i is the ith row in the grid
	{
		GridRow* g = &(data.at(i));//get GridRow # i

		for(int j = 0; j <numCols; /*g->data.size();*/ j++)//go through the Nodes of each GridRow  so now we have cell (i, j), but think of it as (j, i)
											   // since we are moving in the X direction then the Z direction and we are on Row i and column j
		{			
			file << i << " , " << j << "	"; /* CHANGE TO VISUAL POSITIONS */
			//g->data.at(j).setID(i*j + j);//set the ID of each Node
			//implementing ids like above diagram...
			GridNode gNode( (i*(this->cols) + j), i, j);
			//g->data.at(j).setID(i*(this->cols) + j);//now each id will represent the grid coordinates kind of, like 00, 10, 20, = (0,0),(1,0),(2,0)....
			//g->data.at(j).setX(j);//set the X Grid-coordinate of each node
			//g->data.at(j).setY(i);//and the y Grid-coordinate
			//g->data.at(j).setClear();//make it clear initially
			//g->data.at(j).contains = '.';//for debugging purposes put this dot here so we can print it in that one method..printToFile()...
			gNode.contains = '.';
			gNode.parent = this;
			//g->data.at(j).parent = this;//is this a good place to say that the node's parent grid is this...or is it &this?
			g->data[j] = gNode;
			
		}
		file << std::endl;
	}
	file.close();
		
}

GridNode* Grid::getNode(int r, int c)
{
	//return NULL;
	GridNode* gr = &(data[r].data[c]);//seems backwards, but I believe this retreives the node from the rth row and the cth column
									  //since a Grid is a vector of GridRows and a GridRow is a vector of GridNodes, to get to the
									 //(r, c) entry of this 'array' (by this we mean from teh top left corner of the grid, 
									 // go r cells down and c spots over) we would need to go into the cth grid row in Grid's data vector
									 //and then go to teh rth entry in that GridRow...maybe I'm wrong...who knows..

	return gr;
}

int Grid::getHeight()
{
	
	return this->height;
}

int Grid::getDiagonal()
{
	return this->diag;
}

void Grid::setHeight(int h)
{
	this->height = h;
}

void Grid::setDiagonal(int d)
{
	this->diag = d;
}



GridNode* Grid::getNorthNode(GridNode* n)
{	
	int x = n->getX();		//get the coords of the node north of n
	int y = n->getY() - 1;  //moving 'north' is moving in the negative 'y' direction on our 'grid'

	

	if(y >= 0)//0 should be the north most y-position.  make sure it is still within the grid
	{
		GridNode* g = n->parent->getNode(x, y);
		return g;
	}
	else
	{
		GridNode* old = NULL; 
		return old;
	}
}


GridNode* Grid::getSouthNode(GridNode* n)
{
	int x = n->getX();
	int y = n->getY() + 1;

	if(y < this->rows)
	{
		GridNode* g = n->parent->getNode(x, y);
		return g;
	}
	else
	{
		GridNode* old = NULL; 
		return old;
	}
}

GridNode* Grid::getEastNode(GridNode* n)
{
	int x = n->getX() + 1;
	int y = n->getY();

	if(x< this->rows)
	{
		GridNode* g = n->parent->getNode(x, y);
		return g;
	}
	else
	{
		GridNode* old = NULL; 
		return old;
	}

}

GridNode* Grid::getWestNode(GridNode* n)
{
	int x = n->getX() - 1;
	int y = n->getY();
	if(x >= 0)
	{
		GridNode* g = n->parent->getNode(x, y);
		return g;
	}
	else
	{
		GridNode* old = NULL; 
		return NULL;
	}
}

GridNode* Grid::getNENode(GridNode* n)
{
	int x = n->getX() + 1;
	int y = n->getY() - 1;



	if(x < this->rows && y >= 0)
	{

		GridNode* g = n->parent->getNode(x, y);
		return g;
	}
	else
	{
		GridNode* old = NULL;
		return old;
	}	
}

GridNode* Grid::getNWNode(GridNode* n)
{
	int x = n->getX() - 1;
	int y = n->getY() - 1;



	if(x >= 0 && y>=0)
	{
		GridNode* g = n->parent->getNode(x, y);
		return g;
	}
	else
	{
		GridNode* old = NULL;
		return old;
	}
}

GridNode* Grid::getSENode(GridNode* n)
{
	int x = n->getX() + 1;
	int y = n->getY() + 1;



	if(x< this->rows && y<this->cols)
	{

		GridNode* g = n->parent->getNode(x, y);
		return g;
	}
	else
	{
		GridNode* old = NULL;
		return old;
	}	
}

GridNode* Grid::getSWNode(GridNode* n)
{
	int x = n->getX() - 1;
	int y = n->getY() + 1;



	if(x >= 0 && y < this->rows)
	{
		GridNode* g = n->parent->getNode(x, y);
		return g;
	}
	else
	{
		GridNode* old = NULL;
		return old;
	}	
}


int Grid::getManhattan(GridNode* node1, GridNode* node2)//return the Manhattan distance between two nodes on the grid
{
	int manhattan = 0;

	int xdistance = node1->getX() - node2->getX();
	int ydistance = node1->getY() - node2->getY();

	xdistance = abs(xdistance*this->getHeight());
	ydistance = abs(ydistance*this->getHeight());

	manhattan = xdistance+ydistance;

	return manhattan;
	
}


int Grid::getGcost(GridNode* node1, GridNode* node2)
{
	//we assume that node1 and node2 are adjacent
	//we need to know if these node2 is either N,E,S, or W of node1, in which case we'd return height
	//or if node2 is NE,SE,SW,or NW of node1 in which case we'd return diagonal

	Grid* grid = node1->parent;

	if(
		(grid->getNorthNode(node1) == node2) ||
		(grid->getEastNode(node1) == node2) ||
		(grid->getSouthNode(node1) == node2) ||
		(grid->getWestNode(node1) == node2))
		return grid->getHeight();
	else if(
		(grid->getNENode(node1) == node2) ||
		(grid->getSENode(node1) == node2) ||
		(grid->getSWNode(node1) == node2) ||
		(grid->getNWNode(node1) == node2))
		return grid->getDiagonal();
	else 
		return NULL;
			
}

void Grid::printToFile() 
{
	xx.str("");
//	xx << getGcost( getNode(0,0), getNode(9,1) );
	std::ofstream outFile; 
	outFile.open("Grid.txt"); 
	outFile << this->getNode(0, 0)->getID();
	for (int i = 0; i < rows; i++) 
	{ 
		for (int j = 0; j < cols; j++) 
		{ 
			outFile << this->getNode(i, j)->contains << " "; 
		} 
		outFile << std::endl; 
	} 
	outFile<<std::endl;
	outFile << "okay, just a test" << std::endl;
	outFile.close(); 
} 