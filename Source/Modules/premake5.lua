-- Main build script for the IshakEngine --

local modulesDir = "../Modules/"


--Main Project Configuration--

workspace "IshakEngine"
	configurations { "Debug", "Release" }


project "IshakEngine"
    kind "ConsoleApp"
    language "C++"

    targetdir "../../Binaries/%{cfg.buildcfg}"
    objdir "../../Intermediate/%{cfg.buildcfg}"


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

    links { "Engine", "Core", "Renderer", "DTEST", "Ecs", "Launch", "IMGUI", "Platform", "Game", "SDL", "SDL_image" }

    filter "configurations:Debug"
        defines { "DEBUG_ENGINE", "LINUX" }
    	symbols "On"
		buildoptions{ "-g" }


    filter "configurations:Release"
		defines {"LINUX"}
    	optimize "On"
   
-- Include the Modules build scripts --

include(modulesDir .. "Core/Core.lua")
include(modulesDir .. "Renderer/Renderer.lua")
include(modulesDir .. "DTEST/DTEST.lua")
include(modulesDir .. "Ecs/Ecs.lua")
include(modulesDir .. "Engine/Engine.lua")
include(modulesDir .. "Game/Game.lua")
include(modulesDir .. "IMGUI/IMGUI.lua")
include(modulesDir .. "Launch/Launch.lua")
include(modulesDir .. "Platform/Platform.lua")


-- Include the Third Party scripts --

local thirdPartyDir = modulesDir .. "ThirdParty/"



include(thirdPartyDir .. "SolParser/Sol.lua")




