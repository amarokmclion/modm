#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright (c) 2018, Niklas Hauser
#
# This file is part of the modm project.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
# -----------------------------------------------------------------------------

import os
from collections import defaultdict

def common_source_files(env, buildlog):
    files_to_build = []

    prev = env.outbasepath
    env.outbasepath = "."
    outpath = env.outpath("modm")
    for operations in buildlog:
        filename = os.path.relpath(operations.filename_out, outpath)
        _, extension = os.path.splitext(filename)

        if extension in [".c", ".cpp", ".cc", ".sx", ".S"]:
            files_to_build.append(os.path.normpath(filename).replace('\\','/'))

    env.outbasepath = prev
    return sorted(files_to_build)

def common_target(target):
    core = target.get_driver("core")["type"]
    core = core.replace("fd", "").replace("f", "")
    mcu = target._properties.get("mcu", "")
    p = {
        "core": core,
        "mcu": mcu,
        "platform": target.identifier["platform"],
        "family": target.identifier["family"],
        "partname": target.partname,
    }
    return p

def common_memories(target):
    core_driver = target.get_driver("core")
    memories = []
    if "memory" in core_driver:
        memories.extend([
            {
                k:(int(v) if v.isdigit() else (int(v, 16) if v.startswith("0x") else v))
                for k, v in memory.items()
            }
            for memory in core_driver["memory"]
        ])
    return memories


def common_compiler_flags(compiler, target, buildlog):
    flags = defaultdict(lambda: defaultdict(list))
    # flags for C **and** C++
    flags["CCFLAGS"]["base"] = [
        "-W",
        "-Wall",
        "-Wduplicated-cond",
        "-Werror=format",
        "-Werror=maybe-uninitialized",
        "-Werror=overflow",
        "-Werror=sign-compare",
        "-Wextra",
        "-Wlogical-op",
        "-Wpointer-arith",
        "-Wundef",
        # "-Wcast-align",
        # "-Wcast-qual",
        # "-Wmissing-declarations",
        # "-Wredundant-decls",
        # "-Wshadow",

        "-fdata-sections",
        "-ffunction-sections",
        "-fshort-wchar",
        "-funsigned-char",
        # "-fmerge-all-constants",

        "-g3",
        "-gdwarf",
    ]
    if compiler.startswith("gcc"):
        flags["CCFLAGS"]["base"] += [
            "-finline-limit=10000",
            "-funsigned-bitfields",
        ]
    flags["CCFLAGS"]["release"] = [
        "-Os",
    ]
    # not a valid profile
    # flags["CCFLAGS"]["fast"] = [
    #     "-O3",
    # ]
    flags["CCFLAGS"]["debug"] = [
        "-Og",
        "-fno-split-wide-types",
        "-fno-tree-loop-optimize",
        "-fno-move-loop-invariants",
    ]
    # flags only for C
    flags["CFLAGS"]["base"] = [
        "-Wimplicit",
        "-Wnested-externs",
        "-Wredundant-decls",
        "-Wstrict-prototypes",
        "-Wbad-function-cast",
        "-std=gnu11",
        # "-pedantic",
    ]
    # flags only for C++
    flags["CXXFLAGS"]["base"] = [
        "-Woverloaded-virtual",
        # "-Wctor-dtor-privacy",
        # "-Wnon-virtual-dtor",
        # "-Wold-style-cast",
        "-fstrict-enums",
        "-std=c++17",
        # "-pedantic",
    ]
    # flags only for Assembly
    flags["ASFLAGS"]["base"] = [
        "-g3",
        "-gdwarf",
        # "-xassembler-with-cpp",
    ]
    # flags for the linker
    if target.identifier["family"] != "darwin":
        flags["LINKFLAGS"]["base"] = [
            "-Wl,--fatal-warnings",
            "-Wl,--gc-sections",
            "-Wl,--relax",
            "-Wl,-Map,{target_base}.map,--cref",
        ]
    # C Preprocessor defines
    flags["CPPDEFINES"]["base"] = \
        buildlog.metadata.get("cpp.define", [])
    flags["CPPDEFINES"]["release"] = \
        buildlog.metadata.get("cpp.define.release", [])
    flags["CPPDEFINES"]["debug"] = \
        buildlog.metadata.get("cpp.define.debug", []) + [
        "MODM_DEBUG_BUILD",
    ]
    # Architecture flags for C, C++, Assembly and **Linker**
    flags["ARCHFLAGS"]["base"] = []

    # Target specific flags
    core = target.get_driver("core")["type"]
    if core.startswith("cortex-m"):
        cpu = core.replace("fd", "").replace("f", "")
        flags["ARCHFLAGS"]["base"] += [
            "-mcpu={}".format(cpu),
            "-mthumb",
        ]
        single_precision = True
        fpu = core.replace("cortex-m", "").replace("+", "")
        if "f" in fpu:
            fpu_spec = {
                "4f": "-mfpu=fpv4-sp-d16",
                "7f": "-mfpu=fpv5-sp-d16",
                "7fd": "-mfpu=fpv5-d16",
            }[fpu]
            flags["ARCHFLAGS"]["base"] += [
                "-mfloat-abi=hard",
                fpu_spec
            ]
            single_precision = ("-sp-" in fpu_spec)
        if single_precision:
            flags["CCFLAGS"]["base"] += [
                "-fsingle-precision-constant",
                "-Wdouble-promotion",
            ]
        flags["CXXFLAGS"]["base"] += [
            "-fno-exceptions",
            "-fno-unwind-tables",
            "-fno-rtti",
            "-fno-threadsafe-statics",
            "-fuse-cxa-atexit",
        ]
        flags["LINKFLAGS"]["base"] += [
            "-Wl,--no-wchar-size-warning",
            "-Wl,-wrap,_calloc_r",
            "-Wl,-wrap,_free_r",
            "-Wl,-wrap,_malloc_r",
            "-Wl,-wrap,_realloc_r",
            "--specs=nano.specs",
            "--specs=nosys.specs",
            "-nostartfiles",
            "-L{linkdir}",
            "-Tlinkerscript.ld",
        ]

    elif core.startswith("avr"):
        flags["ARCHFLAGS"]["base"] += [
            "-mmcu={}".format(target.partname),
        ]
        flags["CXXFLAGS"]["base"] += [
            "-fno-exceptions",
            "-fno-unwind-tables",
            "-fno-rtti",
            "-fno-threadsafe-statics",
        ]
        flags["LINKFLAGS"]["base"] += [
            "-L{linkdir}",
            "-Tlinkerscript.ld",
        ]

    return flags

