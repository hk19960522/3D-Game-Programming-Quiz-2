//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// Date: 2018/11/09
//
#include "TutorialApplication.h"
#include "BasicTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace Ogre;

const float PI = 3.141592654;

#define mMoveDirection_NONE 0
#define mMoveDirection_DOWN (1 << 0)
#define mMoveDirection_UP   (1 << 1)
#define mMoveDirection_LEFT (1 << 2)
#define mMoveDirection_RIGHT (1<<3)


BasicTutorial_00::BasicTutorial_00(void)
{
	mMoveDirection = Vector3::ZERO;
	mMoveSpeed = 100;
}

void BasicTutorial_00::chooseSceneManager()
{
	mSceneMgrArr[0] = mRoot
		->createSceneManager(ST_GENERIC, "primary");
	mSceneMgrArr[1] = mRoot
		->createSceneManager(ST_GENERIC, "secondary");
}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(120,300,600));
	mCamera->lookAt(Ogre::Vector3(120,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	mSceneMgr = mSceneMgrArr[1];
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0,350,0.0));
	mCamera->lookAt(Ogre::Vector3(0.00001,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller

}



void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0.0,1.0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mViewportArr[0] = vp;
}

void BasicTutorial_00::createViewport_01(void)
{
}

void BasicTutorial_00::resolveCollision(
    SceneNode *nodeA, SceneNode *nodeB,
    float rA, float rB, bool moveA, bool moveB, 
	float time, float speed)
{
    Vector3 posA = nodeA->getPosition();
    Vector3 posB = nodeB->getPosition();
    float R = rA + rB;
    ///////////////////////
    // add your own stuff
    ///////////////////////

	Vector3 dir = posB - posA;
	Vector3 dirA = Vector3::ZERO, dirB = Vector3::ZERO;
	float dis = dir.normalise();

	if (dis < R) {
		if (moveA && moveB) {
			dirA = dir * -0.5 * time;
			dirB = dir * 0.5 * time;
		}
		else if (moveA) {
			dirA = dir * -time;
			dirB = Vector3::ZERO;
		}
		else {
			dirA = Vector3::ZERO;
			dirB = dir * time;
		}
	}
	dirA.y = 0;
	dirB.y = 0;
	nodeA->translate(dirA * mMoveSpeed);
	nodeB->translate(dirB * mMoveSpeed);
}

void BasicTutorial_00::resolveCollisionLargeSphere(float time)
{
    float smallR = 15; // small sphere radius

    float largeR = 1.0/0.15*smallR; // large sphere radius

	for (int i=0;i<mNumSpheres;i++){
		SceneNode *small = mSceneNode[i];
		resolveCollision(mLargeSphereSceneNode, small, largeR, smallR, false, true, time, mMoveSpeed);
	}
	for (int i = 0; i < mNumObstacles; i++) {
		int index = i + mNumSpheres;
		SceneNode *small = mSceneNode[index];
		resolveCollision(mLargeSphereSceneNode, small, largeR, smallR, true, false, time, mMoveSpeed);
	}	
}

// perform collision handling for all pairs
void BasicTutorial_00::resolveCollisionSmallSpheres(float time)
{
    float ri = 15; // sphere radius
    float rj = 15; // sphere radius
    for (int i = 0; i < mNumSpheres; ++i)
	{
		for (int j = 0; j < mNumObstacles; j++) {
			int index = j + mNumSpheres;
			resolveCollision(mSceneNode[i], mSceneNode[index], ri, rj, true, false, time, mMoveSpeed / 2);
		}	
		for (int j = i+1; j < mNumSpheres; ++j) {
            ///////////////////////
            // add your own stuff
            ///////////////////////
			resolveCollision(mSceneNode[i], mSceneNode[j], ri, rj, true, true, time, mMoveSpeed / 2);
        }
    }
}

void BasicTutorial_00::resolveCollision(float time)
{
    int num = 10;
    for (int i = 0; i < num;++i) {
        resolveCollisionSmallSpheres(time);
        resolveCollisionLargeSphere(time);
    }
}

