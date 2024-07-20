workspace "RocketGameEngine"
    configurations { "Debug", "Release" }

project "UnitTests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    objdir "obj/%{cfg.buildcfg}"
    targetdir "build/%{cfg.buildcfg}"

files {
    "tests/**.cpp",
    "src/**.cpp"
}

includedirs {
    "include"
}

removefiles {
    "src/main.cpp"
}

defines {
    "BOOST_TEST_DYN_LINK"
}

links {
    "boost_unit_test_framework"
}

buildoptions {
    "`pkg-config spdlog --cflags`"
}

linkoptions {
    "`pkg-config spdlog --libs`"
}

postbuildcommands {
    "build/%{cfg.buildcfg}/UnitTests --show_progress=true --report_level=detailed --report_memory_leaks_to=memory_leaks.txt"
}

-----------------

project "RocketEngine"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    objdir "obj/%{cfg.buildcfg}"
    targetdir "build/%{cfg.buildcfg}"

files {
    "src/**.cpp"
}

includedirs {
    "include"
}

buildoptions {
    "-Wall", "`pkg-config spdlog --cflags`"
}

linkoptions {
    "`pkg-config spdlog --libs`"
}

filter "configurations:Debug"
    symbols "On"
    defines { "ROCKET_DEBUG" }

filter "configurations:Release"
    optimize "On"