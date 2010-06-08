/* 
 * Copyright (C) 2010 Piotr Pokora <piotrek.pokora@gmail.com>
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

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_executor_Type;
PyTypeObject G_GNUC_INTERNAL Pymidgard_query_select_Type;

static PyObject *
pymidgard_query_executor_set_constraint (PyGObject *self, PyObject *args)
{
	PyObject *pyconstraint;
	if (!PyArg_ParseTuple (args, "O", &pyconstraint))
		return NULL;

	MidgardQueryExecutor *executor = MIDGARD_QUERY_EXECUTOR (self->obj);
	MidgardQueryConstraintSimple *mqc = MIDGARD_QUERY_CONSTRAINT_SIMPLE (((PyGObject *)pyconstraint)->obj);

	if (midgard_query_executor_set_constraint (executor, mqc))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_query_executor_add_order (PyGObject *self, PyObject *args)
{	
	const gchar *order = NULL;
	PyObject *pyproperty;
	if (!PyArg_ParseTuple (args, "O|s", &pyproperty, &order))
		return NULL;

	MidgardQueryExecutor *executor = MIDGARD_QUERY_EXECUTOR(self->obj);
	MidgardQueryProperty *property = MIDGARD_QUERY_PROPERTY (((PyGObject*) pyproperty)->obj);

	if (midgard_query_executor_add_order (executor, property, order))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_query_executor_set_offset (PyGObject *self, PyObject *args)
{	
	guint offset;
	if (!PyArg_ParseTuple (args, "i", &offset))
		return NULL;

	MidgardQueryExecutor *executor = MIDGARD_QUERY_EXECUTOR (self->obj);
	midgard_query_executor_set_offset(executor, offset);
	
	Py_RETURN_NONE;	
}

static PyObject *
pymidgard_query_executor_set_limit (PyGObject *self, PyObject *args)
{	
	long limit;
	if (!PyArg_ParseTuple (args, "l", &limit))
		return NULL;

	MidgardQueryExecutor *executor = MIDGARD_QUERY_EXECUTOR (self->obj);
	midgard_query_executor_set_limit (executor, limit);
	
	Py_RETURN_NONE;
}

static PyObject *
pymidgard_query_executor_add_join (PyGObject *self, PyObject *args)
{	
	gchar *jtype;
	PyObject *py_left_prop;
	PyObject *py_right_prop;

	if (!PyArg_ParseTuple (args, "sOO", &jtype, &py_left_prop, &py_right_prop))
		return NULL;

	MidgardQueryExecutor *executor = MIDGARD_QUERY_EXECUTOR (self->obj);
	MidgardQueryProperty *left_prop = MIDGARD_QUERY_PROPERTY (((PyGObject *)py_left_prop)->obj);
	MidgardQueryProperty *right_prop = MIDGARD_QUERY_PROPERTY (((PyGObject *)py_right_prop)->obj);

	if (midgard_query_executor_add_join (executor, jtype, left_prop, right_prop))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_query_executor_execute (PyGObject *self, PyObject *args)
{
	MidgardQueryExecutor *executor = MIDGARD_QUERY_EXECUTOR (self->obj);
	midgard_query_executor_execute (executor);
	Py_RETURN_NONE;
}

static PyMethodDef pymidgard_query_executor_methods[] = {
	{ "set_constraint", (PyCFunction)pymidgard_query_executor_set_constraint, METH_VARARGS },
	{ "set_offset", (PyCFunction)pymidgard_query_executor_set_offset, METH_VARARGS },
	{ "set_limit", (PyCFunction)pymidgard_query_executor_set_limit, METH_VARARGS },
	{ "add_order", (PyCFunction)pymidgard_query_executor_add_order, METH_VARARGS },
	{ "add_join", (PyCFunction)pymidgard_query_executor_add_join, METH_VARARGS },
	{ "execute", (PyCFunction)pymidgard_query_executor_execute, METH_NOARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_executor_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "executor",                   /* tp_name */
    sizeof (PyGObject),          /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)0,  /* tp_dealloc */
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
    _py_midgard_get_object_attribute,     /* tp_getattro */
    _py_midgard_set_object_attribute,     /* tp_setattro */
    (PyBufferProcs*)0,  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)0,     /* tp_traverse */
    (inquiry)0,             /* tp_clear */
    (richcmpfunc)0,   /* tp_richcompare */
    offsetof (PyGObject, weakreflist),             /* tp_weaklistoffset */
    (getiterfunc)0,          /* tp_iter */
    (iternextfunc)0,     /* tp_iternext */
    pymidgard_query_executor_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof (PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)0,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,            /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void 
