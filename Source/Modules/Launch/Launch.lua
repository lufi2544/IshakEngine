project "Launch"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/Engine/%{cfg.buildcfg}"
    objdir "../../../Intermediate/Engine"

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
					modulesDir .. "ThirdParty/SDL/Public/include",
					modulesDir .. "ThirdParty/SDLIMAGE/Public",
					modulesDir .. "ThirdParty/SolParser/Public"
 				}

files {"Public/**.h", "Private/**.cpp"}

links{"SDL2", "SDL2_image", "Core", "Engine", "Renderer", "Ecs"}


filter "configurations:Debug"
	--defines{ "DEBUG_ENGINE"}
	cppdialect "C++17"
	symbols "On" 
	buildoptions { "-g"}

    filter { "system:linux" }
        defines { "LINUX" }

    filter { "system:windows" }
        defines { "WINDOWS" }
