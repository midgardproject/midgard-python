/* 
 * Copyright (C) 2007, 2010 Piotr Pokora <piotrek.pokora@gmail.com>
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

static PyObject *
pymidgard_reflector_object_get_property_primary (PyGObject *self, PyObject *args) 
{
	const gchar *classname;

	if(!PyArg_ParseTuple(args, "s", &classname))
		return NULL;
	
	const gchar *pname = 
		midgard_reflector_object_get_property_primary(classname);

	if(pname == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", pname); 
}

static PyObject *
pymidgard_reflector_object_get_property_parent (PyGObject *self, PyObject *args) 
{
	const gchar *classname;

	if(!PyArg_ParseTuple(args, "s", &classname))
		return NULL;
	
	const gchar *pname = 
		midgard_reflector_object_get_property_parent(classname);

	if(pname == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", pname); 
}

static PyObject *
pymidgard_reflector_object_get_property_up (PyGObject *self, PyObject *args) 
{
	const gchar *classname;

	if(!PyArg_ParseTuple(args, "s", &classname))
		return NULL;
	
	const gchar *pname = 
		midgard_reflector_object_get_property_up (classname);

	if(pname == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", pname); 
}

static PyObject *
pymidgard_reflector_object_get_property_unique (PyGObject *self, PyObject *args) 
{
	const gchar *classname;

	if(!PyArg_ParseTuple(args, "s", &classname))
		return NULL;
	
	const gchar *pname = 
		midgard_reflector_object_get_property_unique (classname);

	if(pname == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", pname); 
}

static PyObject *
pymidgard_reflector_object_list_children (PyGObject *self, PyObject *args) 
{
	const gchar *classname;

	if(!PyArg_ParseTuple(args, "s", &classname))
		return NULL;

	guint n_child;
	guint i;
	gchar **children = midgard_reflector_object_list_children (classname, &n_child);

	if (!children)
		n_child = 0;

	PyObject *list = PyTuple_New (n_child);

	if(!children)
		return list;

	for (i = 0; i < n_child; i++) {
	
		PyObject *strname = PyString_FromString(children[i]);
		PyTuple_SetItem(list, i, strname);
	}

	return list;
}

static PyObject *
pymidgard_reflector_object_has_metadata_class (PyGObject *self, PyObject *args) 
{
	const gchar *classname;

	if (!PyArg_ParseTuple (args, "s", &classname))
		return NULL;
	
	if (midgard_reflector_object_has_metadata_class (classname))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_reflector_object_get_metadata_class (PyGObject *self, PyObject *args) 
{	
	const gchar *classname;
	const gchar *name;

	if (!PyArg_ParseTuple (args, "s", &classname))
		return NULL;

	const gchar *value = midgard_reflector_object_get_metadata_class (classname);

	if (value == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue ("s", value); 
}


static PyObject *
pymidgard_reflector_object_get_schema_value (PyGObject *self, PyObject *args) 
{	
	const gchar *classname;
	const gchar *name;

	if (!PyArg_ParseTuple (args, "ss", &classname, &name))
		return NULL;

	const gchar *value = 
		midgard_reflector_object_get_schema_value (classname, name);

	if (value == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue ("s", value); 
}

static PyMethodDef pymidgard_reflector_object_methods[] = {
	{ "get_property_primary", 
		(PyCFunction)pymidgard_reflector_object_get_property_primary, METH_VARARGS | METH_STATIC},
	{ "get_property_parent", 
		(PyCFunction)pymidgard_reflector_object_get_property_parent, METH_VARARGS | METH_STATIC},
	{ "get_property_up", 
		(PyCFunction)pymidgard_reflector_object_get_property_up, METH_VARARGS | METH_STATIC},
	{ "get_property_unique", 
		(PyCFunction)pymidgard_reflector_object_get_property_unique, METH_VARARGS | METH_STATIC},
	{ "list_children",
		(PyCFunction)pymidgard_reflector_object_list_children, METH_VARARGS | METH_STATIC},
	{ "get_metadata_class",
		(PyCFunction)pymidgard_reflector_object_get_metadata_class, METH_VARARGS | METH_STATIC},
	{ "has_metadata_class",
		(PyCFunction)pymidgard_reflector_object_has_metadata_class, METH_VARARGS | METH_STATIC},
	{ "get_schema_value", 
		(PyCFunction)pymidgard_reflector_object_get_schema_value, METH_VARARGS | METH_STATIC},
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_reflector_object_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "reflector_object",                   /* tp_name */
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
    pymidgard_reflector_object_methods, /* tp_methods */
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

void py_midgard_reflector_object_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"reflector_object", 
			MIDGARD_TYPE_OBJECT, 
			&Pymidgard_reflector_object_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
