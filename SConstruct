#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["cpp/"])
env.Append(CXXFLAGS=["-std=c++20"])
sources = Glob("cpp/*.cpp")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "bin/godsvg.{}.{}.framework/godsvg.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "bin/godsvg.{}.{}.simulator.a".format(
                env["platform"], env["target"]
            ),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "bin/godsvg.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "bin/godsvg{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
