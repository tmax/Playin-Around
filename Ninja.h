#pragma once
#include "Agent.h"

#define NINJA_NUM_ANIMS 20

/*
Attack1 Attack2 Attack3 Backflip Block Climb Crouch Death1 Death2 HighJump Idle1 Idle2 Idle3 
Jump JumpNoHeight Kick SideKick Spin Stealth Walk
*/

class Ninja : public Agent
{
private:
	enum AnimID
	{
		ANIM_ATTACK_1,
		ANIM_ATTACK_2,
		ANIM_ATTACK_3,
		ANIM_BACKFLIP,
		ANIM_BLOCK,
		ANIM_CLIMB,
		ANIM_CROUCH,
		ANIM_DEATH_1,
		ANIM_DEATH_2,
		ANIM_HIGH_JUMP,
		ANIM_IDLE_1,
		ANIM_IDLE_2,
		ANIM_IDLE_3,
		ANIM_JUMP,
		ANIM_JUMP_NO_HEIGHTS,
		ANIM_KICK,
		ANIM_SIDE_KICK,
		ANIM_SPIN,
		ANIM_STEALTH,
		ANIM_WALK,
		ANIM_NONE	
	};
	bool isNinja;
	//####################################################################

	//Ogre::SceneNode* mBodyNode; 
	//Ogre::Entity* mBodyEntity; 
	//float height;						// Characters are different heights
	//float angleOffset;					// Some times they are created facing different directions

	Ogre::AnimationState* mAnims[NINJA_NUM_ANIMS];    // master animation list
	AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
	AnimID mTopAnimID;                    // current top (upper-body) animation
	bool mFadingIn[NINJA_NUM_ANIMS];            // which animations are fading in
	bool mFadingOut[NINJA_NUM_ANIMS];           // which animations are fading out
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
	Ninja(bool isNinja, std::string name, 
				std::string filename, float height = 10, float angleOffset = 90);

	//virtual void update(Ogre::Real deltaTime);

	

};