project "IMGUI"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/%{cfg.buildcfg}"

files {"Public/**.h", "Private/**.cpp"}

local moduesDir = "../../Modules/"

includedirs{ 
	"Public",
	moduesDir .. "ThirdParty/SDL/Public/include"
}

links {"SDL"}

filter"configurations:Debug" 
	defines{ "LINUX" }
	symbols "On" 
