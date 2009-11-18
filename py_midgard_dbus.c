/* 
 * Copyright (C) 2007 Piotr Pokora <piotrek.pokora@gmail.com>
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

PyTypeObject G_GNUC_INTERNAL Pymidgard_dbus_Type;

#define DBUS_DEBUG(__name) \
	CHECK_MGD; \
	CLASS_METHOD_DEBUG(Pymidgard_dbus_Type.tp_name, __name);

#define __GET_DBUS	\
	MidgardDbus *mbus = MIDGARD_DBUS(self->obj);	\
	g_assert(mbus != NULL); 

static int
__dbus_constructor(PyGObject *self, PyObject *args, PyObject *kwargs)
{
	DBUS_DEBUG("__init__");
	const gchar *path;
	gboolean use_session = FALSE;

	if(!PyArg_ParseTuple(args,"s|b:__init__", &path, &use_session))
		return -1;

	MidgardConnection *mgd =
		_py_midgard_connection_singleton_get();

	MidgardDbus *mbus = midgard_dbus_new(mgd, path, use_session);
	
	if(!mbus) 
		return -1;
	
	self->obj = G_OBJECT(mbus);
	
	return 0;
}

static PyObject *
pymidgard_dbus_send(PyGObject *self, PyObject *args)
{
	DBUS_DEBUG("send");
	const gchar *path, *msg;
	gboolean use_session = FALSE;

	if(!PyArg_ParseTuple(args, "ssb", &path, &msg, &use_session))
		return NULL;

	MidgardConnection *mgd =
		_py_midgard_connection_singleton_get();

	midgard_dbus_send(mgd, path, msg, use_session);

	Py_RETURN_NONE;
}

static PyObject *
pymidgard_dbus_get_message(PyGObject *self, PyObject *args)
{
	DBUS_DEBUG("get_message");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	__GET_DBUS;

	const gchar *msg = midgard_dbus_get_message(mbus);

	return Py_BuildValue("s", msg);
}

static PyMethodDef pymidgard_dbus_methods[] = {
	{ "send", (PyCFunction)pymidgard_dbus_send, METH_VARARGS | METH_STATIC},
	{ "get_message", (PyCFunction)pymidgard_dbus_get_message, METH_VARARGS},
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_dbus_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "dbus",                   /* tp_name */
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
    pymidgard_dbus_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__dbus_constructor,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void py_midgard_dbus_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"dbus", 
			MIDGARD_TYPE_DBUS, 
			&Pymidgard_dbus_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
