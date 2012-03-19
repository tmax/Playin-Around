#pragma once
#include "Agent.h"

#define FISH_NUM_ANIMS 1

/*
Attack1 Attack2 Attack3 Backflip Block Climb Crouch Death1 Death2 HighJump Idle1 Idle2 Idle3 
Jump JumpNoHeight Kick SideKick Spin Stealth Walk
*/

class Fish : public Agent
{
private:
	enum AnimID
	{
		ANIM_SWIM,
		ANIM_NONE	
	};
	bool isFish;
	//####################################################################

	//Ogre::SceneNode* mBodyNode; 
	//Ogre::Entity* mBodyEntity; 
	//float height;						// Characters are different heights
	//float angleOffset;					// Some times they are created facing different directions

	Ogre::AnimationState* mAnims[FISH_NUM_ANIMS];    // master animation list
	AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
	AnimID mTopAnimID;                    // current top (upper-body) animation
	bool mFadingIn[FISH_NUM_ANIMS];            // which animations are fading in
	bool mFadingOut[FISH_NUM_ANIMS];           // which animations are fading out
	virtual void updateLocomote(Ogre::Real deltaTime);			// update the character's walking

	virtual void setupAnimations();
	virtual void setBaseAnimation(AnimID id, bool reset = false);
	//virtual void setTopAnimation(AnimID id, bool reset = false);
	virtual void fadeAnimations(Ogre::Real deltaTime);
	virtual void updateAnimations(Ogre::Real deltaTime);

public:
	Fish(bool isFish, std::string name, 
				std::string filename, float height = 10, float angleOffset = 90);
	//void update(Ogre::Real deltaTime);


	

};