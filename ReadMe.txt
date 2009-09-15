-- FileWatcher --

Description:

FileWatcher is a C++ wrapper for OS file monitoring systems. Currently
it uses Win32 ReadDirectoryChangesW for monitoring changes in Windows,
and inotify in linux. OSX is supported via kqueue and directory scans.


TODO:
Create a pure directory scan based fallback mode.

------------------------------
Written by James Wynn
Contact: james@jameswynn.com

The most recent version can be located at:
http://simplefilewatcher.googlecode.com
