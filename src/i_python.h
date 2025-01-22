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
    Declares initialization, shutdown, and example execution functions.

******************************************************************************/

#ifndef __I_PYTHON__
#define __I_PYTHON__

#ifdef __cplusplus
extern "C" {
#endif

void I_InitPython(void);
void I_ShutdownPython(void);
void I_RunPythonScript(const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* __I_PYTHON__ */
