/******************************************************************************
 i_python.h
 Chocolate Doom Python Subsystem Interface

 Based on the Doom Source code (C) 1993-1996 Id Software, Inc.
 Additional code (C) 2025 by:
    - Ericson Willians (ericsonwillians@protonmail.com)

 This source is distributed under the GNU General Public License v2.
 See the file COPYING for more details.

 Description:
    Provides an embedded Python subsystem for Chocolate Doom. 
    Declares initialization, shutdown, example module creation, 
    and basic script execution functions.

******************************************************************************/

#ifndef __I_PYTHON__
#define __I_PYTHON__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the embedded Python interpreter.
 * This includes hooking up any custom modules that expose 
 * Chocolate Doom engine subsystems or functions to Python.
 */
void I_InitPython(void);

/**
 * Cleanly shut down the embedded Python interpreter.
 * Ensures that all Python resources are freed and finalizers are run.
 */
void I_ShutdownPython(void);

/**
 * Execute a Python script from the given file.
 * 
 * @param filename Path to the Python script file.
 */
void I_RunPythonScript(const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* __I_PYTHON__ */
