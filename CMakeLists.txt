# Top-level CMakeLists.txt for Jungle Doom
#
# This is a fork of Chocolate Doom, rebranded to "Jungle Doom"
# by Ericson Willians (ericsonwillians@protonmail.com) to integrate
# Python and other custom modifications. The original Chocolate Doom
# credits and license remain intact. Only minimal changes were made.

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

cmake_minimum_required(VERSION 3.7.2)

###############################################################################
# 1) Project Renamed from "Chocolate Doom" to "Jungle Doom"
###############################################################################
project("Jungle Doom" VERSION 3.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

# Autotools variables (these remain, but we rename chocolate-doom to jungle-doom):
set(top_srcdir ${CMAKE_CURRENT_SOURCE_DIR})
set(top_builddir ${CMAKE_CURRENT_BINARY_DIR})

# AC_INIT variables (renamed TARNAME to "jungle-doom"):
set(PACKAGE_NAME "${PROJECT_NAME}")
set(PACKAGE_TARNAME "jungle-doom")
set(PACKAGE_VERSION "${PROJECT_VERSION}")
set(PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")

# Optionally, you can replace the bug report address:
set(PACKAGE_BUGREPORT "chocolate-doom-dev-list@chocolate-doom.org")

# The below logic is mostly unchanged; it extracts a short name by removing " Doom"
string(REGEX REPLACE " Doom$" "" PACKAGE_SHORTNAME "${PACKAGE_NAME}")
set(PACKAGE_COPYRIGHT "Copyright (C) 1993-2024")
set(PACKAGE_LICENSE "GNU General Public License, version 2")

###############################################################################
# 2) Original compile settings, unchanged except for new name references.
###############################################################################
if(MSVC)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS" "/D_CRT_SECURE_NO_DEPRECATE"
                    "/D_CRT_NONSTDC_NO_DEPRECATE")
else()
    add_compile_options("-Wall" "-Wdeclaration-after-statement"
                        "-Wredundant-decls")
endif()

option(CMAKE_FIND_PACKAGE_PREFER_CONFIG
       "Lookup package config files before using find modules" On)

option(ENABLE_SDL2_NET "Enable SDL2_net" On)
option(ENABLE_SDL2_MIXER "Enable SDL2_mixer" On)

if(NOT DEFINED EMSCRIPTEN)
    find_package(SDL2 2.0.14)
endif()
if(ENABLE_SDL2_MIXER)
    if(NOT DEFINED EMSCRIPTEN)
        find_package(SDL2_mixer 2.0.2)
    endif()
else()
    add_compile_definitions(DISABLE_SDL2MIXER=1)
endif()

if(ENABLE_SDL2_NET)
    if(NOT DEFINED EMSCRIPTEN)
        find_package(SDL2_net 2.0.0)
    endif()
else()
    add_compile_definitions(DISABLE_SDL2NET=1)
endif()

# Check for libsamplerate.
find_package(SampleRate)
if(SampleRate_FOUND)
    set(HAVE_LIBSAMPLERATE TRUE)
endif()

# Check for libpng.
find_package(PNG)
if(PNG_FOUND)
    set(HAVE_LIBPNG TRUE)
endif()

# Check for FluidSynth.
find_package(FluidSynth 2.2.0)
if(FluidSynth_FOUND AND ENABLE_SDL2_MIXER)
    set(HAVE_FLUIDSYNTH TRUE)
endif()

find_package(m)

include(CheckSymbolExists)
include(CheckIncludeFile)
check_symbol_exists(strcasecmp "strings.h" HAVE_DECL_STRCASECMP)
check_symbol_exists(strncasecmp "strings.h" HAVE_DECL_STRNCASECMP)
check_include_file("dirent.h" HAVE_DIRENT_H)

# Example usage to build a resource-file version string:
string(CONCAT WINDOWS_RC_VERSION "${PROJECT_VERSION_MAJOR}, "
    "${PROJECT_VERSION_MINOR}, ${PROJECT_VERSION_PATCH}, 0")

# Without a hyphen. This is used for the bash-completion scripts.
string(TOLOWER "${PACKAGE_SHORTNAME}" PROGRAM_SPREFIX)

# With a hyphen, used almost everywhere else.
set(PROGRAM_PREFIX "${PROGRAM_SPREFIX}-")

# Generate config.h from config.h.cin
configure_file(cmake/config.h.cin config.h)

# Resource, manifest, and rc file generation
configure_file(src/doom-res.rc.in       src/doom-res.rc)
configure_file(src/heretic-res.rc.in    src/heretic-res.rc)
configure_file(src/hexen-res.rc.in      src/hexen-res.rc)
configure_file(src/setup-res.rc.in      src/setup-res.rc)
configure_file(src/setup/setup-manifest.xml.in src/setup/setup-manifest.xml)
configure_file(src/strife-res.rc.in     src/strife-res.rc)

###############################################################################
# 3) Subdirectories: same as original, referencing the new project name.
###############################################################################
foreach(SUBDIR textscreen opl pcsound src)
    add_subdirectory("${SUBDIR}")
endforeach()

# End of the top-level CMakeLists for Jungle Doom
