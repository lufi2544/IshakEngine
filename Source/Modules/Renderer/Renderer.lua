project "Renderer"
	kind "SharedLib"
	language "C++"
	targetdir "../../../Binaries/%{cfg.buildcfg}"

files { "Public/**.h", "Private/**.cpp" }

links{"Core", "SDL", "SDLIMAGE", "Platform", "IMGUI", "Ecs"}
