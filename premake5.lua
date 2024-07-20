workspace "RocketGameEngine"
    configurations { "Debug", "Release" }

project "UnitTests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    objdir "obj/%{cfg.buildcfg}"
    targetdir "bin/%{cfg.buildcfg}"

files {
    "tests/**.cpp"
}

defines {
    "BOOST_TEST_DYN_LINK"
}

links {
    "boost_unit_test_framework"
}

-----------------

project "RocketEngine"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    objdir "obj/%{cfg.buildcfg}"
    targetdir "bin/%{cfg.buildcfg}"

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