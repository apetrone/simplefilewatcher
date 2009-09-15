-- A solution contains projects, and defines the available configurations
solution "OgreDemo"
	location(_ACTION)
	targetdir "bin"
	configurations { "Debug", "Release" }

	-- A project defines one build target
	project "OgreDemo"
		kind "WindowedApp"
		language "C++"
		files { 
			"../source/*.cpp",
			"OgreDemo.cpp"
		}
		includedirs { "include", "../include", "exampleapp" }
		objdir "intermediate"
		libdirs { "lib" }
	
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		objdir "intermediate/Debug"
		targetname "OgreDemo_d"
		links { "OgreMain_d", "OIS_d" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		objdir "intermediate/Release"
		links { "OgreMain", "OIS" }
