-- FileWatcher --

Description:

FileWatcher is a C++ wrapper for OS file monitoring systems. Currently
it uses Win32 ReadDirectoryChangesW for monitoring changes in Windows,
and inotify in linux. OSX is supported via kqueue and directory scans.


TODO:
Create a pure directory scan based fallback mode.


Windows:

Builds in Visual Studio 2005 (no UNICODE support yet, sorry)
After building the demo app you will need to copy over the Ogre3d and OIS
dlls (at least OgreMain_d.dll, RenderSystem_Direct3D9_d.dll, and OIS_d.dll).

During the demo run the various batch files in the media directory to change
the current texture. These changes are automatically caught by FileWatcher
and will update the texture in the demo.


Linux/OSX:

Should just be able to use the Makefile to build the demo. Sorry, but I
haven't had time to port the Ogre demo, though it should port easily enough.


------------------------------
Written by James Wynn
Contact: james@jameswynn.com

The most recent version can be located at:
http://simplefilewatcher.googlecode.com
