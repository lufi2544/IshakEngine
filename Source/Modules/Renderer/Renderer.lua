project "Renderer"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/%{cfg.buildcfg}"

files { "Public/**.h", "Private/**.cpp" }

local modulesDir = "../../Modules/"

	includedirs{
					"Public",
					modulesDir .. "Core/Public",
					modulesDir.. "Launch/Public",
					modulesDir .. "Ecs/Public",
				    modulesDir .. "Game/Public",
					modulesDir .. "Platform/Public",				
					modulesDir .. "Engine/Public",
					modulesDir .. "IMGUI/Public",
					modulesDir .. "ThirdParty/Lua/Public",
					modulesDir .. "ThirdParty/SDL/Public/include",
					modulesDir .. "ThirdParty/SDLIMAGE/Public",
					modulesDir .. "ThirdParty/SolParser/Public"
 				}

links{"Core", "Platform", "IMGUI", "Ecs","SDL2_image" }


filter "configurations:Debug"
	defines { "DEBUG_ENGINE", "LINUX" }
	symbols "On"

