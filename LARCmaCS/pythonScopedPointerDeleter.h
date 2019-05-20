#pragma once

// Bug lib. Include first of all - math.h, then Python.h. Use Python 3.7
#include <math.h>
#include <Python.h>

struct ScopedPointerPy_DecodeLocaleDeleter{
	static inline void cleanup(wchar_t* py_argv){
		PyMem_RawFree(py_argv);
	}
};

struct ScopedPointerPyObjectDeleter{
	static inline void cleanup(PyObject* p){
		Py_CLEAR(p);
	}
};
