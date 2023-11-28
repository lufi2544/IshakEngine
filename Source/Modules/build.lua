-- Main build script for the IshakEngine --

local modulesDir = "../Modules/"


--Main Project Configuration--

workspace "IshakEngine"
	configurations { "Debug", "Release" }


project "IshakEngine"
    kind "ConsoleApp"
    language "C++"

    targetdir "../../Binaries/%{cfg.buildcfg}"
    obdir "../../Intermediate/%{cfg.buildcfg}"


    files{ "Public/**.h", "Private/**.cpp" }

    buildoptions { "-std=c++17" }
    links { "SDL2", "SDL2_image", "lua5.3" }

    filter "configurations:Debug"
        defines { "DEBUG_ENGINE" }
    	symbols "On"


    filter "configurations:Release"
    	optimize "On"
   
-- Include the Modules build scripts --

include(modulesDir .. "Core/Core.lua")
include(modulesDir .. "DTEST/DTEST.lua")
include(modulesDir .. "ECS/Ecs.lua")
include(modulesDir .. "Engine/Engine.lua")
include(modulesDir .. "Game/Game.lua")
include(modulesDir .. "IMGUI/IMGUI.lua")
include(modulesDir .. "Launch/Launch.lua")
include(modulesDir .. "Platform/Platform.lua")
include(modulesDir .. "Renderer/Renderer.lua")

-- Include the Third Party scripts --

local thirdPartyDir = modulesDir .. "ThirdParty/"

include(thirdPartyDir .. "SDL/SDL.lua")
include(thirdPartyDir .. "SDLIMAGE/SDLIMAGE.lua")
include(thirdPartyDir .. "SolParser/Sol.lua")



