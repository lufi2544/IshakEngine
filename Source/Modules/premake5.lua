--- Main build script for the IshakEngine --

local modulesDir = "../Modules/"

-- Main Project Configuration --

workspace "IshakEngine"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    platforms { "Linux64", "Win64" }

project "IshakEngine"
    kind "ConsoleApp"
    language "C++"

filter { "platforms:Linux64" }
	system "linux"

filter { "platforms:Win64" }
	system "windows"


    targetdir "../../Binaries/Engine/%{cfg.buildcfg}"
    objdir "../../Intermediate/Engine/%{cfg.buildcfg}"

    includedirs {
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

    buildoptions {
        "-std=c++17",
        "-g" -- adding debug information for debugging
    }

    links { "Engine", "Core", "Renderer", "Ecs", "Launch", "IMGUI", "Platform", "Game" }

    filter "system:linux"
        print("Applying settings for Linux.")
        buildoptions { "-j$(nproc)" }
        defines { "LINUX" }

    filter "system:windows"
        print("Applying settings for Windows.")
        defines { "WINDOWS" }
	cppdialect "C++17"

    filter { "system:windows", "action:vs*" }
        print("Applying settings for Visual Studio on Windows.")
        systemversion "latest"  -- Set the project's platform toolset to the latest available
        buildoptions { "/std:c++17" }  -- Set C++17 standard for compilation
        flags { "MultiProcessorCompile" } -- Enable multi-processor compilation

    filter { "toolset:gcc" }
        buildoptions { "-j$(nproc)" }

    -- CONFIGURATIONS --

    filter "configurations:Debug"
        defines { "DEBUG_ENGINE" }
        symbols "On"
        buildoptions { "-g" }
	cppdialect "C++17"

    filter "configurations:Release"
        optimize "On"

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
