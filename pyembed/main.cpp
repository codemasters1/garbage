#include <iostream>
#include <Windows.h>
#include <Python.h>



static PyObject*
aqq_hook_event(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, "sO"))
		return NULL;

	PyObject * fn = PyTuple_GetItem(args, 1);
	PyObject * result = NULL;

	if (PyCallable_Check(fn))
	{
		PyObject * num = PyLong_FromLong(123);
		PyObject * params = PyTuple_Pack(2, num, num);

		

		result = PyObject_CallObject(fn, params);

		Py_DECREF(num);
		Py_DECREF(params);
	}
	
	return result;
}

static PyMethodDef AqqMethods[] = {
		{ "hook_event", aqq_hook_event, METH_VARARGS, NULL },
		{ NULL, NULL, 0, NULL }
};

static PyModuleDef AqqModule = {
	PyModuleDef_HEAD_INIT, "aqq_api", NULL, -1, AqqMethods,
	NULL, NULL, NULL, NULL
};

static PyObject*
PyInit_aqq(void)
{
	return PyModule_Create(&AqqModule);
}


int
main(int argc, char *argv[])
{
	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;

	PyImport_AppendInittab("aqq_api", &PyInit_aqq);

	Py_Initialize();

	pName = PyUnicode_FromString("plugin");
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		pFunc = PyObject_GetAttrString(pModule, "init");

		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(0);
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);

			if (pValue != NULL) {
				printf("Result of call: %ld\n", PyLong_AsLong(pValue));
				Py_DECREF(pValue);
			}
			else {
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				fprintf(stderr, "Call failed\n");
				return 1;
			}
		}
		else {
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
		return 1;
	}
	Py_Finalize();
	return 0;
}