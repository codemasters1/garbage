#include <Python.h>
//#include "PluginAPI.h"

static PyObject* aqq_add(PyObject *self, PyObject *args)
{
	long a, b;
	
	if (!PyArg_ParseTuple(args, "ll", &a, &b))
		return NULL;
	
	return PyLong_FromLong(a * b);
}

static PyMethodDef AqqMethods[] = {
	{"add", aqq_add, METH_VARARGS, "AQQ API module"},
	{NULL, NULL, 0, NULL}
};

static PyModuleDef AqqModule = {
    PyModuleDef_HEAD_INIT, "aqq", NULL, -1, AqqMethods, NULL, NULL, NULL, NULL
};

static PyObject* PyInit_aqq(void)
{
    return PyModule_Create(&AqqModule);
}

void init(wchar_t *name)
{
	Py_SetProgramName(name);
	Py_SetPythonHome(L"C:\\Python34x86");
  //Py_SetPath(Py_GetPath());
	PyImport_AppendInittab("aqq", &PyInit_aqq);
	Py_Initialize();
}

int main(int argc, char *argv[])
{
	init((wchar_t *) argv[0]);

	PyObject *pName = PyUnicode_FromString("plug");
	PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL)
	{
		PyObject *pFunc = PyObject_GetAttrString(pModule, "init");
		
		if (pFunc && PyCallable_Check(pFunc))
		{
			PyObject *pArgs, *pValue;
		
			pArgs = PyTuple_New(2);
            for (int i = 0; i < 2; ++i)
			{
                pValue = PyLong_FromLong((i+1) << 3);
                if (!pValue)
				{
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    return 1;
                }
				
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL)
			{
                Py_DECREF(pValue);
            }
            else
			{
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				return 1;
			}
        }
        else
		{
			if (PyErr_Occurred())
				PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else
	{
        PyErr_Print();
        return 1;
    }
    Py_Finalize();
    return 0;
}