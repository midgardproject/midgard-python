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

PyTypeObject G_GNUC_INTERNAL Pymidgard_config_Type;

#define CONFIG_DEBUG(__name) \
	CHECK_MGD; \
        CLASS_METHOD_DEBUG(Pymidgard_config_Type.tp_name, __name);

typedef struct {
	PyObject_HEAD
	MidgardConfig *mgdcfg;
} PyMidgardConfig;

static PyObject *
pymidgard_config_read_file(PyGObject *self, PyObject *args) 
{
	CONFIG_DEBUG("read_file");
	gchar *name;
	gint user = 0;
	
	if(!PyArg_ParseTuple(args, "s|i", &name, &user))
		return NULL;

	GError *error = NULL;

	if(midgard_config_read_file(MIDGARD_CONFIG(self->obj), 
				(const gchar *)name, user, &error))
		Py_RETURN_TRUE;	

	if(error) {
		
		PyErr_SetString(PyExc_SystemError, error->message);
		g_error_free(error);
	}

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_config_read_file_at_path(PyGObject *self, PyObject *args)
{
	CONFIG_DEBUG("read_file_at_path");
	gchar *path;
	
	if(!PyArg_ParseTuple(args, "s", &path))
		return NULL;

	GError *error = NULL;

	if(midgard_config_read_file_at_path(MIDGARD_CONFIG(self->obj), 
				(const gchar *)path, &error))
		Py_RETURN_TRUE;	

	if(error) {
		
		PyErr_SetString(PyExc_SystemError, error->message);
		g_error_free(error);
	}

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_config_read_data(PyGObject *self, PyObject *args)
{
	CONFIG_DEBUG("read_data");
	gchar *data;
	
	if(!PyArg_ParseTuple(args, "s", &data))
		return NULL;

	GError *error = NULL;

	if(midgard_config_read_data(MIDGARD_CONFIG(self->obj), 
				(const gchar *)data, &error))
		Py_RETURN_TRUE;	

	if(error) {
		
		PyErr_SetString(PyExc_SystemError, error->message);
		g_error_free(error);
	}

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_config_save_file(PyGObject *self, PyObject *args) 
{
	CONFIG_DEBUG("save_file");
	gchar *name;
	gint user = 1;
	
	if(!PyArg_ParseTuple(args, "s|i", &name, &user))
		return NULL;

	GError *error = NULL;
	if(midgard_config_save_file(MIDGARD_CONFIG(self->obj),
				(const gchar *)name, user, &error))     
		Py_RETURN_TRUE;

	if(error) {
		
		PyErr_SetString(PyExc_SystemError, error->message);
		g_error_free(error);
	}
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_config_list_files(PyGObject *self, PyObject *args) 
{
	CONFIG_DEBUG("list_files");

	gint user = 1;

	if(!PyArg_ParseTuple(args, "|i", &user))
		return NULL;

	gchar **names = midgard_config_list_files(user);

	if(!names)
		return NULL;

	guint i = 0;
	while(names[i] != NULL)
		i++;

	PyObject *list = PyTuple_New(i); 

	i = 0;
	while(names[i] != NULL) {
	
		PyObject *strname = PyString_FromString(names[i]);
		PyTuple_SetItem(list, i, strname);
		i++;
	}
	
	g_strfreev(names); /* Free names array, we have copy in list's tuple */

	return list;
}

static PyObject *
pymidgard_config_create_blobdir(PyGObject *self, PyObject *args) 
{
	CONFIG_DEBUG("create_blobdir");

	if (midgard_config_create_blobdir(MIDGARD_CONFIG(self->obj)))
	{
		Py_RETURN_TRUE;
	}

	Py_RETURN_FALSE;
}

static PyMethodDef pymidgard_config_methods[] = {
	{ "read_file", (PyCFunction)pymidgard_config_read_file, METH_VARARGS },
	{ "read_file_at_path", (PyCFunction)pymidgard_config_read_file_at_path, METH_VARARGS },
	{ "read_data", (PyCFunction)pymidgard_config_read_data, METH_VARARGS },
	{ "list_files", (PyCFunction)pymidgard_config_list_files, METH_VARARGS },
	{ "save_file", (PyCFunction)pymidgard_config_save_file, METH_VARARGS },
	{ "create_blobdir", (PyCFunction)pymidgard_config_create_blobdir, METH_NOARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_config_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "config",                   /* tp_name */
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
    _py_midgard_get_object_attribute, /* tp_getattro */
    _py_midgard_set_object_attribute, /* tp_setattro */
    (PyBufferProcs*)0,  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                      /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)0,     /* tp_traverse */
    (inquiry)0,             /* tp_clear */
    (richcmpfunc)0,   /* tp_richcompare */
    offsetof(PyGObject, weakreflist),             /* tp_weaklistoffset */
    (getiterfunc)0,          /* tp_iter */
    (iternextfunc)0,     /* tp_iternext */
    pymidgard_config_methods, /* tp_methods */
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

void py_midgard_config_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"config", 
			MIDGARD_TYPE_CONFIG, 
			&Pymidgard_config_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
