/**
	Main header for the FileWatcher class. Declares all implementation
	classes to reduce compilation overhead.

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

#ifndef _FW_FILEWATCHER_H_
#define _FW_FILEWATCHER_H_
#pragma once

#include <string>

namespace FW
{
	/// Type for a string
	typedef std::string String;
	/// Type for a watch id
	typedef unsigned long WatchID;

	// forward declarations
	class FileWatcherImpl;
	class FileWatchListener;

	/// Listens to files and directories and dispatches events
	/// to notify the parent program of the changes.
	/// @class FileWatcher
	class FileWatcher
	{
	public:

	public:
		/// Actions to listen for. Rename will send two events, one for
		/// the deletion of the old file, and one for the creation of the
		/// new file.
		enum Action
		{
			/// Sent when a file is created or renamed
			ACTION_ADD = 1,
			/// Sent when a file is deleted or renamed
			ACTION_DELETE = 2,
			/// Sent when a file is modified
			ACTION_MODIFIED = 4
		};

	public:
		///
		///
		FileWatcher();

		///
		///
		virtual ~FileWatcher();

		/// Add a directory watch
		WatchID addWatch(const String& directory, FileWatchListener* watcher);

		/// Remove a directory watch. This is a brute force search O(nlogn).
		void removeWatch(const String& directory);

		/// Remove a directory watch. This is a map lookup O(logn).
		void removeWatch(WatchID watchid);

		/// Updates the watcher. Must be called often.
		void update();

	private:
		/// The implementation
		FileWatcherImpl* mImpl;

	};//end FileWatcher


	/// Basic interface for listening for file events.
	/// @class FileWatchListener
	class FileWatchListener
	{
	public:
		FileWatchListener() {}
		virtual ~FileWatchListener() {}

		/// Handles the action file action
		/// @param watchid The watch id for the directory
		/// @param dir The directory
		/// @param filename The filename that was accessed (not full path)
		/// @param action Action that was performed
		virtual void handleFileAction(WatchID watchid, const String& dir, const String& filename, FileWatcher::Action action) = 0;

	};//class FileWatchListener

};//namespace FW

#endif//_FW_FILEWATCHER_H_
