set_defaultmode("debug")
add_rules("mode.debug")
add_rules("mode.releasedbg")
add_rules("mode.release")

add_requires("eigen")

set_toolchains("llvm")
set_runtimes("c++_shared")
set_policy("build.warning", true)
set_warnings("allextra", "pedantic")
add_cxxflags("-fexperimental-library")
add_rules("plugin.compile_commands.autoupdate", { outputdir = "build" })

local use_webots = true;

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

target("src")
    set_kind("static")
    set_languages("cxx26")
    add_files("src/**.mpp", { public = true })
    add_packages("eigen")
    if use_webots then
        add_defines("USE_WEBOTS", { public = true })
        add_includedirs("$(env WEBOTS_HOME)/include/controller/c", { public = true })
        add_linkdirs("$(env WEBOTS_HOME)/lib/controller")
        add_links("Controller")
    else
        remove_files("src/**.webots.mpp");
    end

target("test")
    set_kind("binary")
    set_languages("cxx26")
    add_deps("src")
    add_files("app/test/*.cc")
    -- TODO rt
    -- after_build(function (target)
    --     import("privilege.sudo")
    --     sudo.run("setcap CAP_SYS_NICE+eip %s", target:targetfile())
    -- end)

