#include "Fish.h"



Fish::Fish(bool isFish, std::string name, std::string filename, float height, float angleOffset)	:
Agent(name, filename, 0, angleOffset)
{
	using namespace Ogre;
	this->mBodyNode->scale(Ogre::Vector3(1.250f, 1.250f, 1.250f));
	if (sceneMgr == NULL)
		return;
	
	this->isFish = isFish;

	this->height = 3;
	this->angleOffset = 180;
	setupAnimations();

	mWalkSpeed = 25.0f;
	mDirection = Ogre::Vector3::ZERO;	

}

void Fish::setupAnimations()
{
	this->mTimer = 0;
	this->mVerticalVelocity = 0;

	// this is very important due to the nature of the exported animations
	mBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

/*
		ANIM_SWIM,
		ANIM_NONE	
*/


	Ogre::String animNames[] =
		{"swim"};

	// populate our animation list
	for (int i = 0; i < FISH_NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEntity->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setBaseAnimation(ANIM_SWIM);
}

void Fish::setBaseAnimation(AnimID id, bool reset)
{

	if (mBaseAnimID >= 0 && mBaseAnimID < FISH_NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mBaseAnimID] = false;
		mFadingOut[mBaseAnimID] = true;
	}

	mBaseAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}

void Fish::fadeAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	for (int i = 0; i < FISH_NUM_ANIMS; i++)
	{
		if (mFadingIn[i])
		{
			// slowly fade this animation in until it has full weight
			Real newWeight = mAnims[i]->getWeight() + deltaTime * 7.5f; //ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight >= 1) mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			// slowly fade this animation out until it has no weight, and then disable it
			Real newWeight = mAnims[i]->getWeight() - deltaTime * 7.5f; //ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(true);
				mFadingOut[i] = true;
			}
		}
	}
}

void Fish::updateAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	Real baseAnimSpeed = 1;
	//Real topAnimSpeed = 1;

	mTimer += deltaTime;

	if (mTimer >= mAnims[mBaseAnimID]->getLength())
	{
			// animation is finished, so return to what we were doing before
			if (mBaseAnimID == ANIM_SWIM) 
			{
				//initially commented line below
				this->setBaseAnimation(ANIM_SWIM, true);

				mTimer = 0;
			} 
			else if (mBaseAnimID == ANIM_SWIM)
			{
				setBaseAnimation(ANIM_SWIM);
				mTimer = 0;
			}
			else
			{
				this->setBaseAnimation(this->ANIM_SWIM);
			}
		}
	
	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);

	// apply smooth transitioning between our animations
	fadeAnimations(deltaTime);
}

void Fish::updateLocomote(Ogre::Real deltaTime)
{
	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (nextLocation()) // a check to see if there is another destination
		{
			// Set walking animation
			this->setBaseAnimation(ANIM_SWIM, true);
		}
	}
	else
	{
		Ogre::Real move = mWalkSpeed * deltaTime;
		mDistance -= move;

		if (mDistance <= 0.0f) // over shooting target
		{
			this->mBodyNode->setPosition(mDestination);

			mDirection = Ogre::Vector3::ZERO;
			if (!nextLocation()) // a check to see if there is another destination
			 {
				 this->setBaseAnimation(ANIM_SWIM, true);
			 }
			 else
			 {

				 Ogre::Vector3 src = this->mBodyNode->getOrientation() * Ogre::Vector3::UNIT_X;
				 if ((1.0f + src.dotProduct(mDirection)) < 0.0001f)
				 {
					 this->mBodyNode->yaw(Ogre::Degree(180));
				 }
				 else
				 {
					Ogre::Quaternion quat = src.getRotationTo(mDirection);
					this->mBodyNode->rotate(quat);
					this->mBodyNode->yaw(Ogre::Degree(angleOffset));  // To correct for models facing different directions
				}
			 }
		}
		else
		{
			this->mBodyNode->translate(mDirection * move);			
		}
	}
}


