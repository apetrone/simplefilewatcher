/**
	Demo app for FileWatcher. FileWatcher is a simple wrapper for the file
	modification system in Windows and Linux.

	@author James Wynn
	@date 2/25/2009
*/

#include <FileWatcher/FileWatcher.h>
#include <iostream>

// ugly globals for demo
FW::FileWatcher* gFileWatcher = 0;
FW::WatchID gWatchID = 0;


/// Processes a file action
class UpdateListener : public FW::FileWatchListener
{
public:
	UpdateListener() {}
	void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
		FW::FileWatcher::Action action)
	{
		std::cout << "DIR (" << dir + ") FILE (" + filename + ") has event " << action << std::endl;
	}
};


int main(int argc, char **argv)
{
	try 
	{
		std::cin.get();
		// create the file watcher object
		gFileWatcher = new FW::FileWatcher();
		// add a watch to the system
		gWatchID = gFileWatcher->addWatch("./test", new UpdateListener());
		//gWatchID = gFileWatcher->addWatch("./build", new UpdateListener());
		
		while(1)
		{
			gFileWatcher->update();
		}
		
		delete gFileWatcher;
	} 
	catch( std::exception& e ) 
	{
		fprintf(stderr, "An exception has occurred: %s\n", e.what());
	}

	return 0;
}
