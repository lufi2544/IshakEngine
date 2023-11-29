-- This is the File that will build the project Test Enviroment


local modulesDir = "../Source/Modules/"


workspace "IshakEngineTests"
	configurations { "Debug" }



project "IshakEngineTests"
	kind "ConsoleApp"
	language "C++"


	targetdir "../Binaries/Tests/%{cfg.buildcfg}"
	objdir "../Intermediate/Tests/%{cfg.buildcfg}"

	files {"dtest/**.h", "Tests/**.cpp"}


	includedirs
	{
		"dtest",
		modulesDir .. "Core/Public", 
		modulesDir .. "Ecs/Public",
		modulesDir .. "IMGUI/Public"
 	}

	buildoptions 
	{
		"-std=c++17",
		"-g"
	}

	links { "Core", "Ecs", "IMGUI" }

	filter "system:linux"
		print("Defining TESTS for LINUX")
		defines{ "LINUX" }

	filter "system:windows"
		print("Defining TESTS for WINDOWS")
		defines{ "WINDOWS" }

	filter "configurations:Debug"
		defines { "DEBUG_ENGINE"}
		symbols "On"
		buildoptions {"-g"} -- includes the symbols in the executable for debugging

	include(modulesDir .. "Core/Core.lua")
	include(modulesDir .. "Ecs/Ecs.lua")
	include(modulesDir .. "IMGUI/IMGUI.lua")
