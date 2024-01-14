project "SDLIMAGE"
	kind "None"
	language "C++"


files { "Public/SDL/*.h" }

filter "system:windows"
	files 
	{
		"../../../../Binaries/SDL2_image.dll"
	}
