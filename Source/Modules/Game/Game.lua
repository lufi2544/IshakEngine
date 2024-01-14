project "Game"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/Engine/%{cfg.buildcfg}"
    objdir "../../../Intermediate/Engine"

files { "Public/**.h", "Private/**.cpp" }

local modulesDir = "../../Modules/"

	includedirs{
					 "Public",
					modulesDir.. "Core/Public",
					modulesDir .. "Ecs/Public",
					modulesDir .. "Platform/Public",
					modulesDir .. "Renderer/Public",
					modulesDir .. "Engine/Public",
					modulesDir .. "IMGUI/Public",
					modulesDir .. "ThirdParty/Lua/Public",
					modulesDir .. "ThirdParty/SDL/Public/include",
					modulesDir .. "ThirdParty/SDLIMAGE/Public",
					modulesDir .. "ThirdParty/SolParser/Public"
 				}

links{"Core", "Engine", "Ecs"}


filter "configurations:Debug"
	defines{ "LINUX"  }
	symbols "On"
	cppdialect "C++17"
	buildoptions { "-g" }

    filter { "system:linux" }
        defines { "LINUX" }

    filter { "system:windows" }
        defines { "WINDOWS" }
