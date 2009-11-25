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

PyTypeObject G_GNUC_INTERNAL Pymidgard_connection_Type;

#define CONNECTION_DEBUG(__name) \
	CHECK_MGD; \
	CLASS_METHOD_DEBUG(Pymidgard_connection_Type.tp_name, __name);

static MidgardConnection *_global_singleton = NULL;

void _py_midgard_connection_singleton_set(MidgardConnection *mgd)
{
	_global_singleton = mgd;
	
	midgard_connection_set_loghandler(mgd, get_global_log_handler());
}

MidgardConnection *_py_midgard_connection_singleton_get(void)
{
	return _global_singleton;
}

/* Overwrite constructor.
 * Basically we do ( almost ) the same what is done in pygobject_init.
 * The difference is that we do not need any argument(s) here, and we can 
 * easily create own singleton implementation */
static int
__connection_singleton_init(PyGObject *self, PyObject *args, PyObject *kwargs)
{
	CONNECTION_DEBUG("__init__");
	if(_py_midgard_connection_singleton_get()){

		/* Throw TypeError exception */
		PyErr_SetString(PyExc_TypeError, 
				"MidgardConnection singleton already initialized");
		return -1;
	}

	if (pygobject_constructv(self, 0, NULL))
		PyErr_SetString(PyExc_RuntimeError, "could not create object");

	if(self->obj != NULL) {

		/* set global */
		_py_midgard_connection_singleton_set(MIDGARD_CONNECTION(self->obj));
		/* get module object and set python object */
		PyObject *module = PyImport_AddModule("_midgard");	
		PyObject *d = PyModule_GetDict(module);
		PyDict_DelItemString(d, "_connection");	
		PyDict_SetItemString(d, "_connection", (PyObject *)self);
	}

	return (self->obj) ? 0 : -1;
}

static PyObject *
pymidgard_connection_open(PyGObject *self, PyObject *args)
{
	CONNECTION_DEBUG("open");
	gchar *name;
	
	if(!PyArg_ParseTuple(args, "s", &name))
		return NULL;


	GError *error = NULL;

	gboolean connected = 
		midgard_connection_open(
				MIDGARD_CONNECTION(self->obj),
				(const gchar *)name, &error);
	if(connected)
		Py_RETURN_TRUE;
	
	if(error) {
		
		PyErr_SetString(PyExc_SystemError, error->message);
		g_error_free(error);
		
		return NULL;
	}

	PyErr_SetString(PyExc_SystemError, "Unhandled midgard exception. FIXME!");
	return NULL;
}

