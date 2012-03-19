#include "Ninja.h"

	/*
	void setupAnimations();
	void setBaseAnimation(AnimID id, bool reset = false);
	void setTopAnimation(AnimID id, bool reset = false);
	void fadeAnimations(Ogre::Real deltaTime);
	void updateAnimations(Ogre::Real deltaTime);
	*/


Ninja::Ninja(bool isNinja, std::string name, std::string filename, float height, float angleOffset)	:
Agent(name, filename, 0, angleOffset)
{
	using namespace Ogre;
	this->mBodyNode->scale(Ogre::Vector3(.06f, .06f, .06f));
	if (sceneMgr == NULL)
		return;
	
	this->isNinja = isNinja;

	//this->height = 0;
	this->angleOffset = 270;
	setupAnimations();

	mWalkSpeed = 25.0f;
	mDirection = Ogre::Vector3::ZERO;	

}

void Ninja::setupAnimations()
{
	this->mTimer = 0;
	this->mVerticalVelocity = 0;

	// this is very important due to the nature of the exported animations
	mBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] =
		{"Attack1", "Attack2", "Attack3", "Backflip", "Block", "Climb", "Crouch",
		"Death1", "Death2", "HighJump", "Idle1", "Idle2", "Idle3", "Jump", "JumpNoHeight", "Kick", "SideKick", "Spin", "Stealth", "Walk"};

	// populate our animation list
	for (int i = 0; i < NINJA_NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEntity->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setBaseAnimation(ANIM_IDLE_1);
	//setTopAnimation(ANIM_IDLE_1);

	// relax the hands since we're not holding anything
	//mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
}

void Ninja::setBaseAnimation(AnimID id, bool reset)
{

	if (mBaseAnimID >= 0 && mBaseAnimID < NINJA_NUM_ANIMS)
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


/*
void Ninja::setTopAnimation(AnimID id, bool reset)
{
	if (Ninja::mTopAnimID >= 0 && Ninja::mTopAnimID < NINJA_NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mTopAnimID] = false;
		mFadingOut[mTopAnimID] = true;
	}

	Ninja::mTopAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}*/

void Ninja::fadeAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	for (int i = 0; i < NINJA_NUM_ANIMS; i++)
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

void Ninja::updateAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	Real baseAnimSpeed = 1;
	//Real topAnimSpeed = 1;

	mTimer += deltaTime;

	if (mTimer >= mAnims[mBaseAnimID]->getLength())
	{
			if (mBaseAnimID == ANIM_WALK)
			{
				//setTopAnimation(ANIM_WALK);
				setBaseAnimation(ANIM_WALK);
				mTimer = 0;
			}

			// animation is finished, so return to what we were doing before
			else if (mBaseAnimID != ANIM_IDLE_1) 
			{
				//initially commented line below
				//setTopAnimation(ANIM_IDLE_TOP);
				this->setBaseAnimation(ANIM_IDLE_1, true);
				//this->setTopAnimation(ANIM_IDLE_1, true);
				mTimer = 0;
				//initially commented line bel
				//mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			} 
			//else
			//{
			//	this->setBaseAnimation(this->ANIM_IDLE_1);

			//	//this->setTopAnimation(this->ANIM_IDLE_1);
			//}
		}
	
	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);
	//if (mTopAnimID != ANIM_NONE) mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);

	// apply smooth transitioning between our animations
	fadeAnimations(deltaTime);
}

void Ninja::updateLocomote(Ogre::Real deltaTime)
{
	int xOffset = ((this->grid->getRows() * this->grid->getHeight()) - this->grid->getHeight())/2;
	int yOffset = ((this->grid->getCols() * this->grid->getHeight()) - this->grid->getHeight())/2;
	int x = (xOffset + mDestination.x)/grid->getHeight();
	int y = (yOffset + mDestination.z)/grid->getHeight();

	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (nextLocation()) // a check to see if there is another destination
		{
			// Set walking animation
			this->setBaseAnimation(ANIM_WALK, true);
			//this->setTopAnimation(ANIM_WALK, true);
		}
	}
	else
	{
		Ogre::Real move = mWalkSpeed * deltaTime;
		mDistance -= move;

		if (mDistance <= 0.0f) // over shooting target
		{
			this->mBodyNode->setPosition(mDestination);
			
			this->setCurrentNode(grid->getNode(x, y));

			while(this->getCurrentNode()->getItemList()->size() > 0)
			{		
				Ogre::Entity* ent = this->getCurrentNode()->getItemList()->front();
				this->inventory->push_back(ent);
				this->getCurrentNode()->getItemList()->pop();
				ent->detachFromParent();
				
				this->getmBodyEntity()->attachObjectToBone("Joint13", ent);		
				ent->getParentNode()->scale(1.15f,1.15f,1.15f);
				
				ent->setVisible(true);
			}

			mDirection = Ogre::Vector3::ZERO;
			if (!nextLocation()) // a check to see if there is another destination
			 {
				 this->setCurrentNode(grid->getNode(x, y));
				 this->setBaseAnimation(ANIM_IDLE_1, true);
				//this->setTopAnimation(ANIM_IDLE_1, true);
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

//void Ninja::update(Ogre::Real deltaTime)
//{
//	while(this->getCurrentNode()->getItemList()->size() > 0)
//	{		
//		Ogre::Entity* ent = this->getCurrentNode()->getItemList()->front();
//		this->inventory->push_back(ent);
//		this->getCurrentNode()->getItemList()->pop();
//		ent->detachFromParent();
//		
//		this->getmBodyEntity()->attachObjectToBone("Joint13", ent);		
//		ent->getParentNode()->scale(1.15f,1.15f,1.15f);
//		
//		ent->setVisible(true);
//	}
//
//
//	this->updateLocomote(deltaTime);	
//	this->updateAnimations(deltaTime);	
//}