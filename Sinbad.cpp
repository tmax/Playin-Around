//#include "Agent.h"
#include "Sinbad.h"
#include "windows.h"
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")



Sinbad::Sinbad(bool isOgre, std::string name, std::string filename, float height, float angleOffset) : 
Agent(name, filename, height, angleOffset)
{
	using namespace Ogre;

	if (sceneMgr == NULL)
		return;
	
	this->isOgre = isOgre;

	this->height = height;
	this->angleOffset = angleOffset;
	setupAnimations();

	mWalkSpeed = 15.0f;
	mDirection = Ogre::Vector3::ZERO;	

}

void Sinbad::setupAnimations()
{
	this->mTimer = 0;
	this->mVerticalVelocity = 0;

	// this is very important due to the nature of the exported animations
	mBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] =
		{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
		"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};

	// populate our animation list
	for (int i = 0; i < SINBAD_NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEntity->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setBaseAnimation(ANIM_IDLE_BASE);
	setTopAnimation(ANIM_IDLE_TOP);

	// relax the hands since we're not holding anything
	//mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
}

void Sinbad::setBaseAnimation(AnimID id, bool reset)
{

	if (mBaseAnimID >= 0 && mBaseAnimID < SINBAD_NUM_ANIMS)
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

void Sinbad::setTopAnimation(AnimID id, bool reset)
{

	if (mTopAnimID >= 0 && mTopAnimID < SINBAD_NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mTopAnimID] = false;
		mFadingOut[mTopAnimID] = true;
	}

	mTopAnimID = id;

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

void Sinbad::fadeAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	for (int i = 0; i < SINBAD_NUM_ANIMS; i++)
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
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

void Sinbad::updateAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	Real baseAnimSpeed = 1;
	Real topAnimSpeed = 1;

	mTimer += deltaTime;

	if (mTimer >= mAnims[mTopAnimID]->getLength())
	{
			 if (mTopAnimID == ANIM_RUN_TOP)
			{
				setTopAnimation(ANIM_RUN_TOP, 1);
				setBaseAnimation(ANIM_RUN_BASE, 1);
				mTimer = 0;
			}
			// animation is finished, so return to what we were doing before
			else if (mTopAnimID != ANIM_IDLE_TOP) 
			{
				//initially commented line below
				//setTopAnimation(ANIM_IDLE_TOP);
				//should teh below be run???
				this->setBaseAnimation(ANIM_IDLE_BASE, true);
				this->setTopAnimation(ANIM_IDLE_TOP, true);
				mTimer = 0;
				//initially commented line bel
				//mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			}
			//FINITE STATE MACHINE == SAME AS RULE-BASED-SYSTEM
			//else
			//{
			//	this->setBaseAnimation(this->ANIM_IDLE_BASE);
			//	this->setTopAnimation(this->ANIM_IDLE_TOP);
			//}
	}
	
	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);
	if (mTopAnimID != ANIM_NONE) mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);

	// apply smooth transitioning between our animations
	fadeAnimations(deltaTime);
}

void Sinbad::updateLocomote(Ogre::Real deltaTime)
{
	int xOffset = ((this->grid->getCols() * this->grid->getHeight())/2 - this->grid->getHeight()/2);
	int yOffset = ((this->grid->getRows() * this->grid->getHeight())/2 - this->grid->getHeight()/2);
	int c = (xOffset + mDestination.x)/grid->getHeight();
	int r = (yOffset + mDestination.z)/grid->getHeight();
	//int y = ((mDestination.z - 5) * this->grid->getHeight()) + 1;

	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (nextLocation()) // a check to see if there is another destination
		{
			// Set walking animation
			this->setBaseAnimation(ANIM_RUN_BASE, true);
			this->setTopAnimation(ANIM_RUN_TOP, true);
		}
	}
	else
	{
		Ogre::Real move = mWalkSpeed * deltaTime;
		mDistance -= move;

		if (mDistance <= 0.0f) // over shooting target
		{
			this->setCurrentNode(this->grid->getNode(c, r));

			while(this->getCurrentNode()->getItemList()->size() > 0)
			{		
				Ogre::Entity* ent = this->getCurrentNode()->getItemList()->front();
				this->inventory->push_back(ent);
				this->getCurrentNode()->getItemList()->pop();
				ent->detachFromParent();
				
				PlaySound(TEXT("C:\\Documents and Settings\\Tarek\\My Documents\\CS_425\\hw6\\bin\\Debug\\clip.wav"), NULL, SND_FILENAME|SND_ASYNC);
				//PlaySound(0,0,0);
				//Beep(450,100);
				
				

				//this->getmBodyEntity()->detachAllObjectsFromBone();
				
				this->getmBodyEntity()->attachObjectToBone("Handle.R", ent);
				
				ent->getParentNode()->scale(.1f,.1f,.1f);
				ent->getParentNode()->yaw(Ogre::Degree(-90));
				ent->getParentNode()->pitch(Ogre::Degree(180));
				ent->getParentNode()->translate(0, -1.0, 0);		
				ent->setVisible(true);
			}


			this->mBodyNode->setPosition(mDestination);

			mDirection = Ogre::Vector3::ZERO;
			if (!nextLocation()) // a check to see if there is another destination
			 {
	 			this->setCurrentNode(this->grid->getNode(r, c));
				this->setBaseAnimation(ANIM_IDLE_BASE, true);
				this->setTopAnimation(ANIM_IDLE_TOP, true);
				this->mBodyNode->yaw(Ogre::Degree(180));

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

//void Sinbad::update(Ogre::Real deltaTime)
//{
//
//
//	//check to see if the current gridNode we are on has an itemList that is not empty
//	//	if itemlist is NOT empty
//	//		grab ALL of the items in the itemList of the node and store them in sinbad's inventory
//	//
//
//	//int randx = rand()%this->grid->getRows();
//	//int randy = rand()%this->grid->getCols();
//
//	//GridNode* gNode = grid->getNode(randx, randy);
//
//	//if(gNode->isClear())
//	//{
//	//	this->moveTo(gNode);		
//	//	//this->setCurrentNode(gNode);
//	//}		
//
//
//
//	while(this->getCurrentNode()->getItemList()->size() > 0)
//	{		
//		Ogre::Entity* ent = this->getCurrentNode()->getItemList()->front();
//		this->inventory->push_back(ent);
//		this->getCurrentNode()->getItemList()->pop();
//		ent->detachFromParent();
//		
//		//this->getmBodyEntity()->detachAllObjectsFromBone();
//		
//		this->getmBodyEntity()->attachObjectToBone("Handle.R", ent);		
//		ent->getParentNode()->scale(.1f,.1f,.1f);
//		ent->getParentNode()->yaw(Ogre::Degree(-90));
//		ent->getParentNode()->pitch(Ogre::Degree(180));
//		ent->getParentNode()->translate(0, -1.0, 0);		
//		ent->setVisible(true);
//	}
//
//
//	this->updateLocomote(deltaTime);	
//	this->updateAnimations(deltaTime);	
//}