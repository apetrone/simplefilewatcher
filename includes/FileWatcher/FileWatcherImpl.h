/**
	Basic interface for the FileWatcher backend.

	@author James Wynn
	@date 5/11/2009

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
#ifndef _FW_FILEWATCHERIMPL_H_
#define _FW_FILEWATCHERIMPL_H_
#pragma once

#include "FileWatcher.h"

#define FILEWATCHER_PLATFORM_WIN32 1
#define FILEWATCHER_PLATFORM_LINUX 2
#define FILEWATCHER_PLATFORM_BSD   3

#if defined(_WIN32)
#	define FILEWATCHER_PLATFORM FILEWATCHER_PLATFORM_WIN32
#elif defined(__APPLE_CC__) || defined(BSD)
#	define FILEWATCHER_PLATFORM FILEWATCHER_PLATFORM_BSD
#elif defined(__linux__)
#	define FILEWATCHER_PLATFORM FILEWATCHER_PLATFORM_LINUX
#endif

namespace FW
{
	struct WatchStruct;

	class FileWatcherImpl
	{
	public:
		///
		///
		FileWatcherImpl() {}

		///
		///
		virtual ~FileWatcherImpl() {}

		/// Add a directory watch
		virtual WatchID addWatch(const String& directory, FileWatchListener* watcher) = 0;

		/// Remove a directory watch. This is a brute force lazy search O(nlogn).
		virtual void removeWatch(const String& directory) = 0;

		/// Remove a directory watch. This is a map lookup O(logn).
		virtual void removeWatch(WatchID watchid) = 0;

		/// Updates the watcher. Must be called often.
		virtual void update() = 0;

		/// Handles the action
		virtual void handleAction(WatchStruct* watch, const String& filename, unsigned long action) = 0;

	};//end FileWatcherImpl
};//namespace FW

#endif//_FW_FILEWATCHERIMPL_H_