py_midgard_query_executor_register_class (PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class (d, "executor", 
			MIDGARD_TYPE_QUERY_EXECUTOR, &Pymidgard_query_executor_Type, 
			Py_BuildValue ("(O)", pygobject_type));

	pyg_set_object_has_new_constructor(MIDGARD_TYPE_QUERY_EXECUTOR);
}

/* QUERY SELECT */

static int
__query_select_constructor (PyGObject *self, PyObject *args, PyObject *kwargs)
{
	PyObject *py_storage;
	if (!PyArg_ParseTuple (args, "O", &py_storage))
                return -1;

	MidgardConnection *mgd = _py_midgard_connection_singleton_get ();
	MidgardQueryStorage *storage = MIDGARD_QUERY_STORAGE (((PyGObject *)py_storage)->obj);
	MidgardQuerySelect *executor = midgard_query_select_new (mgd, storage);
	
	if (!executor)
		return -1;

	self->obj = G_OBJECT (executor);	

	return 0;
}

static PyObject *
pymidgard_query_select_list_objects (PyGObject *self, PyObject *args)
{	
	MidgardQuerySelect *select = MIDGARD_QUERY_SELECT (self->obj);

	guint n_objects;
	MidgardDBObject **objects = midgard_query_select_list_objects (select, &n_objects);

	PyObject *list = PyTuple_New (n_objects);

	if (objects == NULL) 
		return list;
		
	OBJECTS2LIST(objects, list);	
	g_free (objects);
	
	return list;
}

static PyObject *
pymidgard_query_select_toggle_read_only (PyGObject *self, PyObject *args)
{	
	gboolean toggle;
	if (!PyArg_ParseTuple (args, "b", &toggle))
		return NULL;

	MidgardQuerySelect *select = MIDGARD_QUERY_SELECT (self->obj);
	midgard_query_select_toggle_read_only (select, toggle);

	Py_RETURN_NONE;
}

static PyObject *
pymidgard_query_select_include_deleted (PyGObject *self, PyObject *args)
{	
	gboolean toggle;
	if (!PyArg_ParseTuple (args, "b", &toggle))
		return NULL;

	MidgardQuerySelect *select = MIDGARD_QUERY_SELECT (self->obj);
	midgard_query_select_include_deleted (select, toggle);

	Py_RETURN_NONE;
}


static PyMethodDef pymidgard_query_select_methods[] = {
	{ "include_deleted", (PyCFunction)pymidgard_query_select_include_deleted, METH_VARARGS },
	{ "list_objects", (PyCFunction)pymidgard_query_select_list_objects, METH_NOARGS },
	{ "toggle_read_only", (PyCFunction)pymidgard_query_select_toggle_read_only, METH_VARARGS },
	{ "include_deleted", (PyCFunction)pymidgard_query_select_include_deleted, METH_VARARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_select_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "select",                   /* tp_name */
    sizeof (PyGObject),          /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)_py_midgard_gobject_destructor,  /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)0,     /* tp_traverse */
    (inquiry)0,             /* tp_clear */
    (richcmpfunc)0,   /* tp_richcompare */
    offsetof (PyGObject, weakreflist),             /* tp_weaklistoffset */
    (getiterfunc)0,          /* tp_iter */
    (iternextfunc)0,     /* tp_iternext */
    pymidgard_query_select_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof (PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__query_select_constructor,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,            /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void 
py_midgard_query_select_register_class (PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class (d, "select", 
			MIDGARD_TYPE_QUERY_SELECT, &Pymidgard_query_select_Type, 
			Py_BuildValue ("(O)", &Pymidgard_query_executor_Type));
	pyg_set_object_has_new_constructor(MIDGARD_TYPE_QUERY_SELECT);
}

/* COMMON */

void 
py_midgard_query_executor_register_classes (PyObject *d, gpointer pygobject_type)
{
	py_midgard_query_executor_register_class (d, pygobject_type);
	py_midgard_query_select_register_class (d, pygobject_type);
}

