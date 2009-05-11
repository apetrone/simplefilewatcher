/**
	Implementation for Windows. Uses ReadDirectoryChangesW to watch for
	file system changes.

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
#ifndef _FW_FILEWATCHERWIN32_H_
#define _FW_FILEWATCHERWIN32_H_
#pragma once

#ifdef _WIN32

#include "FileWatcherImpl.h"
#include <map>

namespace FW
{
	// forward decl
	struct WatchStruct;

	///
	/// @class FileWatcherWin32
	class FileWatcherWin32 : public FileWatcherImpl
	{
	public:
		/// type for a map from WatchID to WatchStruct pointer
		typedef std::map<WatchID, WatchStruct*> WatchMap;

	public:
		///
		///
		FileWatcherWin32();

		///
		///
		virtual ~FileWatcherWin32();

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
		/// The last watchid
		WatchID mLastWatchID;

	};//end FileWatcherWin32

};//namespace FW

#endif//_WIN32

#endif//_FW_FILEWATCHERWIN32_H_
