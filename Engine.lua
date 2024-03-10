--- Main build script for the IshakEngine --

--local modulesDir = "../Modules/"

-- Main Project Configuration --

project "IshakEngine"
    kind "StaticLib"
    language "C++"
    location "Intermediate/Engine"

filter { "platforms:Linux" }
	system "linux"

filter { "platforms:Windows" }
	system "windows"


-- In the future I want to add per platform and config folder, but as it has to be added in the modules too, I willhave to think about it.
    targetdir "Binaries"
    objdir "Intermediate/Engine/%{cfg.buildcfg}/%{cfg.platform}"

    includedirs { "include" }

   files { "include/**.h", "Source/**.cpp" }



libdirs{
 "Binaries" }

    links { "Binaries/SDL2.lib", "Binaries/SDL2_image.lib", "Binaries/SDL2main.lib" }

    filter "system:linux"
        print("Applying settings for Linux.")
        buildoptions { "-j$(nproc)" }
        defines { "LINUX" }
        buildoptions {
            "-std=c++17", -- for GCC or Clang
            "-g" -- adding debug information for debugging on Linux
        }

    filter "system:windows"
        print("Applying settings for Windows.")
        defines { "WINDOWS" }
	cppdialect "C++17"
        buildoptions {
            "/std:c++17", -- for MSVC compiler
            "/DEBUG" -- adding debug information for debugging on Windows
        }

    filter { "system:windows", "action:vs*" }
        print("Applying settings for Visual Studio on Windows.")
        systemversion "latest"  -- Set the project's platform toolset to the latest available
        buildoptions { "/std:c++17" }  -- Set C++17 standard for compilation
        flags { "MultiProcessorCompile" } -- Enable multi-processor compilation

    filter { "toolset:gcc" }
        buildoptions { "-j$(nproc)" }

    -- CONFIGURATIONS --

    filter {"configurations:DebugTests"}
        defines { "DEBUG_ENGINE", "WITH_TESTS" }
        symbols "On"
	cppdialect "C++17"

    filter {"configurations:Debug", "system:windows"}
        defines { "DEBUG_ENGINE" }
        symbols "On"
	cppdialect "C++17"

    filter {"configurations:Debug", "system:linux"}
        defines { "DEBUG_ENGINE" }
        symbols "On"
	cppdialect "C++17"

    filter "configurations:Release"
        optimize "On"
	symbols "Off"
