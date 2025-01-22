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
    Demonstrates how to expose engine subsystems through a custom 'doom' module.

******************************************************************************/

#include "i_python.h"
#include <Python.h>
#include <stdio.h>

/* -------------------------------------------------------------------------
 * Example of a C function exposed to Python that prints to Doom's console.
 * Extend this as needed to interact with the engine’s subsystems:
 * ------------------------------------------------------------------------- */
static PyObject* py_doom_print(PyObject* self, PyObject* args)
{
    const char* text = NULL;
    if (!PyArg_ParseTuple(args, "s", &text))
        return NULL;

    /* Print text using the game’s console/logging. */
    fprintf(stdout, "[Python] %s\n", text);
    Py_RETURN_NONE;
}

/* 
 * Table of Python-callable functions in the "doom" module.
 * Add more binding functions to expose deeper engine features.
 */
static PyMethodDef doom_methods[] = {
    {
        "doom_print",
        (PyCFunction) py_doom_print,
        METH_VARARGS,
        "Print a message to the Doom console."
    },
    {NULL, NULL, 0, NULL}
};

/*
 * Python module definition object.
 */
static struct PyModuleDef doom_module = {
    PyModuleDef_HEAD_INIT,
    "doom",                    /* m_name */
    "Chocolate Doom integration module.", /* m_doc */
    -1,                        /* m_size (state size; -1 if not needed) */
    doom_methods,             /* m_methods */
    NULL,                      /* m_slots */
    NULL,                      /* m_traverse */
    NULL,                      /* m_clear */
    NULL                       /* m_free */
};

/*
 * Module initialization function, called on Python import doom.
 */
PyMODINIT_FUNC PyInit_doom(void)
{
    return PyModule_Create(&doom_module);
}

/* -------------------------------------------------------------------------
 * I_InitPython
 * Initialize the Python interpreter and register the "doom" module.
 * ------------------------------------------------------------------------- */
void I_InitPython(void)
{
    /* Optional: set any custom program name, etc. */
    /* Py_SetProgramName(L"chocolate_doom"); */

    /* Register our custom module so that Python can import it. */
    PyImport_AppendInittab("doom", PyInit_doom);

    Py_Initialize();
    if (!Py_IsInitialized())
    {
        fprintf(stderr, "I_InitPython: Failed to initialize Python interpreter.\n");
        return;
    }

    fprintf(stdout, "I_InitPython: Python interpreter initialized.\n");
}

/* -------------------------------------------------------------------------
 * I_ShutdownPython
 * Finalize the Python interpreter if it is active.
 * ------------------------------------------------------------------------- */
void I_ShutdownPython(void)
{
    if (Py_IsInitialized())
    {
        Py_Finalize();
        fprintf(stdout, "I_ShutdownPython: Python interpreter finalized.\n");
    }
}

/* -------------------------------------------------------------------------
 * I_RunPythonScript
 * Execute a Python file using PyRun_SimpleFile.
 * ------------------------------------------------------------------------- */
void I_RunPythonScript(const char *filename)
{
    if (!Py_IsInitialized())
    {
        fprintf(stderr, "I_RunPythonScript: Python not initialized.\n");
        return;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        fprintf(stderr, "I_RunPythonScript: Failed to open '%s'\n", filename);
        return;
    }

    /* Runs the file as if it was a main script. */
    PyRun_SimpleFile(fp, filename);
    fclose(fp);
    fprintf(stdout, "I_RunPythonScript: Successfully ran '%s'.\n", filename);
}
