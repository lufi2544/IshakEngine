project "Launch"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/%{cfg.buildcfg}"

local modulesDir = "../../Modules/"

	includedirs{
					 "Public",
					modulesDir.. "Core/Public",
					modulesDir .. "Ecs/Public",
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

files {"Public/**.h", "Private/**.cpp"}

links{"SDL", "SDL_image", "Core", "Engine", "Renderer", "Ecs"}


filter "configurations:Debug"
	defines{ "DEBUG_ENGINE", "LINUX" }
	symbols "On" 
