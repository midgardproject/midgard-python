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

PyTypeObject G_GNUC_INTERNAL Pymidgard_transaction_Type;

#define TRANSACTION_DEBUG(__name) \
	CHECK_MGD; \
	CLASS_METHOD_DEBUG(Pymidgard_transaction_Type.tp_name, __name);

#define __GET_TRANSACTION	\
	MidgardTransaction *trns = MIDGARD_TRANSACTION(self->obj);	\
	g_assert(trns != NULL); 

static int
__transaction_constructor(PyGObject *self, PyObject *args, PyObject *kwargs)
{
	TRANSACTION_DEBUG("__init__");

	MidgardConnection *mgd =
		_py_midgard_connection_singleton_get();

	MidgardTransaction *trns = midgard_transaction_new(mgd);
	
	if(!trns) 
		return -1;
	
	self->obj = G_OBJECT(trns);
	
	return 0;
}

static PyObject *
pymidgard_transaction_begin(PyGObject *self, PyObject *args)
{
	TRANSACTION_DEBUG("begin");

	__GET_TRANSACTION;
	
	if (midgard_transaction_begin(trns))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_transaction_commit(PyGObject *self, PyObject *args)
{
	TRANSACTION_DEBUG("commit");

	__GET_TRANSACTION;
	
	if (midgard_transaction_commit(trns))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_transaction_rollback(PyGObject *self, PyObject *args)
{
	TRANSACTION_DEBUG("rollback");

	__GET_TRANSACTION;
	
	if (midgard_transaction_rollback(trns))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_transaction_get_status(PyGObject *self, PyObject *args)
{
	TRANSACTION_DEBUG("get_status");

	__GET_TRANSACTION;
	
	if (midgard_transaction_get_status(trns))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_transaction_get_name(PyGObject *self, PyObject *args)
{
	TRANSACTION_DEBUG("get_name");
	
	__GET_TRANSACTION;
	const gchar *msg = midgard_transaction_get_name(trns);

	return Py_BuildValue("s", msg);
}

static PyMethodDef pymidgard_transaction_methods[] = {
	{ "begin", (PyCFunction)pymidgard_transaction_begin, METH_NOARGS},
	{ "commit", (PyCFunction)pymidgard_transaction_commit, METH_NOARGS},
	{ "rollback", (PyCFunction)pymidgard_transaction_rollback, METH_NOARGS},
	{ "get_status", (PyCFunction)pymidgard_transaction_get_status, METH_NOARGS},
	{ "get_name", (PyCFunction)pymidgard_transaction_get_name, METH_NOARGS},
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_transaction_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "transaction",                   /* tp_name */
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
    pymidgard_transaction_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__transaction_constructor,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void py_midgard_transaction_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"transaction", 
			MIDGARD_TYPE_TRANSACTION, 
			&Pymidgard_transaction_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
