/**
	Demo app for FileWatcher. FileWatcher is a simple wrapper for the file
	modification system in windows. Run the demo and then execute the batch
	files in the media directory (red.bat, blue.bat, green.bat) and the
	textures will automatically reload in the window.

	@author James Wynn
	@date 2/25/2009
*/

#include "ExampleApplication.h"
#include <FileWatcher/FileWatcher.h>

using namespace Ogre;

// ugly globals for demo
FW::FileWatcher* gFileWatcher = 0;
FW::WatchID gWatchID = 0;


class FWTestFrameListener : public ExampleFrameListener
{
public:
	FWTestFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr)
		: ExampleFrameListener(win, cam, false, false)
	{
	}

	bool frameStarted(const FrameEvent &evt)
	{
		// update the file watcher every frame
		gFileWatcher->update();

		return ExampleFrameListener::frameStarted(evt);
	}
};//class FWTestFrameListener


/// Processes a file action
class TextureReloader : public FW::FileWatchListener
{
public:
	TextureReloader() {}
	void handleFileAction(FW::WatchID watchid, const String& dir, const String& filename, FW::FileWatcher::Action action)
	{
		// Get the texture with this name (if it exists)
		Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName(filename);

		// this is actually a bug. Sometimes the event is received before the system is
		// finished writing the change and you will get file contention errors. So, wait
		// for the write to finish.
		Sleep(100);

		// if the texture exists, reload it
		if(!tex.isNull())
			tex->reload();
	}
};

class FWTestApplication : public ExampleApplication
{
protected:
	Ogre::SceneNode *mNinjaNode;
	Ogre::Entity *mNinjaEntity;

public:
	FWTestApplication()
	{
		// create the file watcher object
		gFileWatcher = new FW::FileWatcher();
		// add a watch to the system
		gWatchID = gFileWatcher->addWatch("..\\media", new TextureReloader());
	}

	~FWTestApplication()
	{
		// file watcher releases all watches on cleanup
		delete gFileWatcher;
		gFileWatcher = 0;
	}

protected:
	void createScene(void)
	{
		//Some normal stuff.
		mSceneMgr->setAmbientLight(ColourValue(0.7,0.7,0.7));
		mCamera->setPosition(Vector3(0,0,100));
		mCamera->lookAt(Vector3::ZERO);
		mCamera->setNearClipDistance(0.05);
		mCamera->setFarClipDistance(300);
		LogManager::getSingleton().setLogDetail(LL_BOREME);

		//----------------------------------------------------------
		// Ninja!
		//----------------------------------------------------------

		Vector3 pos = Vector3(0,0,0);
		Quaternion rot = Quaternion::IDENTITY;

		//Create Ogre stuff.
		mNinjaEntity = mSceneMgr->createEntity("ogrehead", "ogrehead.mesh");
		mNinjaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ogrehead", pos, rot);
		mNinjaNode->attachObject(mNinjaEntity);
		mNinjaNode->translate(0,0,0);
		mNinjaNode->setScale(0.5,0.5,0.5);
	}

	void createFrameListener(void)
	{
		mFrameListener = new FWTestFrameListener(mWindow, mCamera, mSceneMgr);
		mRoot->addFrameListener(mFrameListener);
	}
};//class FWTestApplication



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
	// Create application object
	FWTestApplication app;

	try 
	{
		app.go();
	} 
	catch( Exception& e ) 
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox( NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		fprintf(stderr, "An exception has occurred: %s\n",
			e.what());
#endif
	}

	return 0;
}
