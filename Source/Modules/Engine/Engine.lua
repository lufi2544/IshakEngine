project "Engine"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/Engine/%{cfg.buildcfg}"
    objdir "../../../Intermediate/Engine"

files { "Public/**.h", "Private/**.cpp" }

local modulesDir = "../../Modules/"

	includedirs{
					 "Public",
					modulesDir .. "Core/Public", 
					modulesDir.. "Launch/Public",
					modulesDir .. "Ecs/Public",
				    modulesDir .. "Game/Public",
					modulesDir .. "Platform/Public",
					modulesDir .. "Renderer/Public",
					modulesDir .. "IMGUI/Public",
					modulesDir .. "ThirdParty/Lua/Public",
					modulesDir .. "ThirdParty/SDL/Public/include",
					modulesDir .. "ThirdParty/SDLIMAGE/Public",
					modulesDir .. "ThirdParty/SolParser/Public"
 				}



links{"Platform", "Core", "Renderer", "IMGUI", "Ecs"  }


filter "configurations:Debug"
	defines { "DEBUG_ENGINE", "LINUX" }
	symbols "On"
	buildoptions { "-g" }
