/* 
 * Copyright (C) 2009 Piotr Pokora <piotrek.pokora@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define NO_IMPORT_PYGOBJECT
#include "py_midgard.h"

PyTypeObject G_GNUC_INTERNAL Pymidgard_storage_Type;

#define STORAGE_DEBUG(__name) \
	CHECK_MGD; \
	CLASS_METHOD_DEBUG(Pymidgard_storage_Type.tp_name, __name);

#define __STORAGE_FUNC \
	gchar *classname; \
	if(!PyArg_ParseTuple(args, "s", &classname)) \
		return NULL; \
	MidgardConnection *mgd = _py_midgard_connection_singleton_get(); \
	if (!mgd) { \
		g_warning("Underlying midgard connection not found"); \
		return NULL; \
	} 

static PyObject *
pymidgard_storage_create_class_storage (PyGObject *self, PyObject *args) 
{	
	STORAGE_DEBUG("create_class_storage");
	
	__STORAGE_FUNC

	if (midgard_storage_create (mgd, classname))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_storage_create (PyGObject *self, PyObject *args) 
{	
	STORAGE_DEBUG("create");
	
	__STORAGE_FUNC

	if (midgard_storage_create (mgd, classname))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_storage_update_class_storage(PyGObject *self, PyObject *args) 
{	
	STORAGE_DEBUG("update_class_storage");
	
	__STORAGE_FUNC

	if (midgard_storage_update (mgd, classname))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_storage_update (PyGObject *self, PyObject *args) 
{	
	STORAGE_DEBUG("update");
	
	__STORAGE_FUNC

	if (midgard_storage_update (mgd, classname))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_storage_delete_class_storage (PyGObject *self, PyObject *args) 
{	
	STORAGE_DEBUG("delete_class_storage");
	
	__STORAGE_FUNC

	if (midgard_storage_delete (mgd, classname))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_storage_delete (PyGObject *self, PyObject *args) 
{	
	STORAGE_DEBUG("delete");
	
	__STORAGE_FUNC

	if (midgard_storage_delete (mgd, classname))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_storage_class_storage_exists(PyGObject *self, PyObject *args) 
{	
	STORAGE_DEBUG("class_storage_exists");
	
	__STORAGE_FUNC

	if (midgard_storage_exists (mgd, classname))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_storage_exists (PyGObject *self, PyObject *args) 
{	
	STORAGE_DEBUG("exists");
	
	__STORAGE_FUNC

	if (midgard_storage_exists (mgd, classname))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_storage_create_base_storage(PyGObject *self, PyObject *args) 
{
	STORAGE_DEBUG("create_base_storage");
	
	MidgardConnection *mgd = 
		_py_midgard_connection_singleton_get();
	
	if (midgard_storage_create_base_storage(mgd))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;

}

static PyMethodDef pymidgard_storage_methods[] = {
	{ "create_base_storage", (PyCFunction)pymidgard_storage_create_base_storage, METH_NOARGS | METH_STATIC},
	{ "create_class_storage", (PyCFunction)pymidgard_storage_create_class_storage, METH_VARARGS | METH_STATIC},
	{ "create", (PyCFunction)pymidgard_storage_create, METH_VARARGS | METH_STATIC},
	{ "update_class_storage", (PyCFunction)pymidgard_storage_update_class_storage, METH_VARARGS | METH_STATIC},
	{ "update", (PyCFunction)pymidgard_storage_update, METH_VARARGS | METH_STATIC},
	{ "delete_class_storage", (PyCFunction)pymidgard_storage_delete_class_storage, METH_VARARGS | METH_STATIC},
	{ "delete", (PyCFunction)pymidgard_storage_delete, METH_VARARGS | METH_STATIC},
	{ "class_storage_exists", (PyCFunction)pymidgard_storage_class_storage_exists, METH_VARARGS | METH_STATIC},
	{ "exists", (PyCFunction)pymidgard_storage_exists, METH_VARARGS | METH_STATIC},

	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_storage_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "storage",                   /* tp_name */
    sizeof(PyGObject),          /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)_py_midgard_gobject_destructor,        /* tp_dealloc */
    (printfunc)0,                      /* tp_print */
    (getattrfunc)0,       /* tp_getattr */
    (setattrfunc)0,       /* tp_setattr */
    (cmpfunc)0,           /* tp_compare */
    (reprfunc)0,             /* tp_repr */
    (PyNumberMethods*)0,     /* tp_as_number */
    (PySequenceMethods*)0, /* tp_as_sequence */
    (PyMappingMethods*)0,   /* tp_as_mapping */
    (hashfunc)0,             /* tp_hash */
    (ternaryfunc)0,          /* tp_call */
    (reprfunc)0,              /* tp_str */
    (getattrofunc)0,     /* tp_getattro */
    (setattrofunc)0,     /* tp_setattro */
    (PyBufferProcs*)0,  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                      /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)0,     /* tp_traverse */
    (inquiry)0,             /* tp_clear */
    (richcmpfunc)0,   /* tp_richcompare */
    offsetof(PyGObject, weakreflist),             /* tp_weaklistoffset */
    (getiterfunc)0,          /* tp_iter */
    (iternextfunc)0,     /* tp_iternext */
    pymidgard_storage_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)0,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void py_midgard_storage_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"storage", 
			MIDGARD_TYPE_STORAGE, 
			&Pymidgard_storage_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
