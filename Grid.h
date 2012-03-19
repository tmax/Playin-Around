////////////////////////////////////////////////////////
// Class to hold the grid layout of our environment
// Used for navigation and AI, not graphics

#ifndef GRID_H
#define GRID_H
#include <iostream>
#include <vector>
#include <assert.h>

#include "GameApplication.h"
class Grid;
class GridNode {
//class Grid;
private:
	
	
	int nodeID;			// identity for the node
	int xCoord;			// x coordinate.......GRID COORDINATE GRID COORDINATE GRID COORDINATE NOT PIXELS NOT PIXELS NOT PIXELS
	int yCoord;			// y coordinate
	bool clear;			// is the node walkable?
	Grid* parent2;
	std::queue<Ogre::Entity*>* itemList;	//list of items we will 
	
public:

	void addE(Ogre::Entity* e);
	std::queue<Ogre::Entity*>* getItemList() { return itemList; };
	Grid* parent; //the Grid where this GridNode resides; need this for vital info like height/width of cell
	char contains;		// For printing... B = blocked, S = start, G = goal, numbers = path
	GridNode();			// default constructor
	GridNode(int nID, int x, int y, bool isC = true);	// create a node
	~GridNode();		// destroy a node

	Grid* getParent(){return parent2;};
	void setParent(Grid* g){parent2 = g;};
	void setID(int id);		// set the node id
	int getID(){return nodeID;};			// get the node ID
	void setX(int x);		// set the x coordinate
	void setY(int y);		// set the y coordinate
	int getX();				// get the x and y coordinate of the node
	int getY();
	Ogre::Vector3 getPosition();	// return the position of this node....BUT WHY RETURN A VECTOR3????? DOESN'T MAKE SENSE
					// should I send back a vector with the  X,Y GRID coords? where x maps to x, and y maps to z in the vector3
									// and y is left....um....nothing cuz I don't/can't access the HEIGHT of the actor
	void setClear();		// set the node as walkable
	void setOccupied();		// set the node as occupied
	bool isClear();			// is the node walkable
	//std::vector<Ogre::Entity*>* itemList;//list of items we will 

};

class GridRow {  // helper class
public:
	std::vector<GridNode> data;// a vector of GridNodes
	GridRow(int size) {data.resize(size);};//resize the array of GridNodes in THIS ROW to be a row that holds SIZE # of GridNodes
	~GridRow(){};
};

class Grid {
private:
	std::vector<GridRow> data;  // actually hold the grid data.....A vector of GridRows
	int rows;					// number of rows
	int cols;					// number of columns
	int height;					// height and width of each cell (10 for our A* example)
	int diag;					// length of the diagonal of each cell (14 for our A* example)
public:
	Grid(int numRows, int numCols, int nodeHeight = 10, int nodeDiagonal = 14);	// create a grid
	~Grid(){};					// destroy a grid

	GridNode* getNode(int r, int c);  // get the node specified 
	int getHeight();				  // get the height and width of the nodes	
	int getDiagonal();				  // get the diagonal of the nodes
	void setHeight(int h);			  // set the height of the nodes
	void setDiagonal(int d);		  // set the Diagonal of the nodes
	int getRows(){return rows;};
	int getCols(){return cols;};

	GridNode* getNorthNode(GridNode* n);		  // get adjacent nodes;
	GridNode* getSouthNode(GridNode* n);
	GridNode* getEastNode(GridNode* n);
	GridNode* getWestNode(GridNode* n);
	GridNode* getNENode(GridNode* n);
	GridNode* getNWNode(GridNode* n);
	GridNode* getSENode(GridNode* n);
	GridNode* getSWNode(GridNode* n);

	int getManhattan(GridNode* node1, GridNode* node2);  //get distance between between two nodes
	int getGcost(GridNode* node1, GridNode* node2);  // height or diagonal?

	void printToFile();				// Print a grid to a file.  Good for debugging
};

#endif