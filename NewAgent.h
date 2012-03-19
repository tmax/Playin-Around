#ifndef AGENT_H
#define AGENT_H

#include "BaseApplication.h"
#include <deque>
#include "Grid.h"
#include "aStar.h"

#define NUM_ANIMS 0  // number of animations the character has. Should be made character specific

extern Ogre::SceneManager* sceneMgr; // Defined in main.cpp
class GridNode;
class Grid;

class Agent 
{

protected:
	enum AnimID
	{
	};

private:

	// all the animations our character has, and a null ID
	// some of these affect separate body parts and will be blended together

	Ogre::SceneNode* mBodyNode; 
	Ogre::Entity* mBodyEntity; 
	float height;						// Characters are different heights
	float angleOffset;					// Some times they are created facing different directions

	Ogre::AnimationState* mAnims[NUM_ANIMS];    // master animation list
	AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
	AnimID mTopAnimID;                    // current top (upper-body) animation
	bool mFadingIn[NUM_ANIMS];            // which animations are fading in
	bool mFadingOut[NUM_ANIMS];           // which animations are fading out
	Ogre::Real mTimer;					  // general timer to see how long animations have been playing
	Ogre::Real mVerticalVelocity;		  // for jumping

	Ogre::Real mDistance;					// The distance the agent has left to travel
	Ogre::Vector3 mDirection;				// The direction the object is moving
	Ogre::Vector3 mDestination;				// The destination the object is moving towards
	std::deque<Ogre::Vector3> mWalkList;	// The list of points we are walking to
	Ogre::Real mWalkSpeed;					// The speed at which the object is moving
	bool nextLocation();					// Is there another destination?
	void updateLocomote(Ogre::Real deltaTime);			// update the character's walking

	virtual void setupAnimations();
	virtual void setBaseAnimation(AnimID id, bool reset = false);
	virtual void setTopAnimation(AnimID id, bool reset = false);
	virtual void fadeAnimations(Ogre::Real deltaTime);
	virtual void updateAnimations(Ogre::Real deltaTime);

	std::list<GridNode*> path;		// store path to follow
	GridNode* currentNode;			// where is this agent?
	Grid* grid;						// pointer to the global grid
public:
	Agent(std::string name, std::string filename, float height = 5, float angleOffset = 90); // takes a mesh file
	void setPosition(float x, float y, float z); 

	void update(Ogre::Real deltaTime);		// update the agent
	
	GridNode* getCurrentNode(){return currentNode;};		// get the currentNode
	void setCurrentNode(GridNode* g){currentNode = g;};		// set the currentNode
	void setGrid(Grid* g) {grid = g;};						// keep a pointers to the universal grid
	void moveTo(GridNode* node);							// Use A* to move to this node from your current spot

};

#endif