static PyObject *
pymidgard_connection_open_config(PyGObject *self, PyObject *args)
{
	CONNECTION_DEBUG("open_config");
	PyObject *config;
		
	if(!PyArg_ParseTuple(args, "O", &config))
		return NULL;

	if(config == NULL)
		g_warning("config is NULL");

	PyGObject *_config = (PyGObject *)config;

	if (!MIDGARD_IS_CONFIG(_config->obj)) {
		
		g_warning("Expected midgard_config object argument");
		Py_RETURN_FALSE;
	}

	gboolean connected = 
		midgard_connection_open_config(
				MIDGARD_CONNECTION(self->obj),
				MIDGARD_CONFIG(_config->obj));
	if(connected)
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_connection_is_connected (PyGObject *self, PyObject *args)
{
	CONNECTION_DEBUG("is_connected");
		
	if (midgard_connection_is_connected (MIDGARD_CONNECTION(self->obj)))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_connection_set_loglevel(PyGObject *self, PyObject *args)
{
	CONNECTION_DEBUG("set_loglevel");
	const gchar *level;
	
	if(!PyArg_ParseTuple(args, "s", &level))
		return NULL;
	
	midgard_connection_set_loglevel(MIDGARD_CONNECTION(self->obj), 
			(const gchar *)level, NULL);
	
	Py_RETURN_NONE;
}

static PyObject *
pymidgard_connection_get_error(PyGObject *self, PyObject *args)
{
	CONNECTION_DEBUG("get_error");
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	gint errn = 
		midgard_connection_get_error(MIDGARD_CONNECTION(self->obj));

	return Py_BuildValue("i", (int)errn);
}

static PyObject *
pymidgard_connection_get_error_string(PyGObject *self, PyObject *args)
{
	CONNECTION_DEBUG("get_error_string");
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	const gchar *error_string = 
		midgard_connection_get_error_string(MIDGARD_CONNECTION(self->obj));

	return Py_BuildValue("s", error_string);
}

static PyObject *
pymidgard_connection_get_user(PyGObject *self, PyObject *args)
{
	CONNECTION_DEBUG("get_user");
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	MidgardUser *user = 
		midgard_connection_get_user(MIDGARD_CONNECTION(self->obj));

	if(user == NULL)
		Py_RETURN_NONE;
	
	GValue gval = {0, };
	g_value_init(&gval, G_TYPE_OBJECT);
	g_value_set_object(&gval, G_OBJECT(user));
	PyObject *pval = pyg_value_as_pyobject((const GValue *)&gval, FALSE);
	
	return pval;
}

static PyObject *
pymidgard_connection_list_auth_types(PyGObject *self, PyObject *args)
{
	CONNECTION_DEBUG("list_auth_types");

 	guint i = 0;
	guint n_types;
	gchar **types = midgard_connection_list_auth_types(MIDGARD_CONNECTION(self->obj), &n_types);

	PyObject *list = PyTuple_New(n_types);

	if (n_types == 0)
		return list;

	for (i = 0; i < n_types; i++) {

		PyObject *strname = PyString_FromString(types[i]);
		PyTuple_SetItem(list, i, strname);
	}

        g_free(types);

        return list;
}

static PyMethodDef pymidgard_connection_methods[] = {
	{ "open", (PyCFunction)pymidgard_connection_open, METH_VARARGS },
	{ "open_config", (PyCFunction)pymidgard_connection_open_config, METH_VARARGS },
	{ "is_connected", (PyCFunction)pymidgard_connection_is_connected, METH_NOARGS },
	{ "set_loglevel", (PyCFunction)pymidgard_connection_set_loglevel, METH_VARARGS },
	{ "get_error", (PyCFunction)pymidgard_connection_get_error, METH_VARARGS },
	{ "get_error_string", (PyCFunction)pymidgard_connection_get_error_string, METH_VARARGS },
	{ "get_user", (PyCFunction)pymidgard_connection_get_user, METH_VARARGS },
	{ "list_auth_types", (PyCFunction)pymidgard_connection_list_auth_types, METH_NOARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_connection_Type = {
	PyObject_HEAD_INIT(NULL)
	0,				/* ob_size */
	"connection",			/* tp_name */
	sizeof(PyGObject),          	/* tp_basicsize */
	0,                              /* tp_itemsize */
	/* methods */
	(destructor)_py_midgard_gobject_destructor,        		/* tp_dealloc */
	(printfunc)0,                   /* tp_print */
	(getattrfunc)0,       		/* tp_getattr */
	(setattrfunc)0,       		/* tp_setattr */
	(cmpfunc)0,           		/* tp_compare */
	(reprfunc)0,             	/* tp_repr */
	(PyNumberMethods*)0,     	/* tp_as_number */
	(PySequenceMethods*)0, 		/* tp_as_sequence */
	(PyMappingMethods*)0,   	/* tp_as_mapping */
	(hashfunc)0,             	/* tp_hash */
	(ternaryfunc)0,          	/* tp_call */
	(reprfunc)0,              	/* tp_str */
	_py_midgard_get_object_attribute, /* tp_getattro */
        _py_midgard_set_object_attribute, /* tp_setattro */
	(PyBufferProcs*)0,  		/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
	"MidgardConnection singleton class",		/* Documentation string */
	(traverseproc)0,     		/* tp_traverse */
	(inquiry)0,             	/* tp_clear */
	(richcmpfunc)0,   		/* tp_richcompare */
	offsetof(PyGObject, weakreflist),	/* tp_weaklistoffset */
	(getiterfunc)0,          	/* tp_iter */
	(iternextfunc)0,     		/* tp_iternext */
	pymidgard_connection_methods, 	/* tp_methods */ 
	(struct PyMemberDef*)0,         /* tp_members */
	(struct PyGetSetDef*)0,  	/* tp_getset */
	NULL,                           /* tp_base */
	NULL,                           /* tp_dict */
	(descrgetfunc)0,    		/* tp_descr_get */   
	(descrsetfunc)0,    		/* tp_descr_set */
	offsetof(PyGObject, inst_dict), 	/* tp_dictoffset */
	(initproc)__connection_singleton_init, 	/* tp_init */
	(allocfunc)0,           	/* tp_alloc */
	(newfunc)0,               	/* tp_new */
	(freefunc)0,             	/* tp_free */
	(inquiry)0              	/* tp_is_gc */
};

void py_midgard_connection_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"connection", 
			MIDGARD_TYPE_CONNECTION, 
			&Pymidgard_connection_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