// reset positions of all small spheres
void BasicTutorial_00::reset()
{
	mMoveDirection = Vector3::ZERO;
    for (int i = 0; i < mNumSpheres; ++i ) {
        ///////////////////////
        // add your own stuff
        ///////////////////////
		float x = (float)(rand()%800)-400;
		float z = (float)(rand()%800)-400;
		mSceneNode[i]->setPosition(x, 0, z);
    }
}

// create all spheres
// "Examples/red"
// "Examples/green"
// "Examples/blue"
// "Examples/yellow"
void BasicTutorial_00::createSpace()
{
    String name_en;
    String name_sn;
	// Large Sphere
	mLargeSphereEntity = mSceneMgr->createEntity("LargeSphere", "sphere.mesh");
	mLargeSphereEntity->setMaterialName("Examples/yellow");
	mLargeSphereSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mLargeSphereSceneNode->setPosition(0, 0, 0);
	mLargeSphereSceneNode->attachObject(mLargeSphereEntity);

	mNumSpheres = 300;
	int index = 0;
	// Small Sphere
    for (int i = 0; i < mNumSpheres; ++i ) {

		genNameUsingIndex("R1", i, name_en);
		genNameUsingIndex("S1", i, name_sn);
		mEntity[index] = mSceneMgr
			->createEntity( name_en, "sphere.mesh" );

		mSceneNode[index] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mSceneNode[index]->scale(0.15, 0.15, 0.15);
		mSceneNode[index]->attachObject(mEntity[i]);

		switch(rand()%3) {
		case 0:
			mEntity[index]->setMaterialName("Examples/red");
			break;
		case 1:
			mEntity[index]->setMaterialName("Examples/blue");
			break;
		case 2:
			mEntity[index]->setMaterialName("Examples/green");
			break;
		}

		float x = (float)(rand()%800)-400;
		float z = (float)(rand()%800)-400;
		mSceneNode[index]->setPosition(x, 0, z);
		index++;
    }

	// Barrels
	int offset = mNumSpheres;
    mNumObstacles = 80;
    for (int i = 0; i < mNumObstacles; ++i ) {
        int index = i+offset;
        genNameUsingIndex("R1", index, name_en);
        genNameUsingIndex("S1", index, name_sn);
        mEntity[index] = mSceneMgr
            ->createEntity( name_en, "Barrel.mesh" );
        mSceneNode[index] = mSceneMgr
            ->getRootSceneNode()
            ->createChildSceneNode( 
            name_sn); 

        mSceneNode[index]->scale(10.0, 10., 10.0);
        mSceneNode[index]->attachObject(mEntity[index]);
    }

	float r_obs = 28.0; 	// barrel radius
    float d_gap = 30.0;
    for (int i = 0; i < 20; ++i) {
        int index = i+offset;

        float x = -600.0+2*d_gap;
        float y = 10.0;
        float z = -600.0;
        x += 2*d_gap*i;
        mSceneNode[index]->setPosition(x,y,z);
    }

	offset += 20;
    for (int i = 0; i < 20; ++i) {
        int index = i+offset;

        float x = -600.0;
        float y = 10.0;
        float z = 600.0;
        x += 2*d_gap*i;
        mSceneNode[index]->setPosition(x,y,z);
    }

    offset += 20;
	for (int i = 0; i < 20; ++i) {
        int index = i+offset;

        float x = -600.0;
        float y = 10.0;
        float z = -600.0;
        z += 2*d_gap*i;
        mSceneNode[index]->setPosition(x,y,z);
    }

	offset += 20;
    for (int i = 0; i < 20; ++i) {
        int index = i+offset;

        float x = 600.0;
        float y = 10.0;
        float z = -600.0+2*d_gap;
        z += 2*d_gap*i;
        mSceneNode[index]->setPosition(x,y,z);
    }
}

