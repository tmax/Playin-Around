#ifndef __GameApplication_h_
#define __GameApplication_h_

#include "BaseApplication.h"
#include <list>
//#include "Agent.h"
//#include "Sinbad.h"



class Agent;
class Sinbad;
class Ninja;
class Grid;
class GridNode;

class GameApplication : public BaseApplication
{
	
private:

	Grid* grid;
	std::list<Agent*> agentList;
	void loadEnv();			// Load the buildings or ground plane, etc
	void setupEnv();		// Set up lights, shadows, etc	
	void loadObjects();		// Load other propers or objects (e.g. furniture)
	void loadCharacters();	// Load actors, agents or characters
	
	Agent* player;

	void addTime(Ogre::Real deltaTime);
public:
	void setPlayerPtr(Agent* player){this->player = player;};
	Agent* getPlayerPtr(){return this->player;}
    GameApplication(void);
    virtual ~GameApplication(void);
	void moveLoop(Agent* a, Grid* g);
	Ogre::Vector3 gCoordToVector(Grid* grid, GridNode* gNode, bool setOccupied = false);
	Ogre::Vector3 gCoordToVector2(Grid* grid, GridNode* gNode);
	//////////////////////////////////////////////////////////////////////////
	// Lecture 7: moved from base application
	// OIS::KeyListener
	bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//////////////////////////////////////////////////////////////////////////////////
	// Lecture 8
	bool bLMouseDown, bRMouseDown;	//true if mouse buttons are held down
	Ogre::SceneNode *mCurrentObject;	//pointer to our currently selected object
	//int mCount;						//number of objects created
	/////////////////////////////////////////////////////////////////////////////////


protected:
    virtual void createScene(void);
};

#endif // #ifndef __GameApplication_h_