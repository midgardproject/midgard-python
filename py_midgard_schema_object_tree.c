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
pymidgard_schema_object_tree_is_in_tree (PyGObject *self, PyObject *args) 
{
	PyObject *pobject;
	PyObject *parent_pobject;

	if (!PyArg_ParseTuple (args, "OO", &pobject, &parent_pobject))
		return NULL;

	MidgardObject *object = MIDGARD_OBJECT (((PyGObject *)pobject)->obj);
	MidgardObject *parent_object = MIDGARD_OBJECT (((PyGObject *)pobject)->obj);

	gboolean exists = midgard_schema_object_tree_is_in_tree (object, parent_object); 	

	if (exists) 
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_schema_object_tree_get_parent_name (PyGObject *self, PyObject *args) 
{
	PyObject *pobject;

	if (!PyArg_ParseTuple  (args, "O", &pobject))
		return NULL;

	MidgardObject *object = MIDGARD_OBJECT (((PyGObject *)pobject)->obj);

	const gchar *parent_name = 
		midgard_schema_object_tree_get_parent_name (object);

	if (parent_name)
		return Py_BuildValue ("s", parent_name);

	Py_RETURN_NONE;
}

static PyObject *
pymidgard_schema_object_tree_get_parent_object (PyGObject *self, PyObject *args) 
{
	PyObject *pobject;

	if (!PyArg_ParseTuple  (args, "O", &pobject))
		return NULL;

	MidgardObject *object = MIDGARD_OBJECT (((PyGObject *)pobject)->obj);

	MidgardObject *parent_object = 
		midgard_schema_object_tree_get_parent_object (object);

	if (parent_object)
		return Py_BuildValue ("O", pygobject_new (G_OBJECT (parent_object)));
	
	Py_RETURN_NONE;
}

static PyObject *
pymidgard_schema_object_tree_list_objects (PyGObject *self, PyObject *args) 
{
	PyObject *pobject;

	if (!PyArg_ParseTuple  (args, "O", &pobject))
		return NULL;

	MidgardObject *object = MIDGARD_OBJECT (((PyGObject *)pobject)->obj);

	guint n_objects;
	MidgardObject **objects = midgard_schema_object_tree_list_objects (object, &n_objects);

	PyObject *list = PyTuple_New (n_objects);

	if (!objects)
		return list;

	OBJECTS2LIST(objects, list);
	g_free (objects);
	return list;
}

static PyObject *
pymidgard_schema_object_tree_list_children_objects (PyGObject *self, PyObject *args) 
{
	PyObject *pobject;
	gchar *classname;

	if (!PyArg_ParseTuple  (args, "Os", &pobject, &classname))
		return NULL;

	MidgardObject *object = MIDGARD_OBJECT (((PyGObject *)pobject)->obj);

	guint n_objects;
	MidgardObject **objects = midgard_schema_object_tree_list_children_objects (object, (const gchar *)classname, &n_objects);

	PyObject *list = PyTuple_New (n_objects);

	if (!objects)
		return list;

	OBJECTS2LIST(objects, list);
	g_free (objects);
	return list;
}

static PyMethodDef pymidgard_schema_object_tree_methods[] = {
	{ "is_in_tree", 
		(PyCFunction)pymidgard_schema_object_tree_is_in_tree, METH_VARARGS | METH_STATIC},
	{ "get_parent_name", 
		(PyCFunction)pymidgard_schema_object_tree_get_parent_name, METH_VARARGS | METH_STATIC},
	{ "get_parent_object", 
		(PyCFunction)pymidgard_schema_object_tree_get_parent_object, METH_VARARGS | METH_STATIC},
	{ "list_objects", 
		(PyCFunction)pymidgard_schema_object_tree_list_objects, METH_VARARGS | METH_STATIC},
	{ "list_children_objects", 
		(PyCFunction)pymidgard_schema_object_tree_list_children_objects, METH_VARARGS | METH_STATIC},
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_schema_object_tree_Type = {
    PyObject_HEAD_INIT (NULL)
    0,                                 /* ob_size */
    "schema_object_tree",                   /* tp_name */
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
    pymidgard_schema_object_tree_methods, /* tp_methods */
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

void py_midgard_schema_object_tree_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class (d, "schema_object_tree", 
			MIDGARD_TYPE_SCHEMA_OBJECT_TREE, 
			&Pymidgard_schema_object_tree_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
