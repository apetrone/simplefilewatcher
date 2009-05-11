/**
	Copyright (c) 2009 James Wynn

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.

	James Wynn james@jameswynn.com
*/

#include <FileWatcher/FileWatcher.h>

#if FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_WIN32
#	include <FileWatcher/FileWatcherWin32.h>
#	define FILEWATCHERIMPLE FileWatcherWin32
#elif FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_BSD
#	include <FileWatcher/FileWatcherOSX.h>
#	define FILEWATCHERIMPLE FileWatcherOSX
#elif FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_LINUX
#	include <FileWatcher/FileWatcherLinux.h>
#	define FILEWATCHERIMPLE FileWatcherLinux
#endif

namespace FW
{

	//--------
	FileWatcher::FileWatcher()
	{
		mImpl = new FILEWATCHERIMPLE();
	}

	//--------
	FileWatcher::~FileWatcher()
	{
		delete mImpl;
		mImpl = 0;
	}

	//--------
	WatchID FileWatcher::addWatch(const String& directory, FileWatchListener* watcher)
	{
		return mImpl->addWatch(directory, watcher);
	}

	//--------
	void FileWatcher::removeWatch(const String& directory)
	{
		mImpl->removeWatch(directory);
	}

	//--------
	void FileWatcher::removeWatch(WatchID watchid)
	{
		mImpl->removeWatch(watchid);
	}

	//--------
	void FileWatcher::update()
	{
		mImpl->update();
	}

};//namespace FW
