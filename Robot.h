#pragma once
#include "Agent.h"

#define ROBOT_NUM_ANIMS 5

/*
Attack1 Attack2 Attack3 Backflip Block Climb Crouch Death1 Death2 HighJump Idle1 Idle2 Idle3 
Jump JumpNoHeight Kick SideKick Spin Stealth Walk
*/

class Robot : public Agent
{
private:
	enum AnimID
	{
		ANIM_DIE,
		ANIM_IDLE,
		ANIM_SHOOT,
		ANIM_SLUMP,
		ANIM_WALK,
		ANIM_NONE	
	};
	bool isRobot;
	//####################################################################

	//Ogre::SceneNode* mBodyNode; 
	//Ogre::Entity* mBodyEntity; 
	//float height;						// Characters are different heights
	//float angleOffset;					// Some times they are created facing different directions

	Ogre::AnimationState* mAnims[ROBOT_NUM_ANIMS];    // master animation list
	AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
	AnimID mTopAnimID;                    // current top (upper-body) animation
	bool mFadingIn[ROBOT_NUM_ANIMS];            // which animations are fading in
	bool mFadingOut[ROBOT_NUM_ANIMS];           // which animations are fading out
	virtual void updateLocomote(Ogre::Real deltaTime);			// update the character's walking

	virtual void setupAnimations();
	virtual void setBaseAnimation(AnimID id, bool reset = false);
	//virtual void setTopAnimation(AnimID id, bool reset = false);
	virtual void fadeAnimations(Ogre::Real deltaTime);
	virtual void updateAnimations(Ogre::Real deltaTime);




	//std::list<GridNode*> path;		// store path to follow
	//GridNode* currentNode;			// where is this agent?
	//Grid* grid;						// pointer to the global grid


//#############################################################################################


//protected:
//	void setupAnimations();
	//void setBaseAnimation(AnimID id, bool reset = false);
	//void setTopAnimation(AnimID id, bool reset = false);
	//void fadeAnimations(Ogre::Real deltaTime);
	//void updateAnimations(Ogre::Real deltaTime);

public:
	Robot(bool isRobot, std::string name, 
				std::string filename, float height = 10, float angleOffset = 90);
	//virtual void update(Ogre::Real deltaTime);


	

};