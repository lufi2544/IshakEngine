project "IMGUI"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/Engine/%{cfg.buildcfg}"
    objdir "../../../Intermediate/Engine"

files {"Public/**.h", "Private/**.cpp"}

local moduesDir = "../../Modules/"

includedirs{ 
	"Public",
	moduesDir .. "ThirdParty/SDL/Public/include"
}

links {"SDL", "SDL2_image"}

filter"configurations:Debug" 
	defines{ "DEBUG_ENGINE" }
	cppdialect "C++17"
	symbols "On" 
	buildoptions { "-g" }

    filter { "system:linux" }
        defines { "LINUX" }

    filter { "system:windows" }
        defines { "WINDOWS" }
