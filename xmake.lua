set_defaultmode("debug")
set_policy("run.autobuild", true)

add_rules("mode.debug")
add_rules("mode.releasedbg")
add_rules("mode.release")
add_rules("plugin.compile_commands.autoupdate", { outputdir = "build" })

add_requires("eigen")

set_toolchains("llvm")
set_runtimes("c++_shared")
set_languages("cxx23")
set_policy("build.warning", true)
set_warnings("allextra", "pedantic")
add_cxxflags("-fexperimental-library")
add_cxxflags("-stdlib=libc++")
add_cxxflags("")
    
if is_mode("debug") then
    add_cxxflags("-ftrapv")
    add_cxxflags("-Wnull-dereference")
    add_cxxflags("-Wformat-security")
    add_cxxflags("-Wimplicit-fallthrough")
    add_cxxflags("-Wsign-conversion")
    add_cxxflags("-Wconversion")
    add_cxxflags("-Wfloat-equal")
    add_cxxflags("-Wcast-qual")
    add_cxxflags("-Wcast-align")
    add_cxxflags("-Wstrict-aliasing=2")
    add_cxxflags("-Wundef")
    add_cxxflags("-Wmissing-include-dirs")
    add_cxxflags("-Winvalid-pch")
    add_cxxflags("-Wstack-protector")
    add_cxxflags("-Wpacked")
    add_cxxflags("-Wmissing-noreturn")
    add_cxxflags("-Wshadow")
    add_cxxflags("-Wswitch-enum")
    add_cxxflags("-Wunused")
    add_cxxflags("-Wunused-parameter")
    add_cxxflags("-Wmissing-field-initializers")
    add_cxxflags("-Wformat=2")
    add_cxxflags("-Wformat-nonliteral")
    add_cxxflags("-Wmissing-format-attribute")
    add_cxxflags("-Wdisabled-optimization")
    add_cxxflags("-Wstrict-overflow=5")
    add_cxxflags("-Wdouble-promotion")
    add_cxxflags("-Wmissing-declarations")
    add_cxxflags("-Wwrite-strings")
    add_cxxflags("-Wno-pre-c++23-compat")
end

option("use_webots") do
    add_linkdirs("$(env WEBOTS_HOME)/lib/controller")
    add_links("Controller")
end

target("src") do
    set_kind("static")
    add_files("src/**.mpp|**.webots.mpp", { public = true })
    add_packages("eigen")
    add_options("use_webots")
    if has_config("use_webots") then
        add_defines("USE_WEBOTS", { public = true })
        add_files("src/**.webots.mpp", { public = true })
        add_includedirs("$(env WEBOTS_HOME)/include/controller/c", { public = true })
    end
end

if has_config("use_webots") then
    target("wheel_leg.webots") do
        set_default(false)
        set_kind("binary")
        add_deps("src")
        add_files("app/wheel_leg/webots.cc")
    end
end

