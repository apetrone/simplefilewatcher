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

#ifdef _WIN32

#include <FileWatcher/FileWatcherWin32.h>

#define _WIN32_WINNT 0x0550
#include <windows.h>

#if defined(_MSC_VER)
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "ole32.lib")

// disable secure warnings
#pragma warning (disable: 4996)
#endif

namespace FW
{
	/// Internal watch data
	struct WatchStruct
	{
		OVERLAPPED mOverlapped;
		HANDLE mDirHandle;
		BYTE mBuffer[32 * 1024];
		LPARAM lParam;
		DWORD mNotifyFilter;
		bool mStopNow;
		FileWatcherImpl* mFileWatcher;
		FileWatchListener* mFileWatchListener;
		char* mDirName;
		WatchID mWatchid;
	};

#pragma region Internal Functions

	// forward decl
	bool RefreshWatch(WatchStruct* pWatch);

	/// Unpacks events and passes them to a user defined callback.
	void CALLBACK WatchCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
	{
		TCHAR szFile[MAX_PATH];
		PFILE_NOTIFY_INFORMATION pNotify;
		WatchStruct* pWatch = (WatchStruct*) lpOverlapped;
		size_t offset = 0;

		if (dwErrorCode == ERROR_SUCCESS)
		{
			do
			{
				pNotify = (PFILE_NOTIFY_INFORMATION) &pWatch->mBuffer[offset];
				offset += pNotify->NextEntryOffset;

#			if defined(UNICODE)
				{
					lstrcpynW(szFile, pNotify->FileName,
						min(MAX_PATH, pNotify->FileNameLength / sizeof(WCHAR) + 1));
				}
#			else
				{
					int count = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName,
						pNotify->FileNameLength / sizeof(WCHAR),
						szFile, MAX_PATH - 1, NULL, NULL);
					szFile[count] = TEXT('\0');
				}
#			endif

/*
				FileWatcherWin32* fwatcher = static_cast<FileWatcherWin32*>(pWatch-mFileWatcher);
				pWatch->mFileWatcher->handleAction(pWatch, szFile, pNotify->Action);

*/
				pWatch->mFileWatcher->handleAction(pWatch, szFile, pNotify->Action);

			} while (pNotify->NextEntryOffset != 0);
		}

		if (!pWatch->mStopNow)
		{
			RefreshWatch(pWatch);
		}
	}

	/// Refreshes the directory monitoring.
	bool RefreshWatch(WatchStruct* pWatch)
	{
		return ReadDirectoryChangesW(
			pWatch->mDirHandle, pWatch->mBuffer, sizeof(pWatch->mBuffer), FALSE,
			pWatch->mNotifyFilter, NULL, &pWatch->mOverlapped, WatchCallback) != 0;
	}

	/// Stops monitoring a directory.
	void DestroyWatch(WatchStruct* pWatch)
	{
		if (pWatch)
		{
			pWatch->mStopNow = TRUE;

			CancelIo(pWatch->mDirHandle);

			if (!HasOverlappedIoCompleted(&pWatch->mOverlapped))
			{
				SleepEx(5, TRUE);
			}

			CloseHandle(pWatch->mOverlapped.hEvent);
			CloseHandle(pWatch->mDirHandle);
			delete pWatch->mDirName;
			HeapFree(GetProcessHeap(), 0, pWatch);
		}
	}

	/// Starts monitoring a directory.
	WatchStruct* CreateWatch(LPCTSTR szDirectory, DWORD mNotifyFilter)
	{
		WatchStruct* pWatch;
		size_t ptrsize = sizeof(*pWatch);
		pWatch = static_cast<WatchStruct*>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ptrsize));

		pWatch->mDirHandle = CreateFile(szDirectory, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);

		if (pWatch->mDirHandle != INVALID_HANDLE_VALUE)
		{
			pWatch->mOverlapped.hEvent    = CreateEvent(NULL, TRUE, FALSE, NULL);
			pWatch->mNotifyFilter = mNotifyFilter;

			if (RefreshWatch(pWatch))
			{
				return pWatch;
			}
			else
			{
				CloseHandle(pWatch->mOverlapped.hEvent);
				CloseHandle(pWatch->mDirHandle);
			}
		}

		HeapFree(GetProcessHeap(), 0, pWatch);
		return NULL;
	}

#pragma endregion

	//--------
	FileWatcherWin32::FileWatcherWin32()
		: mLastWatchID(0)
	{
	}

	//--------
	FileWatcherWin32::~FileWatcherWin32()
	{
		WatchMap::iterator iter = mWatches.begin();
		WatchMap::iterator end = mWatches.end();
		for(; iter != end; ++iter)
		{
			DestroyWatch(iter->second);
		}
		mWatches.clear();
	}

	//--------
	WatchID FileWatcherWin32::addWatch(const String& directory, FileWatchListener* watcher)
	{
		WatchID watchid = ++mLastWatchID;

		WatchStruct* watch = CreateWatch(directory.c_str(),
			FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME);

		watch->mWatchid = watchid;
		watch->mFileWatcher = this;
		watch->mFileWatchListener = watcher;
		watch->mDirName = new char[directory.length()+1];
		strcpy(watch->mDirName, directory.c_str());

		mWatches.insert(std::make_pair(watchid, watch));

		return watchid;
	}

	//--------
	void FileWatcherWin32::removeWatch(const String& directory)
	{
		WatchMap::iterator iter = mWatches.begin();
		WatchMap::iterator end = mWatches.end();
		for(; iter != end; ++iter)
		{
			if(directory == iter->second->mDirName)
			{
				removeWatch(iter->first);
				return;
			}
		}
	}

	//--------
	void FileWatcherWin32::removeWatch(WatchID watchid)
	{
		WatchMap::iterator iter = mWatches.find(watchid);

		if(iter == mWatches.end())
			return;

		WatchStruct* watch = iter->second;
		mWatches.erase(iter);

		DestroyWatch(watch);
	}

	//--------
	void FileWatcherWin32::update()
	{
		MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);
	}

	//--------
	void FileWatcherWin32::handleAction(WatchStruct* watch, const String& filename, unsigned long action)
	{
		FileWatcher::Action fwAction;

		switch(action)
		{
		case FILE_ACTION_RENAMED_NEW_NAME:
		case FILE_ACTION_ADDED:
			fwAction = FileWatcher::ACTION_ADD;
			break;
		case FILE_ACTION_RENAMED_OLD_NAME:
		case FILE_ACTION_REMOVED:
			fwAction = FileWatcher::ACTION_DELETE;
			break;
		case FILE_ACTION_MODIFIED:
			fwAction = FileWatcher::ACTION_MODIFIED;
			break;
		};

		watch->mFileWatchListener->handleFileAction(watch->mWatchid, watch->mDirName, filename, fwAction);
	}

};//namespace FW

#endif//_WIN32
