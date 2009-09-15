Windows:

Builds in Visual Studio 2005/2008 (no UNICODE support yet, sorry)
After building the demo app you will need to copy over the Ogre3d and OIS
dlls (at least OgreMain_d.dll, RenderSystem_Direct3D9_d.dll, and OIS_d.dll).

During the demo run the various batch files in the media directory to change
the current texture. These changes are automatically caught by FileWatcher
and will update the texture in the demo.


Linux/OSX:

Should just be able to use the Makefile to build the demo.

