#include "Agent.h"

Agent::Agent(std::string name, std::string filename, float height, float angleOffset) //constructor
{
	using namespace Ogre;

	if (sceneMgr == NULL)
		return;
	
	this->height = height;
	this->angleOffset = angleOffset;
	mBodyNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * height);
	mBodyEntity = sceneMgr->createEntity(name, filename);
	mBodyNode->attachObject(mBodyEntity);

	//setupAnimations();

	mWalkSpeed = 25.0f;
	mDirection = Ogre::Vector3::ZERO;	
	this->inventory = new std::vector<Ogre::Entity*>();
	this->doUpdate = 1;
}

void Agent::setPosition(float x, float y, float z)
{
	this->mBodyNode->setPosition(x, y + height, z);
}

void Agent::setPosition(Ogre::Vector3 destVect)
{
	float x, y, z;
	x = destVect.x;
	y = destVect.y+height;
	z = destVect.z;

	Ogre::Vector3 destination(x,y,z);
	
	this->mBodyNode->setPosition(x, y, z);
}

void Agent::setupAnimations()
{
	this->mTimer = 0;
	this->mVerticalVelocity = 0;

	// this is very important due to the nature of the exported animations
	mBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] =
		{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
		"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};

	// populate our animation list
	for (int i = 0; i < NUM_ANIMS; i++)
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
	mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
}

void Agent::setBaseAnimation(AnimID id, bool reset)
{
	if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
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
	
void Agent::setTopAnimation(AnimID id, bool reset)
{
	if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS)
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

void Agent::updateAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	Real baseAnimSpeed = 1;
	Real topAnimSpeed = 1;

	mTimer += deltaTime;

	if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			// animation is finished, so return to what we were doing before
			if (mBaseAnimID == ANIM_IDLE_BASE) 
			{
				//initially commented line below
				//setTopAnimation(ANIM_IDLE_TOP);
				this->setBaseAnimation(ANIM_IDLE_BASE, true);
				this->setTopAnimation(ANIM_IDLE_TOP, true);
				mTimer = 0;
				//initially commented line bel
				//mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			} else if (mBaseAnimID == ANIM_RUN_BASE)
			{
				setTopAnimation(ANIM_RUN_TOP);
				setBaseAnimation(ANIM_RUN_BASE);
				mTimer = 0;
			}
			else
			{
				this->setBaseAnimation(this->ANIM_IDLE_BASE);
				this->setTopAnimation(this->ANIM_IDLE_TOP);
			}
		}
	
	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);
	if (mTopAnimID != ANIM_NONE) mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);

	// apply smooth transitioning between our animations
	fadeAnimations(deltaTime);
}

void Agent::fadeAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	for (int i = 0; i < NUM_ANIMS; i++)
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

bool Agent::nextLocation()
{
	if (mWalkList.empty())
		return false;

	mDestination = mWalkList.front();	// get the first element
	//make a new gridnode and set this as the currentNode based on the vector we are about to pop
	
	mWalkList.pop_front();				// remove the first element
	
	this->mDirection = this->mDestination - this->mBodyNode->getPosition();

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

	this->mDistance = this->mDirection.normalise();

	// REMOVE THIS:
	//mWalkList.push_back(mDestination); // just keep walking between the points

	return true;
}

void Agent::updateLocomote(Ogre::Real deltaTime)
{
	int x = ((mDestination.x - (this->grid->getHeight() * .5)) / this->grid->getHeight()) + 1;
	int y = ((mDestination.z - (this->grid->getHeight() * .5)) / this->grid->getHeight()) + 1;

	
	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (this->nextLocation()) // a check to see if there is another destination
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

			this->setCurrentNode(grid->getNode(x, y));

			this->mBodyNode->setPosition(mDestination);

			mDirection = Ogre::Vector3::ZERO;
			 if (!this->nextLocation()) // a check to see if there is another destination
			 {
				this->setBaseAnimation(ANIM_IDLE_BASE, true);
				this->setTopAnimation(ANIM_IDLE_TOP, true);
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
void Agent::update(Ogre::Real deltaTime)
{

	if(doUpdate)
	{
		int randx = rand()%this->grid->getCols();
		int randy = rand()%this->grid->getRows();

		GridNode* gNode = grid->getNode(randx, randy);

		if(gNode->isClear())
		{
			this->moveTo(gNode);		
			this->setCurrentNode(gNode);
		}		

	}

		this->updateLocomote(deltaTime);	
		this->updateAnimations(deltaTime);


	
}

void Agent::moveTo(GridNode* node)
{
	//if(!(node->isClear()))
	//	return;
	Astar* a = new Astar(this->grid, this->getCurrentNode(), node);
	bool pathfound = a->findPath();
	if(pathfound)
	{
		//move to the first one in find path
		std::list<GridNode*> destNodeList;
		
		int i = 0;
		
		//this->mWalkList.clear();
		
		while(a->getPath(i) != NULL)
		{
			GridNode* gn = a->getPath(i);//the nodes are coming off in reverse order so keep track of them 
			destNodeList.push_back(gn);//push node that you just got to the back of destNodeList
			GridNode* currentnode = this->getCurrentNode();
			
			Ogre::Vector3 nodeVect = a->getPath(i)->getPosition();//get position of the node to travel to
			Ogre::Vector3 agentVect = Ogre::Vector3(0,this->height, 0);//get position of this agent
			Ogre::Vector3 destination = nodeVect + agentVect;//add them together to get the destination (accounts for height)
			
			if(destination == (agentVect + this->getCurrentNode()->getPosition()))
			{
				return;
			}
			
			this->path.push_back(a->getPath(i));
			//GridNode* previousCurrent = this->getCurrentNode();
			//
			//this->setCurrentNode(a->getPath(i));
			//
			a->getPath(i)->contains = '0' + i - 1;
			this->mWalkList.push_back(destination);//push it onto the walklist, but push each node to the front since in reverse order
			i++;	
		}
	}
}

Ogre::Vector3 Agent::gCoordToVector(Grid* grid, GridNode* gNode, bool setOccupied)/*returns an Ogre::Vector3 of this gNode;
																		  if setOccupied is set to true, this node will become and
																		  obstacle.*/
{
	std::ofstream outFile; 
	outFile.open("ORIGINLIST.txt");

	float xoffset = ((grid->getRows() * grid->getHeight()) - (grid->getHeight()))/2;
	float zoffset = ((grid->getCols() * grid->getHeight()) - (grid->getHeight()))/2;

	float xDest = ((gNode->getX() * grid->getHeight())) - xoffset;	
	float zDest = ((gNode->getY() * grid->getHeight())) - zoffset;	


	Ogre::Vector3 destVector(xDest, 0, zDest);

	outFile << "xOriginVector: " << xoffset <<std::endl;
	outFile << "zOriginVector: " << zoffset <<std::endl;
	outFile << "xDestVector: " << xDest <<std::endl;
	outFile << "zDestVector: " << zDest <<std::endl;

	outFile.close();
	if(setOccupied)
		gNode->setOccupied();
	
	return destVector;

}
