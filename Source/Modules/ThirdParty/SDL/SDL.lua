project "SDL"
	kind "None"	
	language "C++"


files { "Public/*.h" }

filter "system:windows"
	files 
	{
		"../../../../Binaries/SDL2.dll",
		"../../../../Binaries/SDL2.lib"
	}
