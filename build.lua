-- A solution contains projects, and defines the available configurations
solution "SimpleFileWatcher"
	location(_ACTION)
	configurations { "Debug", "Release" }

	-- A project defines one build target
	project "SimpleDemo"
		kind "ConsoleApp"
		location(_ACTION)
		language "C++"
		files { 
			"source/*.cpp",
			"SimpleDemo.cpp"
		}
		includedirs { "includes" }
		objdir "intermediate/SimpleDemo"

	project "OgreDemo"
		kind "ConsoleApp"
		location(_ACTION)
		language "C++"
		files {
			"source/*.cpp",
			"OgreDemo.cpp"
		}
		includedirs {
			"includes",
			"exampleapp"
		}
	
	configuration {"SimpleDemo", "Release"}
		objdir "intermediate/SimpleDemo/Release"
	
	configuration {"SimpleDemo", "Debug"}
		objdir "intermediate/SimpleDemo/Debug"
	
	configuration {"OgreDemo", "Release"}
		objdir "intermediate/OgreDemo/Release"
	
	configuration {"OgreDemo", "Debug"}
		objdir "intermediate/OgreDemo/Debug"
	
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
