-- A solution contains projects, and defines the available configurations
solution "OgreDemo"
	location(_ACTION)
	targetdir "bin"
	configurations { "Debug", "Release" }

	-- A project defines one build target
	project "OgreDemo"
		kind "ConsoleApp"
		language "C++"
		files { 
			"../source/*.cpp",
			"OgreDemo.cpp"
		}
		includedirs { "../includes", "exampleapp", "$(OGRE_HOME)" }
		objdir "intermediate"
	
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		objdir "intermediate/Debug"
		targetname "OgreDemo_d"

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		objdir "intermediate/Release"
