#include <map>
#include <vector>
#include <windows.h>
#include "Python.h"
#include "PluginAPI.h"

using std::map;
using std::vector;
using std::wstring;

#define DEF_HOOK(name) INT_PTR __stdcall __##name(WPARAM w, LPARAM l) {return HookHandler(name, w, l);}
#define HOOK(name) { name, __##name }

HINSTANCE PluginInstance;
TPluginLink PluginLink;
TPluginInfo PluginInfo;

map<PyObject *, vector<PyObject *>> python_hooks;

static INT_PTR
HookHandler(wchar_t* hook_name, WPARAM wParam, LPARAM lParam)
{
	PyObject * name = PyUnicode_FromWideChar(hook_name, -1);

	for (auto &it : python_hooks)
	{
		if (PyObject_RichCompareBool(it.first, name, Py_EQ))
		{
			PyObject *w, *l, *params, *result;

			for (auto hook : it.second)
			{
				w = PyLong_FromLong(wParam);
				l = PyLong_FromLong(lParam);
				params = PyTuple_Pack(2, w, l);
				result = PyObject_CallObject(hook, params);

				Py_DECREF(w);
				Py_DECREF(l);
				Py_DECREF(params);
				Py_DECREF(result);
			}

			break;
		}
	}

	Py_DECREF(name);

	return 0;
}

DEF_HOOK(AQQ_SYSTEM_MODULESLOADED)
DEF_HOOK(AQQ_CONTACTS_RECVMSG)

static map<wchar_t*, TAQQHook> hooks = {
	HOOK(AQQ_SYSTEM_MODULESLOADED),
	HOOK(AQQ_CONTACTS_RECVMSG)
};

BOOL __stdcall
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	PluginInstance = hinstDLL;
	return TRUE;
}

static PyObject*
aqq_show_message(PyObject *self, PyObject *args)
{
	wchar_t *text, *caption;

	if (!PyArg_ParseTuple(args, "uu", &text, &caption))
		return NULL;

	MessageBoxW(0, text, caption, 16);
	
	Py_RETURN_NONE;
}

static PyObject*
aqq_hook_event(PyObject *self, PyObject *args)
{
	PyObject *name, *func;

	if (!PyArg_ParseTuple(args, "OO", &name, &func))
		return NULL;

	if (PyCallable_Check(func))
	{
		bool found = false;

		for (auto &it : python_hooks)
		{
			if (PyObject_RichCompareBool(it.first, name, Py_EQ))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			python_hooks[name];
			Py_INCREF(name);
		}

		python_hooks[name].push_back(func);
		Py_INCREF(func);
	}

	Py_RETURN_NONE;
}

static PyMethodDef
AqqMethods[] = {
		{ "hook_event", aqq_hook_event, METH_VARARGS, NULL },
		{ "show_message", aqq_show_message, METH_VARARGS, NULL },
		{ NULL, NULL, 0, NULL }
};

static PyModuleDef 
AqqModule = {
	PyModuleDef_HEAD_INIT, "aqq_api", NULL, -1, AqqMethods,
	NULL, NULL, NULL, NULL
};

static PyObject*
PyInit_aqq(void)
{
	return PyModule_Create(&AqqModule);
}


INT_PTR __stdcall
Load(PPluginLink Link)
{
	PluginLink = *Link;
	
	for (auto &it : hooks)
	{
		PluginLink.HookEvent(it.first, it.second);
	}

	wstring plugin_path((wchar_t*) PluginLink.CallService(AQQ_FUNCTION_GETPLUGINUSERDIR, (WPARAM)PluginInstance, 0));
	plugin_path += L"\\Python4AQQ;C:\\DEV\\Python34\\Lib";
	Py_SetPath(plugin_path.c_str());

	PyImport_AppendInittab("aqq_api", &PyInit_aqq);
	Py_Initialize();

	PyObject * module_name = PyUnicode_FromString("plugin");
	PyObject * module = PyImport_Import(module_name);
	Py_DECREF(module_name);

	if (module != NULL)
	{
		PyObject * func = PyObject_GetAttrString(module, "init");

		if (func && PyCallable_Check(func))
		{
			PyObject * args = PyTuple_New(0);
			PyObject * value = PyObject_CallObject(func, args);
			Py_DECREF(args);

			if (value != NULL)
			{
				Py_DECREF(value);
			}
		}

		Py_XDECREF(func);
	}

	Py_XDECREF(module);
	
	return 0;
}

INT_PTR __stdcall
Unload()
{
	Py_Finalize();

	for (auto &it : hooks)
	{
		PluginLink.UnhookEvent(it.second);
	}

	return 0;
}

PPluginInfo __stdcall
AQQPluginInfo(DWORD AQQVersion)
{
	PluginInfo.cbSize = sizeof(TPluginInfo);
	PluginInfo.ShortName = L"Python4AQQ";
	PluginInfo.Version = PLUGIN_MAKE_VERSION(0, 0, 1, 1);
	PluginInfo.Description = L"Python scripting engine for AQQ";
	PluginInfo.Author = L"darksv";
	PluginInfo.AuthorMail = L"darke@gmail.com";
	PluginInfo.Copyright = L"xD";
	PluginInfo.Homepage = L"http://python4aqq.github.io/";
	PluginInfo.Flag = 0;
	PluginInfo.ReplaceDefaultModule = 0;

	return &PluginInfo;
}