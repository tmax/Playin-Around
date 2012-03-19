#pragma once
#define SINBAD_NUM_ANIMS 13
#include "Agent.h"
//#include "GameApplication.h"


class Sinbad : public Agent
{
private:
	enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,
		ANIM_RUN_BASE,
		ANIM_RUN_TOP,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_DRAW_SWORDS,
		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,
		ANIM_DANCE,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_NONE
	};
	bool isOgre;

	float height;						// Characters are different heights
	float angleOffset;					// Some times they are created facing different directions

	Ogre::AnimationState* mAnims[SINBAD_NUM_ANIMS];    // master animation list
	AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
	AnimID mTopAnimID;                    // current top (upper-body) animation
	bool mFadingIn[SINBAD_NUM_ANIMS];            // which animations are fading in
	bool mFadingOut[SINBAD_NUM_ANIMS];           // which animations are fading out
	virtual void updateLocomote(Ogre::Real deltaTime);			// update the character's walking

	virtual void setupAnimations();
	virtual void setBaseAnimation(AnimID id, bool reset = false);
	virtual void setTopAnimation(AnimID id, bool reset = false);
	virtual void fadeAnimations(Ogre::Real deltaTime);
	virtual void updateAnimations(Ogre::Real deltaTime);


public:
	Sinbad(bool isOgre, std::string name, std::string filename, float height = 5, float angleOffset = 90);
	//virtual void update(Ogre::Real deltaTime);
};