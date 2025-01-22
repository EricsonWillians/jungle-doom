/******************************************************************************

 i_python.c
 Chocolate Doom Python Subsystem Implementation

 Based on the Doom Source code (C) 1993-1996 Id Software, Inc.
 Additional code (C) 2025 by:
    - Ericson Willians (ericsonwillians@protonmail.com)

 This source is distributed under the GNU General Public License v2.
 See the file COPYING for more details.

 Description:
    Implements an embedded Python interpreter subsystem for Chocolate Doom.
    Uses the official Python C API. Manages the lifecycle of the interpreter.

******************************************************************************/

#include "i_python.h"
#include <Python.h>
#include <stdio.h>

//=============================================================================
//
// I_InitPython
//
//=============================================================================
void I_InitPython(void)
{
    // Optional: set the program name before Py_Initialize()
    // or manipulate sys.path, etc.
    Py_Initialize();
}

//=============================================================================
//
// I_ShutdownPython
//
//=============================================================================
void I_ShutdownPython(void)
{
    if (Py_IsInitialized())
    {
        Py_Finalize();
    }
}

//=============================================================================
//
// I_RunPythonScript
//
//=============================================================================
void I_RunPythonScript(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        fprintf(stderr, "I_RunPythonScript: Failed to open '%s'\n", filename);
        return;
    }

    PyRun_SimpleFile(fp, filename);
    fclose(fp);
}
