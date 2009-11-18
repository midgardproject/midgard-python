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

PyTypeObject G_GNUC_INTERNAL Pymidgard_key_config_file_context_Type;

#define KCC_DEBUG(__name) \
	CLASS_METHOD_DEBUG(Pymidgard_key_config_file_context_Type.tp_name, __name);

static int
__key_config_file_context_constructor (PyGObject *self, PyObject *args, PyObject *kwargs)
{
	KCC_DEBUG ("__init__");
	const gchar *path;
	
	if (!PyArg_ParseTuple (args, "s", &path))
		return -1;

	GError *error = NULL;	
	MidgardKeyConfigFileContext *mccfk = midgard_key_config_file_context_new (path, &error);

	if (!mccfk)
		return -1;
	
	self->obj = G_OBJECT (mccfk);
	
	return 0;
}

PyTypeObject G_GNUC_INTERNAL Pymidgard_key_config_file_context_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "key_config_file_context",                   /* tp_name */
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
    "MidgardKeyConfigFileContext class",                        /* Documentation string */
    (traverseproc)0,     /* tp_traverse */
    (inquiry)0,             /* tp_clear */
    (richcmpfunc)0,   /* tp_richcompare */
    offsetof(PyGObject, weakreflist),             /* tp_weaklistoffset */
    (getiterfunc)0,          /* tp_iter */
    (iternextfunc)0,     /* tp_iternext */
    0, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__key_config_file_context_constructor,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void py_midgard_key_config_file_context_register_class(
		PyObject *d, gpointer ot)
{
	pygobject_register_class(d, 
			"key_config_file_context", 
			MIDGARD_TYPE_KEY_CONFIG_FILE_CONTEXT, 
			&Pymidgard_key_config_file_context_Type, NULL);
			//Py_BuildValue("(O)", ot));

}

PyTypeObject py_midgard_key_config_file_context_get_type_object(void)
{
	return Pymidgard_key_config_file_context_Type;
}

