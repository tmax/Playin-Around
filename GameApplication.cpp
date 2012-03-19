#include "GameApplication.h"
//#include "aStar.h"
#include "Sinbad.h"
#include "Ninja.h"
#include "Robot.h"
#include "Fish.h"
#include "Penguin.h"
#include "Player.h"


//-------------------------------------------------------------------------------------
GameApplication::GameApplication(void): // Lecture 8
mCurrentObject(0),
//mCount(0),
bLMouseDown(false),
bRMouseDown(false)
{
}
//-------------------------------------------------------------------------------------
GameApplication::~GameApplication(void)
{
}

//-------------------------------------------------------------------------------------
void GameApplication::createScene(void)
{
	loadEnv();
	setupEnv();
	loadObjects();
	loadCharacters();
}


void // Load the buildings or ground plane, etc
GameApplication::loadEnv()
{
	this->grid = new Grid(20,20);
	using namespace Ogre;

	// create a floor mesh resource
	/*MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	Plane(Vector3::UNIT_Y, 0), 100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z);*/

	MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	Plane(Vector3::UNIT_Y, 0), 200, 200, 20, 20, true, 1, 10, 10, Vector3::UNIT_Z);

	// create a floor entity, give it a material, and place it at the origin
    Entity* floor = mSceneMgr->createEntity("Floor", "floor");
    floor->setMaterialName("Examples/Rockwall");
	floor->setCastShadows(false);
    mSceneMgr->getRootSceneNode()->attachObject(floor);

	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
}



void // Set up lights, shadows, etc
GameApplication::setupEnv()
{
	using namespace Ogre;

	// set background and some fog
	//No fog, thank you very much.  
	//mSceneMgr->setFog(Ogre::FOG_LINEAR, ColourValue(1.0f, 1.0f, 0.8f), 0, 15, 100);

	// set shadow properties
	mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTextureSize(1024);
	mSceneMgr->setShadowTextureCount(1);

	// disable default camera control so the character can do its own
	mCameraMan->setStyle(OgreBites::CS_FREELOOK); //CS_FREELOOK, CS_ORBIT, CS_MANUAL

	// use a small amount of ambient lighting
	mSceneMgr->setAmbientLight(ColourValue(0.3f, 0.3f, 0.3f));

	// add a bright light above the scene
	Light* light = mSceneMgr->createLight();
	light->setType(Light::LT_POINT);
	light->setPosition(-10, 40, 20);
	light->setSpecularColour(ColourValue::White);

//////////////////////////////////////////////////////////////////////////////////
	// Lecture 8
	//but we also want to set up our raySceneQuery after everything has been initialized
	mRayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
}

Ogre::Vector3 GameApplication::gCoordToVector2(Grid* grid, GridNode* gNode)
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

	

	return destVector;

}

