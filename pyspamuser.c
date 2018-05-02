#include <Python.h>

#include "spammodule.h"

static PyMethodDef ClientMethods[] = {
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initpyspamuser(void) {
    PyObject *m;

    m = Py_InitModule("pyspamuser", ClientMethods);
    if (m == NULL) {
        return;
    }
    if (import_spam() < 0) {
        return;
    }
    /* additional initialization can happen here */
}

int main(int argc, char *argv[]) {
    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(argv[0]);

    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    /* Add a static module */
    initpyspamuser();

    // ...
	
	return 0;
}
