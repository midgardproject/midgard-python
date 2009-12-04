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

PyTypeObject G_GNUC_INTERNAL Pymidgard_blob_Type;

#define BLOB_DEBUG(__name) \
	CHECK_MGD; \
	CLASS_METHOD_DEBUG(Pymidgard_blob_Type.tp_name, __name);

#define __GET_BLOB	\
	MidgardBlob *blob = MIDGARD_BLOB(self->obj);	\
	g_assert(blob != NULL); 

static int
__blob_constructor(PyGObject *self, PyObject *args, PyObject *kwargs)
{
	BLOB_DEBUG("__init__");
	PyObject *pvalue = NULL;
	const gchar *encoding = NULL;
	PyTypeObject *pytype =
		py_midgard_lookup_schema_type("midgard_attachment");
	if(!PyArg_ParseTuple(args,"O!|s:__init__", pytype, &pvalue, &encoding))
		return -1;
	
	MidgardObject *att = MIDGARD_OBJECT(((PyGObject *)pvalue)->obj);
	MidgardBlob *blob = midgard_blob_new(att, encoding);
		
	if(!blob) 
		return -1;
	
	self->obj = G_OBJECT(blob);
	
	return 0;
}

static PyObject *
pymidgard_blob_read_content(PyGObject *self, PyObject *args)
{
	BLOB_DEBUG("read_content");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	__GET_BLOB;

	gsize bytes_read = 0;
	gchar *content = midgard_blob_read_content(blob, &bytes_read);

	PyObject *ret = Py_BuildValue("s", content);
	g_free(content);
	
	return ret;
}

static PyObject *
pymidgard_blob_write_content(PyGObject *self, PyObject *args)
{
	BLOB_DEBUG("write_content");
	const gchar *content = NULL;
	gboolean written = FALSE;

	if(!PyArg_ParseTuple(args, "s", &content))
		return NULL;
	
	__GET_BLOB;

	written = midgard_blob_write_content(blob, content);

	if(written)
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_blob_remove_file(PyGObject *self, PyObject *args)
{
	BLOB_DEBUG("remove_file");
	gboolean removed = FALSE;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	__GET_BLOB;

	removed = midgard_blob_remove_file(blob);

	if(removed)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_blob_get_path(PyGObject *self, PyObject *args)
{
	BLOB_DEBUG("get_path");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	__GET_BLOB;
	
	const gchar *path = midgard_blob_get_path(blob);
	
	PyObject *ret = Py_BuildValue("s", path);	
	
	return ret;
}

static PyObject *
pymidgard_blob_get_handler(PyGObject *self, PyObject *args)
{
	BLOB_DEBUG("get_handler");
	const gchar *mode = "w+";

	if(!PyArg_ParseTuple(args, "|s", &mode))
		return NULL;

	__GET_BLOB;

	const gchar *path = midgard_blob_get_path(blob);

	PyObject *fh = PyFile_FromString((gchar *)path, (gchar *)mode);

	return fh;
}

static PyObject *
pymidgard_blob_exists(PyGObject *self, PyObject *args)
{
	BLOB_DEBUG("exists");
	gboolean exists = FALSE;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	__GET_BLOB;
	
	exists = midgard_blob_exists(blob);

	if(exists)
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyMethodDef pymidgard_blob_methods[] = {
	{ "read_content", (PyCFunction)pymidgard_blob_read_content, METH_VARARGS },
	{ "write_content", (PyCFunction)pymidgard_blob_write_content, METH_VARARGS },
	{ "remove_file", (PyCFunction)pymidgard_blob_remove_file, METH_VARARGS },
	{ "get_path", (PyCFunction)pymidgard_blob_get_path, METH_VARARGS },
	{ "get_handler", (PyCFunction)pymidgard_blob_get_handler, METH_VARARGS },
	{ "exists", (PyCFunction)pymidgard_blob_exists, METH_VARARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_blob_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "blob",                   /* tp_name */
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
    pymidgard_blob_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__blob_constructor,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void py_midgard_blob_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"blob", 
			MIDGARD_TYPE_BLOB, 
			&Pymidgard_blob_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
