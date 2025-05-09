###############################################################################
# Minimal Python integration into the *original* Chocolate Doom src/CMakeLists.txt
###############################################################################

## PYTHON ADDITION (begin)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
include_directories(${Python3_INCLUDE_DIRS})
## PYTHON ADDITION (end)

foreach(SUBDIR doom heretic hexen strife setup)
    add_subdirectory("${SUBDIR}")
endforeach()

# Common source files used by absolutely everything:
set(COMMON_SOURCE_FILES
    i_main.c
    i_system.c           i_system.h
    m_argv.c             m_argv.h
    m_misc.c             m_misc.h)

# Dedicated server (chocolate-server):
set(DEDSERV_FILES
    d_dedicated.c
    d_iwad.c            d_iwad.h
    d_mode.c            d_mode.h
    deh_str.c           deh_str.h
    i_timer.c           i_timer.h
    m_config.c          m_config.h
    net_common.c        net_common.h
    net_dedicated.c     net_dedicated.h
    net_io.c            net_io.h
    net_packet.c        net_packet.h
    net_sdl.c           net_sdl.h
    net_query.c         net_query.h
    net_server.c        net_server.h
    net_structrw.c      net_structrw.h
    z_native.c          z_zone.h)

add_executable("${PROGRAM_PREFIX}server" WIN32 ${COMMON_SOURCE_FILES} ${DEDSERV_FILES})
target_include_directories("${PROGRAM_PREFIX}server"
                           PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/../")

if (DEFINED EMSCRIPTEN)
    set(SDL_FLAGS "-s USE_SDL=2")
    if(ENABLE_SDL2_MIXER)
        set(SDL_FLAGS "${SDL_FLAGS} -s USE_SDL_MIXER=2")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL_MIXER=2")
    endif()
    if(ENABLE_SDL2_NET)
        set(SDL_FLAGS "${SDL_FLAGS} -s USE_SDL_NET=2")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL_NET=2")
    endif()
else()
    set(SDL_LINK_LIBS SDL2::SDL2main SDL2::SDL2)
    if(ENABLE_SDL2_MIXER)
        list(APPEND SDL_LINK_LIBS SDL2_mixer::SDL2_mixer)
    endif()
    if(ENABLE_SDL2_NET)
        list(APPEND SDL_LINK_LIBS SDL2_net::SDL2_net)
    endif()
endif()

target_link_libraries("${PROGRAM_PREFIX}server" ${SDL_LINK_LIBS})
set_target_properties("${PROGRAM_PREFIX}server" PROPERTIES COMPILE_FLAGS "${SDL_FLAGS}")
set_target_properties("${PROGRAM_PREFIX}server" PROPERTIES LINK_FLAGS "${SDL_FLAGS}")

# Source files used by the game binaries (chocolate-doom, etc.)
set(GAME_SOURCE_FILES
    aes_prng.c          aes_prng.h
    d_event.c           d_event.h
                        doomkeys.h
                        doomtype.h
    d_iwad.c            d_iwad.h
    d_loop.c            d_loop.h
    d_mode.c            d_mode.h
                        d_ticcmd.h
    deh_str.c           deh_str.h
    gusconf.c           gusconf.h
    i_cdmus.c           i_cdmus.h
    i_endoom.c          i_endoom.h
    i_flmusic.c
    i_glob.c            i_glob.h
    i_input.c           i_input.h
    i_joystick.c        i_joystick.h
                        i_swap.h
    i_musicpack.c
    i_oplmusic.c
    i_pcsound.c
    i_sdlmusic.c
    i_sdlsound.c
    i_sound.c           i_sound.h
    i_timer.c           i_timer.h
    i_video.c           i_video.h
    i_videohr.c         i_videohr.h
    i_winmusic.c

    ## PYTHON ADDITION (begin)
    # Add your Python subsystem here
    i_python.c          i_python.h
    ## PYTHON ADDITION (end)

    midifallback.c      midifallback.h
    midifile.c          midifile.h
    mus2mid.c           mus2mid.h
    m_bbox.c            m_bbox.h
    m_cheat.c           m_cheat.h
    m_config.c          m_config.h
    m_controls.c        m_controls.h
    m_fixed.c           m_fixed.h
    net_client.c        net_client.h
    net_common.c        net_common.h
    net_dedicated.c     net_dedicated.h
    net_defs.h
    net_gui.c           net_gui.h
    net_io.c            net_io.h
    net_loop.c          net_loop.h
    net_packet.c        net_packet.h
    net_petname.c       net_petname.h
    net_query.c         net_query.h
    net_sdl.c           net_sdl.h
    net_server.c        net_server.h
    net_structrw.c      net_structrw.h
    sha1.c              sha1.h
    memio.c             memio.h
    tables.c            tables.h
    v_diskicon.c        v_diskicon.h
    v_video.c           v_video.h
                        v_patch.h
    w_checksum.c        w_checksum.h
    w_main.c            w_main.h
    w_wad.c             w_wad.h
    w_file.c            w_file.h
    w_file_stdc.c
    w_file_posix.c
    w_file_win32.c
    w_merge.c           w_merge.h
    z_zone.c            z_zone.h
)

set(GAME_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/../")

if(WIN32)
    list(APPEND GAME_SOURCE_FILES
         "../win32/win_opendir.c" "../win32/win_opendir.h")

    list(APPEND GAME_INCLUDE_DIRS
         "${PROJECT_SOURCE_DIR}/win32/")
endif()

set(DEHACKED_SOURCE_FILES
    deh_defs.h
    deh_io.c            deh_io.h
    deh_main.c          deh_main.h
    deh_mapping.c       deh_mapping.h
    deh_text.c)

# Some games support dehacked patches, some don't:

set(SOURCE_FILES ${COMMON_SOURCE_FILES} ${GAME_SOURCE_FILES})
set(SOURCE_FILES_WITH_DEH ${SOURCE_FILES} ${DEHACKED_SOURCE_FILES})

set(EXTRA_LIBS ${SDL_LINK_LIBS} textscreen pcsound opl)
if(SampleRate_FOUND)
    list(APPEND EXTRA_LIBS SampleRate::samplerate)
endif()
if(PNG_FOUND)
    list(APPEND EXTRA_LIBS PNG::PNG)
endif()
if(FluidSynth_FOUND)
    list(APPEND EXTRA_LIBS FluidSynth::libfluidsynth)
endif()
if(WIN32)
    list(APPEND EXTRA_LIBS winmm shlwapi)
endif()

## PYTHON ADDITION (begin)
# Link the standard Python library so i_python.c can resolve Py_Initialize, etc.
list(APPEND EXTRA_LIBS Python3::Python)
## PYTHON ADDITION (end)

if(WIN32)
    add_executable("${PROGRAM_PREFIX}doom" WIN32 ${SOURCE_FILES_WITH_DEH} "${CMAKE_CURRENT_BINARY_DIR}/doom-res.rc")
else()
    add_executable("${PROGRAM_PREFIX}doom" ${SOURCE_FILES_WITH_DEH})
endif()

target_include_directories("${PROGRAM_PREFIX}doom" PRIVATE ${GAME_INCLUDE_DIRS})
target_link_libraries("${PROGRAM_PREFIX}doom" doom ${EXTRA_LIBS})
set_target_properties("${PROGRAM_PREFIX}doom" PROPERTIES COMPILE_FLAGS "${SDL_FLAGS}")

if(MSVC)
    set_target_properties("${PROGRAM_PREFIX}doom" PROPERTIES LINK_FLAGS "/MANIFEST:NO")
endif()

if(WIN32)
    add_executable("${PROGRAM_PREFIX}heretic" WIN32 ${SOURCE_FILES_WITH_DEH} "${CMAKE_CURRENT_BINARY_DIR}/heretic-res.rc")
else()
    add_executable("${PROGRAM_PREFIX}heretic" ${SOURCE_FILES_WITH_DEH})
endif()

target_include_directories("${PROGRAM_PREFIX}heretic" PRIVATE ${GAME_INCLUDE_DIRS})
target_link_libraries("${PROGRAM_PREFIX}heretic" heretic ${EXTRA_LIBS})
set_target_properties("${PROGRAM_PREFIX}heretic" PROPERTIES COMPILE_FLAGS "${SDL_FLAGS}")

if(MSVC)
    set_target_properties("${PROGRAM_PREFIX}heretic" PROPERTIES LINK_FLAGS "/MANIFEST:NO")
endif()

if(WIN32)
    add_executable("${PROGRAM_PREFIX}hexen" WIN32 ${SOURCE_FILES} "${CMAKE_CURRENT_BINARY_DIR}/hexen-res.rc")
else()
    add_executable("${PROGRAM_PREFIX}hexen" ${SOURCE_FILES})
endif()

target_include_directories("${PROGRAM_PREFIX}hexen" PRIVATE ${GAME_INCLUDE_DIRS})
target_link_libraries("${PROGRAM_PREFIX}hexen" hexen ${EXTRA_LIBS})
set_target_properties("${PROGRAM_PREFIX}hexen" PROPERTIES COMPILE_FLAGS "${SDL_FLAGS}")

if(MSVC)
    set_target_properties("${PROGRAM_PREFIX}hexen" PROPERTIES LINK_FLAGS "/MANIFEST:NO")
endif()

if(WIN32)
    add_executable("${PROGRAM_PREFIX}strife" WIN32 ${SOURCE_FILES_WITH_DEH} "${CMAKE_CURRENT_BINARY_DIR}/strife-res.rc")
else()
    add_executable("${PROGRAM_PREFIX}strife" ${SOURCE_FILES_WITH_DEH})
endif()

target_include_directories("${PROGRAM_PREFIX}strife" PRIVATE ${GAME_INCLUDE_DIRS})
target_link_libraries("${PROGRAM_PREFIX}strife" strife ${EXTRA_LIBS})
set_target_properties("${PROGRAM_PREFIX}strife" PROPERTIES COMPILE_FLAGS "${SDL_FLAGS}")

if(MSVC)
    set_target_properties("${PROGRAM_PREFIX}strife" PROPERTIES LINK_FLAGS "/MANIFEST:NO")
endif()

# Source files needed for chocolate-setup:

set(SETUP_FILES
    deh_str.c           deh_str.h
    d_mode.c            d_mode.h
    d_iwad.c            d_iwad.h
    i_timer.c           i_timer.h
    m_config.c          m_config.h
    m_controls.c        m_controls.h
    net_io.c            net_io.h
    net_packet.c        net_packet.h
    net_petname.c       net_petname.h
    net_sdl.c           net_sdl.h
    net_query.c         net_query.h
    net_structrw.c      net_structrw.h
    z_native.c          z_zone.h)

if(WIN32)
    add_executable("${PROGRAM_PREFIX}setup" WIN32 ${SETUP_FILES} ${COMMON_SOURCE_FILES} "${CMAKE_CURRENT_BINARY_DIR}/setup-res.rc")
else()
    add_executable("${PROGRAM_PREFIX}setup" ${SETUP_FILES} ${COMMON_SOURCE_FILES})
endif()

target_include_directories("${PROGRAM_PREFIX}setup"
                           PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/../")
target_link_libraries("${PROGRAM_PREFIX}setup" ${SDL_LINK_LIBS} setup textscreen)
set_target_properties("${PROGRAM_PREFIX}setup" PROPERTIES COMPILE_FLAGS "${SDL_FLAGS}")

if(WIN32)
    target_link_libraries("${PROGRAM_PREFIX}setup" winmm)
endif()

if(MSVC)
    set_target_properties("${PROGRAM_PREFIX}setup" PROPERTIES
                          LINK_FLAGS "/MANIFEST:NO")
endif()

if (DEFINED EMSCRIPTEN)
    set_target_properties("${PROGRAM_PREFIX}setup" PROPERTIES LINK_FLAGS "${SDL_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXPORTED_FUNCTIONS=_main,ccall,cwrap,FS,ENV,PATH,ERRNO_CODES")
    set_target_properties("${PROGRAM_PREFIX}strife" PROPERTIES LINK_FLAGS "${SDL_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXPORTED_FUNCTIONS=_main,ccall,cwrap,FS,ENV,PATH,ERRNO_CODES")
    set_target_properties("${PROGRAM_PREFIX}doom" PROPERTIES LINK_FLAGS "${SDL_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXPORTED_FUNCTIONS=_main,ccall,cwrap,FS,ENV,PATH,ERRNO_CODES")
    set_target_properties("${PROGRAM_PREFIX}heretic" PROPERTIES LINK_FLAGS "${SDL_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXPORTED_FUNCTIONS=_main,ccall,cwrap,FS,ENV,PATH,ERRNO_CODES")
    set_target_properties("${PROGRAM_PREFIX}hexen" PROPERTIES LINK_FLAGS "${SDL_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXPORTED_FUNCTIONS=_main,ccall,cwrap,FS,ENV,PATH,ERRNO_CODES")
endif()

add_executable(midiread midifile.c z_native.c i_system.c m_argv.c m_misc.c d_iwad.c deh_str.c m_config.c)
target_compile_definitions(midiread PRIVATE "-DTEST")
target_include_directories(midiread PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/../")
if (DEFINED EMSCRIPTEN)
    set_target_properties(midiread PROPERTIES COMPILE_FLAGS "-s USE_SDL=2")
    set_target_properties(midiread PROPERTIES LINK_FLAGS "-s USE_SDL=2")
else()
    target_link_libraries(midiread SDL2::SDL2)
endif()

add_executable(mus2mid mus2mid.c memio.c z_native.c i_system.c m_argv.c m_misc.c d_iwad.c deh_str.c m_config.c)
target_compile_definitions(mus2mid PRIVATE "-DSTANDALONE")
target_include_directories(mus2mid PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/../")
if (DEFINED EMSCRIPTEN)
    set_target_properties(mus2mid PROPERTIES COMPILE_FLAGS "-s USE_SDL=2")
    set_target_properties(mus2mid PROPERTIES LINK_FLAGS "-s USE_SDL=2")
else()
    target_link_libraries(mus2mid SDL2::SDL2)
endif()