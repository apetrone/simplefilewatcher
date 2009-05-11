/**
	Implementation header file for OSX based on KEvent.

	@author James Wynn
	@date 4/15/2009

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
#ifndef _FW_FILEWATCHEROSX_H_
#define _FW_FILEWATCHEROSX_H_
#pragma once

#ifdef __APPLE_CC__

#include "FileWatcher.h"
#include <map>
#include <sys/types.h>

namespace FW
{

	// forward decl
	struct WatchStruct;
	
	///
	/// @class FileWatcherOSX
	class FileWatcherOSX
	{
	public:
		/// type for a map from WatchID to WatchStruct pointer
		typedef std::map<WatchID, WatchStruct*> WatchMap;

	public:
		///
		///
		FileWatcherOSX();

		///
		///
		virtual ~FileWatcherOSX();

		/// Add a directory watch
		WatchID addWatch(const String& directory, FileWatchListener* watcher);

		/// Remove a directory watch. This is a brute force lazy search O(nlogn).
		void removeWatch(const String& directory);

		/// Remove a directory watch. This is a map lookup O(logn).
		void removeWatch(WatchID watchid);

		/// Updates the watcher. Must be called often.
		void update();

		/// Handles the action
		void handleAction(WatchStruct* watch, const String& filename, unsigned long action);

	private:
		/// Map of WatchID to WatchStruct pointers
		WatchMap mWatches;
		/// The descriptor for the kqueue
		int mDescriptor;
		/// time out data
		struct timespec mTimeOut;
		/// WatchID allocator
		int mLastWatchID;

	};//end FileWatcherOSX

};//namespace FW

#endif//__APPLE_CC__

#endif//_FW_FILEWATCHEROSX_H_
