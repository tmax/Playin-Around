#include "Robot.h"



Robot::Robot(bool isRobot, std::string name, std::string filename, float height, float angleOffset)	:
Agent(name, filename, 0, angleOffset)
{
	using namespace Ogre;
	this->mBodyNode->scale(Ogre::Vector3(.150f, .150f, .150f));
	if (sceneMgr == NULL)
		return;
	
	this->isRobot = isRobot;

	//this->height = 0;
	this->angleOffset = 0;
	setupAnimations();

	mWalkSpeed = 25.0f;
	mDirection = Ogre::Vector3::ZERO;	

}

void Robot::setupAnimations()
{
	this->mTimer = 0;
	this->mVerticalVelocity = 0;

	// this is very important due to the nature of the exported animations
	mBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

/*
Anim Name: Die, Length: 0.541667
Anim Name: Idle, Length: 2.83333
Anim Name: Shoot, Length: 0.541667
Anim Name: Slump, Length: 0.75
Anim Name: Walk, Length: 1.20833

		ANIM_DIE,
		ANIM_IDLE,
		ANIM_SHOOT,
		ANIM_SLUMP,
		ANIM_WALK,
		ANIM_NONE	

*/


	Ogre::String animNames[] =
		{"Die", "Idle", "Shoot", "Slump", "Walk"};

	// populate our animation list
	for (int i = 0; i < ROBOT_NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEntity->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setBaseAnimation(ANIM_IDLE);
}

void Robot::setBaseAnimation(AnimID id, bool reset)
{

	if (mBaseAnimID >= 0 && mBaseAnimID < ROBOT_NUM_ANIMS)
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

void Robot::fadeAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	for (int i = 0; i < ROBOT_NUM_ANIMS; i++)
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

void Robot::updateAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	Real baseAnimSpeed = 1;
	//Real topAnimSpeed = 1;

	mTimer += deltaTime;

	if (mTimer >= mAnims[mBaseAnimID]->getLength())
	{
			if (mBaseAnimID == ANIM_WALK)
			{
				setBaseAnimation(ANIM_WALK);
				mTimer = 0;
			}

			// animation is finished, so return to what we were doing before
			else if (mBaseAnimID != ANIM_IDLE) 
			{
				//initially commented line below
				this->setBaseAnimation(ANIM_IDLE, true);

				mTimer = 0;
			} 
			//else
			//{
			//	this->setBaseAnimation(this->ANIM_IDLE);
			//}
		}
	
	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);

	// apply smooth transitioning between our animations
	fadeAnimations(deltaTime);
}

void Robot::updateLocomote(Ogre::Real deltaTime)
{
	int xOffset = ((this->grid->getCols() * this->grid->getHeight()) - this->grid->getHeight())/2;
	int yOffset = ((this->grid->getRows() * this->grid->getHeight()) - this->grid->getHeight())/2;
	int x = (xOffset + mDestination.x)/grid->getHeight();
	int y = (yOffset + mDestination.z)/grid->getHeight();

	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (nextLocation()) // a check to see if there is another destination
		{
			// Set walking animation
			this->setBaseAnimation(ANIM_WALK, true);
		}
	}
	else
	{
		Ogre::Real move = mWalkSpeed * deltaTime;
		mDistance -= move;

		if (mDistance <= 0.0f) // over shooting target
		{
			this->setCurrentNode(grid->getNode(x,y));
			this->mBodyNode->setPosition(mDestination);


	while(this->getCurrentNode()->getItemList()->size() > 0)
	{		
		Ogre::Entity* ent = this->getCurrentNode()->getItemList()->front();
		this->inventory->push_back(ent);
		this->getCurrentNode()->getItemList()->pop();
		ent->detachFromParent();
		
		this->getmBodyEntity()->attachObjectToBone("Joint15", ent);		
		ent->getParentNode()->scale(.5f,.5f,.5f);
		ent->getParentNode()->yaw(Ogre::Degree(90));
		ent->getParentNode()->pitch(Ogre::Degree(180));
		ent->getParentNode()->translate(0, -1.0, 0);		
		ent->setVisible(true);
		
		ent->setVisible(true);
	}


			mDirection = Ogre::Vector3::ZERO;
			if (!nextLocation()) // a check to see if there is another destination
			 {
				 this->setCurrentNode(grid->getNode(x, y));
				 this->setBaseAnimation(ANIM_IDLE, true);
				 this->mBodyNode->yaw(Ogre::Degree(90));

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
					this->mBodyNode->yaw(Ogre::Degree(0));  // To correct for models facing different directions
				}
			 }
		}
		else
		{
			this->mBodyNode->translate(mDirection * move);			
		}
	}
}

//void Robot::update(Ogre::Real deltaTime)
//{
//	while(this->getCurrentNode()->getItemList()->size() > 0)
//	{		
//		Ogre::Entity* ent = this->getCurrentNode()->getItemList()->front();
//		this->inventory->push_back(ent);
//		this->getCurrentNode()->getItemList()->pop();
//		ent->detachFromParent();
//		
//		this->getmBodyEntity()->attachObjectToBone("Joint15", ent);		
//		ent->getParentNode()->scale(.5f,.5f,.5f);
//		ent->getParentNode()->yaw(Ogre::Degree(90));
//		ent->getParentNode()->pitch(Ogre::Degree(180));
//		ent->getParentNode()->translate(0, -1.0, 0);		
//		ent->setVisible(true);
//		
//		ent->setVisible(true);
//	}
//
//
//	this->updateLocomote(deltaTime);	
//	this->updateAnimations(deltaTime);	
//}