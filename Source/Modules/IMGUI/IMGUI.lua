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

links {"SDL2", "SDL2_image"}

filter"configurations:Debug" 
	defines{ "LINUX" }
	symbols "On" 
	buildoptions { "-g" }
