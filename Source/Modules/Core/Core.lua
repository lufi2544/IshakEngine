project "Core"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/Engine/%{cfg.buildcfg}"
    objdir "../../../Intermediate/Engine"

files { "Public/**.h", "Private/**.cpp" } -- Include all the files under the Public and the Private Dir

local modulesDir = "../../Modules/"

	includedirs{
					 "Public",
					modulesDir.. "Launch/Public",
					modulesDir .. "ECS/Public",
				    modulesDir .. "Game/Public",
					modulesDir .. "Platform/Public",
					modulesDir .. "Renderer/Public",
					modulesDir .. "Engine/Public",
					modulesDir .. "IMGUI/Public",
					modulesDir .. "ThirdParty/Lua/Public",
					modulesDir .. "ThirdParty/SDL/Public",
					modulesDir .. "ThirdParty/SDLIMAGE/Public",
					modulesDir .. "ThirdParty/SolParser/Public"
 				}



    filter "configurations:Debug"
        defines { "DEBUG_ENGINE" }
    	symbols "On"
	cppdialect "C++17"

       filter { "system:linux" }
            buildoptions { "-g", "-std=c++17" }

        filter { "system:windows" }
            defines { "WINDOWS" }
            buildoptions { "/std:c++17" } 

