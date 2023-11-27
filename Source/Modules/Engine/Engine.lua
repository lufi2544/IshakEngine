project "Engine"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/%{cfg.buildcfg}"

files { "Public/**.h", "Private/**.cpp" }

links{"Platform", "Core", "SDL", "SDLIMAGE", "Renderer", "DTEST", "IMGUI", "Ecs"  }
