project "Platform"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/%{cfg.buildcfg}"

files { "Public/**.h", "Private/**.cpp" }

local modulesDir = "../../Modules/"

	includedirs{
					 "Public",
					modulesDir.. "Launch/Public",
					modulesDir.. "Core/Public",
					modulesDir .. "ECS/Public",
				    modulesDir .. "Game/Public",
					modulesDir .. "Renderer/Public",
					modulesDir .. "Engine/Public",
					modulesDir .. "IMGUI/Public",
					modulesDir .. "ThirdParty/Lua/Public",
					modulesDir .. "ThirdParty/SDL/Public/include",
					modulesDir .. "ThirdParty/SDLIMAGE/Public",
					modulesDir .. "ThirdParty/SolParser/Public"
 				}



links{"SDL2", "SDL2_image", "Core"  }

    filter "configurations:Debug"
        defines { "DEBUG_ENGINE", "LINUX" }
    	symbols "On"




