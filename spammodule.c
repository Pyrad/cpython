#include <Python.h>

#define SPAM_MODULE

#include "spammodule.h"


static int
PySpam_System(const char *command) {
    return system(command);
}

static PyObject *my_callback = NULL;

static PyObject * my_set_callback(PyObject *dummy, PyObject *args) {
    PyObject *result = NULL;
    PyObject *temp;

    // The ':' in "O:set_callback"
    // The list of format units ends here;
    // the string after the colon is used as the function name in error messages
    // (the “associated value” of the exception that PyArg_ParseTuple() raises).
    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(my_callback);  /* Dispose of previous callback */
        my_callback = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}


///< Test for keyword(dict) argument
static PyObject * keywdarg_parrot(PyObject *self, PyObject *args, PyObject *keywds) {
    int voltage;
    char *state = "a stiff";
    char *action = "voom";
    char *type = "Norwegian Blue";

    static char *kwlist[] = {"voltage", "state", "action", "type", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|sss", kwlist, &voltage, &state, &action, &type)) {
        return NULL;
    }

    printf("-- This parrot wouldn't %s if you put %i Volts through it.\n", action, voltage);
    printf("-- Lovely plumage, the %s -- It's %s!\n", type, state);

    Py_INCREF(Py_None);
    return Py_None;
}






static PyObject *SpamError;

static PyObject* spam_system(PyObject *self, PyObject *args) {
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
#if 0
    sts = system(command);
    if (sts < 0) {
        PyErr_SetString(SpamError, "System command failed");
        return NULL;
    }
#else
    sts = PySpam_System(command);
#endif

    return PyLong_FromLong(sts);
}





static PyObject* spamtestcallback(PyObject *self, PyObject *args) {
    int a = 0;
    int b = 0;

    printf("[spamtestcallback] Checkpoint 1\n");
    if (!PyArg_ParseTuple(args, "ii", &a, &b)) {
        return NULL;
    }

    printf("[spamtestcallback] Checkpoint 2\n");
    PyObject *result;
    PyObject *arglist;

    printf("[spamtestcallback] Checkpoint 3\n");
    arglist = Py_BuildValue("(ii)", a, b);
    printf("[spamtestcallback] Checkpoint 4\n");
    result = PyObject_CallObject(my_callback, arglist);
    printf("[spamtestcallback] Checkpoint 5\n");
    Py_DECREF(arglist);

    printf("[spamtestcallback] Checkpoint 6\n");
    if (result == NULL) {
        return NULL;
    }

    printf("The callback result is:\n");
    PyObject_Print(result, stdout, 0);
    printf("End of callback result\n");

    printf("[spamtestcallback] Checkpoint 7\n");
    Py_DECREF(result);

    result = Py_None;
    Py_INCREF(Py_None);
    return result;
}







// The so called 'method table'
static PyMethodDef SpamMethods[] = {
    // ...
    // Below 2 entries are for test of callbacks
    {"setcallback", my_set_callback, METH_VARARGS, "A callback for spam"},
    {"testcallback", spamtestcallback, METH_VARARGS, "Test for spam callback"},

    // Test for keyword argument
    {"parrot", (PyCFunction)keywdarg_parrot, METH_VARARGS | METH_KEYWORDS, "Print a lovely skit to standard output."},
	
	// If use 'METH_VARARGS | METH_KEYWORDS'
	// Then is means the spam_system shoud accept a third argument, and use
	// PyArg_ParseTupleAndKeywords() to parse the arguments, instead of PyArg_ParseTuple()
	// 'system' is the name to be used in Python
	// 'spam_system' is the corresponding C function
    {"system",  spam_system, METH_VARARGS, "Execute a shell command."},
    // ...
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


// !!! IMPORTATN !!!
// The name of this init function should be pattern like init<MODULE_NAME>
// For example, here the module to export is 'spam', so the name of it is 'initspam'
PyMODINIT_FUNC initspam(void) {
    PyObject *m;

    static void *PySpam_API[PySpam_API_pointers];
    PyObject *c_api_object;

    m = Py_InitModule("spam", SpamMethods);
    if (m == NULL) {
        return;
    }

    /* Initialize the C API pointer array */
    PySpam_API[PySpam_System_NUM] = (void *)PySpam_System;

    /* Create a Capsule containing the API pointer array's address */
    c_api_object = PyCapsule_New((void *)PySpam_API, "spam._C_API", NULL);

    if (c_api_object != NULL) {
        PyModule_AddObject(m, "_C_API", c_api_object);
    }

    SpamError = PyErr_NewException("spam.error", NULL, NULL);
    Py_INCREF(SpamError);
    PyModule_AddObject(m, "error", SpamError);
}


// If initspam is added into '_PyImport_Inittab' table, it will be called automatically
// Otherwise should call it directly after Py_Initialize()
int main(int argc, char *argv[]) {
    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(argv[0]);

    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    /* Add a static module */
    initspam();

    // ...
	
	return 0;
}
