IncludeDir=./includes
ObjDir=./intermediate
OutputDir=./bin
SourceDir=./source
cc=g++
cflags=-O3 -g -I$(IncludeDir)
ldflags=-g
PROGRAM=$(OutputDir)/FileWatcherDemo

FileWatcherDemo : $(ObjDir) $(ObjDir)/FileWatcher.o $(ObjDir)/FileWatcherLinux.o $(ObjDir)/SimpleDemo.o $(ObjDir)/FileWatcherOSX.o $(ObjDir)/FileWatcherWin32.o
	$(cc) $(cflags) -o $(PROGRAM) $(ObjDir)/SimpleDemo.o $(ObjDir)/FileWatcher.o $(ObjDir)/FileWatcherLinux.o $(ObjDir)/FileWatcherOSX.o $(ObjDir)/FileWatcherWin32.o

$(ObjDir) :
	mkdir $(ObjDir)

$(ObjDir)/FileWatcher.o : $(SourceDir)/FileWatcher.cpp
	$(cc) $(cflags) -o $(ObjDir)/FileWatcher.o -c $(SourceDir)/FileWatcher.cpp

$(ObjDir)/FileWatcherLinux.o : $(SourceDir)/FileWatcherLinux.cpp
	$(cc) $(cflags) -o $(ObjDir)/FileWatcherLinux.o -c $(SourceDir)/FileWatcherLinux.cpp

$(ObjDir)/FileWatcherOSX.o : $(SourceDir)/FileWatcherOSX.cpp
	$(cc) $(cflags) -o $(ObjDir)/FileWatcherOSX.o -c $(SourceDir)/FileWatcherOSX.cpp

$(ObjDir)/FileWatcherWin32.o : $(SourceDir)/FileWatcherWin32.cpp
	$(cc) $(cflags) -o $(ObjDir)/FileWatcherWin32.o -c $(SourceDir)/FileWatcherWin32.cpp

$(ObjDir)/SimpleDemo.o : SimpleDemo.cpp
	$(cc) $(cflags) -o $(ObjDir)/SimpleDemo.o -c SimpleDemo.cpp

clean :
	$(RM) -rf $(ObjDir) $(PROGRAM)