Ogre::Vector3 GameApplication::gCoordToVector(Grid* grid, GridNode* gNode, bool setOccupied)/*returns an Ogre::Vector3 of this gNode;
																		  if setOccupied is set to true, this node will become and
																		  obstacle.*/
{
	std::ofstream outFile; 
	outFile.open("ORIGINLIST.txt");

	float xoffset = ((grid->getCols() * grid->getHeight()) - (grid->getHeight()))/2;
	float zoffset = ((grid->getRows() * grid->getHeight()) - (grid->getHeight()))/2;

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

void // Load other propers or objects (e.g. furniture)
GameApplication::loadObjects()
{
	using namespace Ogre;

	Ogre::Entity *ent;
	Ogre::SceneNode *node;

		ent = mSceneMgr->createEntity("toilet.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(8, 12), true));
		node->attachObject(ent);
		node->setScale(1.0f, 1.0f, 1.0f);

		ent = mSceneMgr->createEntity("toilet.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(10, 6), true));
		node->attachObject(ent);
		node->setScale(1.0f, 1.0f, 1.0f);

		ent = mSceneMgr->createEntity("toilet.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(12, 18), true));
		node->attachObject(ent);
		node->setScale(1.0f, 1.0f, 1.0f);

		ent = mSceneMgr->createEntity("toilet.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(2, 14), true));
		node->attachObject(ent);
		node->setScale(1.0f, 1.0f, 1.0f);

		ent = mSceneMgr->createEntity("toilet.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(5, 5), true));
		node->attachObject(ent);
		node->setScale(1.0f, 1.0f, 1.0f);

		ent = mSceneMgr->createEntity("toilet.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(15, 15), true));
		node->attachObject(ent);
		node->setScale(1.0f, 1.0f, 1.0f);

		ent = mSceneMgr->createEntity("toilet.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(17, 3), true));
		node->attachObject(ent);
		node->setScale(1.0f, 1.0f, 1.0f);

		ent = mSceneMgr->createEntity("column.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(0, 19), true));
		node->attachObject(ent);
		node->setScale(.10f, .10f, .10f);

		//##################################################GUNS!!!!!!!!!!!!!!!!!!

		ent = mSceneMgr->createEntity("38pistol.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(13, 2)));
		GridNode n = *(grid->getNode( 13, 2 ));
		n.addE( ent );//->insert( n.itemList->end(), ent );
		node->attachObject(ent);
		node->pitch(Degree(90));
		node->translate(Ogre::Vector3(0,1,0));
		node->setScale(.150f, .150f, .150f);

		ent = mSceneMgr->createEntity("38pistol.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(5, 18)));
		grid->getNode(5,18)->getItemList()->push(ent);
		node->attachObject(ent);
		node->pitch(Degree(90));
		node->translate(Ogre::Vector3(0,1,0));
		node->setScale(.150f, .150f, .150f);

		ent = mSceneMgr->createEntity("38pistol.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(2, 12)));		
		grid->getNode(2,12)->getItemList()->push(ent);
		node->attachObject(ent);
		node->pitch(Degree(90));
		node->translate(Ogre::Vector3(0,1,0));
		node->setScale(.150f, .150f, .150f);

		ent = mSceneMgr->createEntity("38pistol.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(11,11)));
		grid->getNode(11,11)->getItemList()->push(ent);
		node->attachObject(ent);
		node->pitch(Degree(90));
		node->translate(Ogre::Vector3(0,1,0));
		node->setScale(.150f,.150f,.150f);

		ent = mSceneMgr->createEntity("38pistol.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(14,10)));
		grid->getNode(14,10)->getItemList()->push(ent);
		node->attachObject(ent);
		node->pitch(Degree(90));
		node->translate(Ogre::Vector3(0,1,0));
		node->setScale(.350f, .350f, .350f);

		ent = mSceneMgr->createEntity("38pistol.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(10,14)));
		grid->getNode(10,14)->getItemList()->push(ent);		
		node->attachObject(ent);
		node->pitch(Degree(90));
		node->translate(Ogre::Vector3(0,1,0));
		node->setScale(.150f, .150f, .150f);

		ent = mSceneMgr->createEntity("38pistol.mesh");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(1,1)));
		grid->getNode(1,1)->getItemList()->push(ent);
		node->attachObject(ent);
		node->pitch(Degree(90));
		node->translate(Ogre::Vector3(0,1,0));
		node->setScale(.150f,.150f,.150f);



		//##########################################END GUNS

}
void GameApplication::moveLoop(Agent* agent, Grid* grid)
{
	//int randx = rand()%10;
	//int randy = rand()%10;

	std::ofstream outFile; 
	outFile.open("RandomPointList.txt");

	
	for(int i = 0; i<50; i++)
	{
		int randx = rand()%10;
		int randy = rand()%10;

		GridNode* gNode = grid->getNode(randx, randy);

		int currentX = agent->getCurrentNode()->getX();
		int currentY = agent->getCurrentNode()->getY();
		


		outFile << "("<<currentX<<", "<<currentY<<")\n";
		

		//Astar* a = new Astar(grid, agent->getCurrentNode(), gNode);

		if(gNode->isClear())
		{
			agent->moveTo(gNode);
			agent->setCurrentNode(gNode);
		}
		else
		{
			//agent->setCurrentNode(grid->getNode(currentX, currentY));
		}
	}
	outFile.close();
}

void // Load actors, agents or characters
GameApplication::loadCharacters()
{
	//use pointers not references to keep in memory...
	//Grid* grid = this->grid;
	//GridNode* g = grid->getNode(5,5); 

	Sinbad* player = new Sinbad(true, "Sinbad1", "Sinbad.mesh");
	//agentList.push_back(player);
	player->doUpdate = false;
	player->setPosition(gCoordToVector(grid, grid->getNode(10,10)));
	player->setGrid(grid);
	player->setCurrentNode(grid->getNode(10,10));
	player->moveTo(grid->getNode(14,10));

	this->setPlayerPtr(player);

	//Ogre::Entity* ent = mSceneMgr->createEntity("38pistol.mesh");
	//Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode(gCoordToVector(grid, grid->getNode(10,10)));
	//grid->getNode(10,10)->getItemList()->push(ent);
	//node->attachObject(ent);
	//node->pitch(Ogre::Degree(90));
	//node->translate(Ogre::Vector3(0,1,0));
	//node->setScale(.150f,.150f,.150f);

	

	//for(int i = 0; i< 50; i++)
	//{
	//	int randx = rand()%this->grid->getCols();
	//	int randy = rand()%this->grid->getRows();

	//	GridNode* gNode = grid->getNode(randx, randy);

	//	if(gNode->isClear())
	//	{
	//		sinbad1->moveTo(gNode);		
	//		sinbad1->setCurrentNode(gNode);
	//	}		
	//}

	Sinbad* sinbad2 = new Sinbad(true, "Sinbad2", "Sinbad.mesh");
	agentList.push_back(sinbad2);
	sinbad2->setPosition(gCoordToVector(grid, grid->getNode(0,0)));
	sinbad2->setGrid(grid);
	sinbad2->setCurrentNode(grid->getNode(0,0));

	//for(int i = 0; i< 50; i++)
	//{
	//	int randx = rand()%grid->getCols();
	//	int randy = rand()%grid->getRows();

	//	GridNode* gNode = grid->getNode(randx, randy);

	//	if(gNode->isClear())
	//	{
	//		sinbad2->moveTo(gNode);		
	//		sinbad2->setCurrentNode(gNode);
	//	}		
	//}
 
	Ninja* ninja1 = new Ninja(true, "Ninja", "Ninja.mesh");
	agentList.push_back(ninja1);
	ninja1->setPosition(gCoordToVector(grid, grid->getNode(14, 0)));
	ninja1->setGrid(grid);
	ninja1->setCurrentNode(grid->getNode(14,0));
	//ninja1->moveTo(grid->getNode(0,14));



	//for(int i = 0; i< 50; i++)
	//{
	//	int randx = rand()%grid->getCols();
	//	int randy = rand()%grid->getRows();

	//	GridNode* gNode = grid->getNode(randx, randy);

	//	if(gNode->isClear())
	//	{
	//		ninja1->moveTo(gNode);		
	//		ninja1->setCurrentNode(gNode);
	//	}		
	//}

	Robot* robot1 = new Robot(true, "Robo1", "Robot.mesh");
	agentList.push_back(robot1);
	robot1->setPosition(gCoordToVector(grid, grid->getNode(19, 0)));
	robot1->setGrid(grid);
	robot1->setCurrentNode(grid->getNode(19,0));

	//for(int i = 0; i< 50; i++)
	//{
	//	int randx = rand()%grid->getCols();
	//	int randy = rand()%grid->getRows();

	//	GridNode* gNode = grid->getNode(randx, randy);

	//	if(gNode->isClear())
	//	{
	//		robot1->moveTo(gNode);		
	//		robot1->setCurrentNode(gNode);
	//	}		
	//}

	// ME Player* playa = new Player(sinbad1);
	
	//Ninja* ninja1 = new Ninja(true, "Sinbad2", "Ninja.mesh");
	//agentList.push_back(ninja1);
	//ninja1->setPosition(gCoordToVector(grid, grid->getNode(5,5)));

	//Penguin* penguin1 = new Penguin(true, "Pengy2", "Penguin.mesh");
	//agentList.push_back(penguin1);
	//penguin1->setPosition(-35, 0, -45);//(1, 0) on grid

	//Robot* robot2 = new Robot(true, "Robo2", "Robot.mesh");
	//agentList.push_back(robot2);
	//robot2->setPosition(45, 0, -45);

	//fish1->setGrid(grid);
	//robot1->setGrid(grid);
	//ninja1->setGrid(grid);
	//penguin1->setGrid(grid);
	//robot2->setGrid(grid);

	//sinbad1->setCurrentNode(grid->getNode(1, 1));
	//fish1->setCurrentNode(grid->getNode(1, 0));
	//robot1->setCurrentNode(grid->getNode(9, 0));
	//ninja1->setCurrentNode(grid->getNode(5,5));
	//penguin1->setCurrentNode(grid->getNode(1, 0));
	//robot2->setCurrentNode(grid->getNode(9, 0));
	
	//grid->printToFile();
}

void 
GameApplication::addTime(Ogre::Real deltaTime)
{
	// update all agents
	std::list<Agent*>::iterator iter;
	for (iter = agentList.begin(); iter != agentList.end(); iter++)
	{
		(*iter)->update(deltaTime);
		this->getPlayerPtr()->update(deltaTime);
	}

	//updateCamera(deltaTime);
}

//-------------------------------------------------------------------------------------
// Lecture 7 (moved into GameApplication)
bool GameApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
	else if(arg.key == OIS::KC_SPACE)
	{

		std::list<Agent*>::iterator iter;
		for (iter = agentList.begin(); iter != agentList.end(); iter++)
		{
			(*iter)->getmWalkList()->clear();
			
			(*iter)->doUpdate  = (*iter)->doUpdate ^= 1;
			(*iter)->getmBodyNode()->lookAt((this->getPlayerPtr()->getCurrentNode()->getPosition() + (*iter)->getAgentHeight()), Ogre::Node::TS_WORLD);
		}


		
		//std::list<Agent*>::iterator iter;
		//GridNode* playerNode = this->getPlayerPtr()->getCurrentNode();
		//Ogre::Vector3 playerVect= gCoordToVector(grid, playerNode);//replace with the vector and gnode of player

		//for(iter = agentList.begin(); iter != agentList.end(); iter++);
		//{
		//	(*iter)->getmobdyngetmBodyNode()->lookAt(playerVect, Ogre::Node::TS_WORLD);
		//}
	}
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
	/////////////////////////////////////////////////////////////////////////////////////
	else if (arg.key == OIS::KC_1) // Lecture 7 (added to pasted code)
	{
		//agentList.front()->drawSwords();  // Assuming too much: front of list is player and Sinbad
	}
	else if (arg.key == OIS::KC_2) // Look around (sort of)
	{
		Agent *agt = agentList.front(); // Again just interacting with the first agent on the list
		if (agt != NULL)
		{
			//agt->lookAround();
		}
	}
	else if (arg.key == OIS::KC_3) // Grab the gun
	{
		Agent *agt = agentList.front(); // Yes, assuming the same agent
		if (agt != NULL)
		{
			//agt->holdGun();
		}
	}
	else if (arg.key == OIS::KC_4) // toggle gun appearance
	{
		Ogre::Entity* ent = sceneMgr->getEntity("Gun");
		if (ent != NULL) 		
			ent->setVisible(!ent->getVisible()); // toggle visiblity
	}
	else if (arg.key == OIS::KC_5) // move 9mm a bit
	{
		sceneMgr->getSceneNode("GunNode2")->translate(0,0,-1);
	}
	else if (arg.key == OIS::KC_6) // move 9mm a bit
	{
		sceneMgr->getSceneNode("GunNode2")->translate(0,0,1);
	}
///////////////////////////////////////////////////////////////////////////////////////
    mCameraMan->injectKeyDown(arg);
    return true;
}

bool GameApplication::keyReleased( const OIS::KeyEvent &arg )
{
    mCameraMan->injectKeyUp(arg);

   // return true;
	return BaseApplication::keyReleased(arg);

}

bool GameApplication::mouseMoved( const OIS::MouseEvent &arg )
{
	// Lecture 8
	//if the left mouse button is held down
	if(bLMouseDown)
	{
		//find the current mouse position
		//CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
		Ogre::Vector3 mousePos; 
		mousePos.x = arg.state.X.abs;
		mousePos.y = arg.state.Y.abs;
		mousePos.z = arg.state.Z.abs;
 
		//create a raycast straight out from the camera at the mouse's location
		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.x/float(arg.state.width), mousePos.y/float(arg.state.height));
		mRayScnQuery->setRay(mouseRay);
		mRayScnQuery->setSortByDistance(false);	//world geometry is at the end of the list if we sort it, so lets not do that
 
		Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
		Ogre::RaySceneQueryResult::iterator iter = result.begin();
 
		//check to see if the mouse is pointing at the world and put our current object at that location
		for(iter; iter != result.end(); iter++)
		{
			//if(iter->worldFragment) // Only works for terrain?
			//{
			//	mCurrentObject->setPosition(iter->worldFragment->singleIntersection);
			//	break;
			//}	
			if (iter->movable)
			{
				mCurrentObject->setPosition(iter->movable->getParentSceneNode()->getPosition());
			}
		}
	}
	else if(bRMouseDown)	//if the right mouse button is held down, be rotate the camera with the mouse
	{
		mCamera->yaw(Ogre::Degree(-arg.state.X.rel * 0.1));
		mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * 0.1));
	}
	/////////////////////////////////////////////////////////////////////////////////////////

    if (mTrayMgr->injectMouseMove(arg)) return true;
    //mCameraMan->injectMouseMove(arg);
    return true;
}

bool GameApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	//////////////////////////////////////////////////////////////////////////////////////
	// Lecture 8
	if(id == OIS::MB_Left)
	{
		//show that the current object has been deselected by removing the bounding box visual
		if(mCurrentObject)
		{
			mCurrentObject->showBoundingBox(false);
		}
 
		//find the current mouse position
		//CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
		Ogre::Vector3 mousePos;
		mousePos.x = arg.state.X.abs;
		mousePos.y = arg.state.Y.abs;
		mousePos.z = arg.state.Z.abs;
		
 
		//then send a raycast straight out from the camera at the mouse's position
		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.x/float(arg.state.width), mousePos.y/float(arg.state.height));
		mRayScnQuery->setRay(mouseRay);
		mRayScnQuery->setSortByDistance(true);
		//mRayScnQuery->setQueryMask(bRobotMode ? ROBOT_MASK : NINJA_MASK);	//will return objects with the query mask in the results
 
		/*
		This next chunk finds the results of the raycast
		If the mouse is pointing at world geometry we spawn a robot at that position
		*/
		Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
		Ogre::RaySceneQueryResult::iterator iter = result.begin();
 
		for(iter; iter != result.end(); iter++)
		{
			//if you clicked on a robot or ninja it becomes selected
			if(iter->movable && iter->movable->getName().substr(0, 5) != "tile[")
			{
				mCurrentObject = iter->movable->getParentSceneNode();
				break;
			}
		}
 
		//now we show the bounding box so the user can see that this object is selected
		if(mCurrentObject)
		{
			mCurrentObject->showBoundingBox(true);
		}
 
		bLMouseDown = true;
	}
	else if (id == OIS::MB_Right)
		bRMouseDown = true;

	//////////////////////////////////////////////////////////////////////////////////////
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool GameApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	// Lecture 8
	if(id  == OIS::MB_Left)
	{
		bLMouseDown = false;
	}
	if(id  == OIS::MB_Right)
	{
		bRMouseDown = false;
	}
	else
		mCameraMan->injectMouseUp(arg, id);

	//////////////////////////////////////////////////////////////////////////////
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    
    return true;
}