void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];
	//mSceneMgr->setAmbientLight( ColourValue( 0.25, 0.25, 0.25 ) ); 

	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  
	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	Light *light;
	light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(150, 250, 100)); 
	light->setDiffuseColour(0.3, 0.3, 0.3);		
	light->setSpecularColour(0.5, 0.5, 0.5);	

	light = mSceneMgr->createLight("Light2"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(-150, 300, 250)); 
	light->setDiffuseColour(0.25, 0.25, 0.25);		
	light->setSpecularColour(0.35, 0.35, 0.35);	

	//
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent = mSceneMgr->createEntity(
		"GroundEntity", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	//ent->setMaterialName("Examples/Rockwall");


	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent); 

    ent = mSceneMgr->createEntity(
		"Background", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	ent->setMaterialName("Examples/Rockwall");

    //Radian angle((45/180.0)*3.141592654);
    Radian angle(3.141952654/2.0);

    Vector3 axis(1.0, 0.0, 0.0);
    mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
            Vector3(0.0, 0.0, -750.0),
            Quaternion( angle, axis))
		->attachObject(ent);

    createSpace();

    ///////////////////////
        // add your own stuff
        ///////////////////////
    
    resolveCollision(0.5);
}

void BasicTutorial_00::createScene_01(void) 
{

}

void BasicTutorial_00::createViewports(void)
{
	createViewport_00();
	createViewport_01();
}

void BasicTutorial_00::createCamera(void) {
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
	createScene_00();
	createScene_01();
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[0];
    //mCamera = mCameraArr[1];
    //
    mCameraMan->getCamera()
            ->setPosition(Vector3(-22.30,	409.24,	816.74));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.02,	-0.23,	-0.97));

}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
//
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyPressed( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyPressed ***\n";
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    
    if (arg.key == OIS::KC_C ) {
        
        //How to clear ss?
        ss.str("");
        ss.clear();
        
        //stringstream ss; // Second way

        // Third way?
        //=============

        // How to get camerea position?
        //-----------------------------
        //This is incorrect.
        //Vector3 pos = mCamera->getPosition();
        //-----------------------------
        Vector3 pos = mCameraMan->getCamera()->getPosition(); //Correct
        ss << std::fixed << std::setprecision(2) 
            << "CameraPosition:" 
            << pos.x << "\t" 
            << pos.y << "\t" 
            << pos.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
        ss.str("");
        ss.clear();
        Vector3 dir = mCameraMan->getCamera()->getDirection();
        ss << std::fixed << std::setprecision(2) 
            << "CameraDirection:" 
            << dir.x << "\t" 
            << dir.y << "\t" 
            << dir.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
    }

    if (arg.key == OIS::KC_1 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(-77.89,   169.11,  1996.70));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.05,    -0.25,   -0.97));

    }

    if (arg.key == OIS::KC_2 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(-824.52,	468.58,	68.45));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.94,	-0.31,	-0.11));

        //-1463.00	606.45	-513.24
        //0.88	-0.47	0.10
    }


    if (arg.key == OIS::KC_3 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(-0.0,   1608.68, 0.0));
		mCameraMan->getCamera()->lookAt(0.0, 0.0, -0.01);

        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }

	if (arg.key == OIS::KC_I) {
		mMoveDirection = Vector3(0, 0, -1);
	}
	
	if (arg.key == OIS::KC_K) {
		mMoveDirection = Vector3(0, 0, 1);
	}

	if (arg.key == OIS::KC_J) {
		mMoveDirection = Vector3(-1, 0, 0);
	}

	if (arg.key == OIS::KC_L) {
		mMoveDirection = Vector3(1, 0, 0);
	}

    ///////////////////////
    // add your own stuff
    ///////////////////////

    if (arg.key == OIS::KC_B ) {
		reset();
    }


    BaseApplication::keyPressed(arg);

    return flg;
}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
// 
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyReleased( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyReleased ***\n";
    
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    BaseApplication::keyReleased(arg);

    return flg;
}

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
    //
	
	mLargeSphereSceneNode->translate(mMoveDirection * mMoveSpeed * evt.timeSinceLastFrame);

    ///////////////////////
    // add your own stuff
    ///////////////////////

	resolveCollision(evt.timeSinceLastFrame);
    //
    return flg;
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
