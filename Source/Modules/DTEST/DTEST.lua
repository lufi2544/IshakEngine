project "DTEST"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/%{cfg.buildcfg}"

files { "Public/**.h", "Private/**.cpp" }	


local modulesDir = "../../Modules/"

	includedirs{
					 "Public",
					modulesDir .. "Core/Public",
					modulesDir .. "Ecs/Public"
 				}

links
{

	"Core", "Ecs"

}

filter "configurations:Debug"
	defines{ "DEBUG_ENGINE", "LINUX"  }
	symbols "On"

	buildoptions { "-g" }
