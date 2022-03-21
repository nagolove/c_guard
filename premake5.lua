workspace "mem_guard"
    configurations { "Debug", "Release" }

    project "mem_guard"
        kind "ConsoleApp"
        language "C"
        targetdir "bin/%{cfg.buildcfg}"

        files { "src/**.h", "src/**.c" }
        includedirs { 
            "src", 
            --"/usr/include/luajit-2.1" 
            "/usr/include/lua5.1"
        }
        buildoptions { "-fPIC" }
        --links { "luajit-5.1" }
        libdirs { "/usr/lib" }
        --links { "lua5.1" }

    --filter "configurations:Debug"
    --defines { "DEBUG" }
    --symbols "On"

    --filter "configurations:Release"
    --defines { "NDEBUG" }
    --optimize "On"
