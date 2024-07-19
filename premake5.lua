workspace "RocketGameEngine"
    configurations { "Debug", "Release" }

project "RocketEngine"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    objdir "obj/%{cfg.buildcfg}"
    targetdir "obj/%{cfg.buildcfg}"

files {
    "src/**.cpp"
}

includedirs {
    "include"
}

buildoptions {
    "-Wall"
}

filter "configurations:Debug"
    symbols "On"
    defines { "ROCKET_DEBUG" }

filter "configurations:Release"
    optimize "On"