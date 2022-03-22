workspace "mem_guard"
    configurations { "Debug", "Release" }

    targetdir "."
    files { "*.h", "*.c" }
    includedirs { 
        ".", 
        "/usr/include/luajit-2.1" 
    }
    links { "luajit-5.1" }


    project "mem_guard_test"
        kind "ConsoleApp"
        language "C"

    project "mem_guard"
        kind "StaticLib"
        language "C"

        --targetdir "bin/%{cfg.buildcfg}"
        --buildoptions { "-fPIC" }
        --libdirs { "/usr/lib" }
        --links { "lua5.1" }

    --filter "configurations:Debug"
    --defines { "DEBUG" }
    --symbols "On"

    --filter "configurations:Release"
    --defines { "NDEBUG" }
    --optimize "On"
