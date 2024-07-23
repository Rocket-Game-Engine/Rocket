workspace "RocketGameEngine"
    configurations { "Debug", "Release" }

project "UnitTests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    objdir "obj/UnitTests/%{cfg.buildcfg}"
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
    "BOOST_TEST_DYN_LINK", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE"
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
    "build/%{cfg.buildcfg}/UnitTests --report_level=detailed --log_format=JUNIT --log_sink=\"report-test-data.xml\""
}

-----------------

project "RocketEngine"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    objdir "obj/Engine/%{cfg.buildcfg}"
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
    defines { "ROCKET_DEBUG", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO" }

filter "configurations:Release"
    optimize "On"
    defines { "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO"}