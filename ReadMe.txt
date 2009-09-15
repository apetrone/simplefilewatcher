-- FileWatcher --

Description:

FileWatcher is a C++ wrapper for OS file monitoring systems. Currently
it uses Win32 ReadDirectoryChangesW for monitoring changes in Windows,
and inotify in linux. OSX is supported via kqueue and directory scans.


TODO:

 * Create a pure directory scan based fallback mode.
 * Optimize the kqueue implementation.
 * Thorough UnitTest
 * Add proper Unicode support.


Compiling:

Build scripts for Visual Studio 2005, Visual Studio 2008, and GNU Make
can be found within the build directory, or can be regenerated using
premake 4.0.


Integration with Ogre3D:

Check the OgreDemo directory for an example integration with Ogre.


Caveats:

When some programs write data in Win32, they will generate both an Add,
and a Modify event. This is likely because the program is actually using
two separate calls to write its data.


------------------------------
Written by James Wynn
Contact: james@jameswynn.com

The most recent version can be located at:
http://simplefilewatcher.googlecode.com
