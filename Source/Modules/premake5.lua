-- Main build script for the IshakEngine --

local modulesDir = "../Modules/"


--Main Project Configuration--

workspace "IshakEngine"
	configurations { "Debug", "Release" }
	architecture "x86_64"


project "IshakEngine"
    kind "ConsoleApp"
    language "C++"

    targetdir "../../Binaries/Engine/%{cfg.buildcfg}"
    objdir "../../Intermediate/Engine/%{cfg.buildcfg}"


 --   files{ "Launch/Public/**.h", "Launch/Private/**.cpp" }
	includedirs{
					"Launch/Public",
					"Core/Public", 
					"Ecs/Public",
					"Game/Public",
					"Platform/Public",
					"Renderer/Public",
					"Engine/Public",
					"IMGUI/Public",
					"ThirdParty/Lua/Public",
					"ThirdParty/SDL/Public",
					"ThirdParty/SDLIMAGE/Public",
					"ThirdParty/SolParser/Public"
 				}

    buildoptions 
	{
		"-std=c++17",
		"-g" -- adding debbug table for debbuging
	}

    links { "Engine", "Core", "Renderer", "Ecs", "Launch", "IMGUI", "Platform", "Game"}

	filter "system:linux"
		print("Applying parallel compilation for Linux.")
		buildoptions { "-j$(nproc)" }
		defines{ "LINUX" }

	filter { "system:windows" }		
		print("Applying parallel compilation for Windows.")
		defines{"WINDOWS"}


	filter { "toolset:gcc" }
		buildoptions {"-j$(nproc)"}

-- CONFIGURATIONS --

    filter "configurations:Debug"
        defines { "DEBUG_ENGINE"}
    	symbols "On"
		buildoptions{ "-g" }

    filter "configurations:Release"	
    	optimize "On"


-- OS Filter --

-- Include the Modules build scripts --

include(modulesDir .. "Core/Core.lua")
include(modulesDir .. "Renderer/Renderer.lua")
include(modulesDir .. "Ecs/Ecs.lua")
include(modulesDir .. "Engine/Engine.lua")
include(modulesDir .. "Game/Game.lua")
include(modulesDir .. "IMGUI/IMGUI.lua")
include(modulesDir .. "Launch/Launch.lua")
include(modulesDir .. "Platform/Platform.lua")


-- Include the Third Party scripts --

local thirdPartyDir = modulesDir .. "ThirdParty/"
include(thirdPartyDir .. "SolParser/Sol.lua")
