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
 * along with this program; if not-> write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define NO_IMPORT_PYGOBJECT
#include "py_midgard.h"

PyTypeObject G_GNUC_INTERNAL Pymidgard_view_Type;

PyTypeObject G_GNUC_INTERNAL Pymidgard_view_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "view",                   /* tp_name */
    sizeof(PyGObject),          /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)0,        /* tp_dealloc */
    (printfunc)0,                      /* tp_print */
    (getattrfunc)_py_midgard_get_object_attribute,       /* tp_getattr */
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
    (struct PyMethodDef*)NULL, /* tp_methods */
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

void py_midgard_base_view_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"view", 
			MIDGARD_TYPE_VIEW, 
			&Pymidgard_view_Type, 
			Py_BuildValue("(O)", pygobject_type));
}

PyTypeObject *__new_view_struct(const gchar *typename)
{
	/* initialize object type */
	PyTypeObject *ot = g_new0(PyTypeObject, 1);
	ot->ob_refcnt = 1;
	ot->ob_type = NULL;
	ot->tp_name = g_strdup(typename);
	ot->ob_size = 0;
	ot->tp_basicsize = sizeof(PyGObject);
	ot->tp_itemsize = 0;
	ot->tp_dealloc = 0;
	ot->tp_print = 0;
	ot->tp_getattr = 0;
	ot->tp_setattr = 0;
	ot->tp_compare = 0;
	ot->tp_repr = 0;
	ot->tp_as_number = 0;
	ot->tp_as_sequence = 0;
	ot->tp_as_mapping = 0;
	ot->tp_hash = 0;
	ot->tp_call = 0;
	ot->tp_str = 0;
	//ot->tp_getattro = 0;
	ot->tp_getattro = _py_midgard_get_object_attribute;
	ot->tp_setattro = _py_midgard_set_object_attribute;
	ot->tp_as_buffer = 0;
	ot->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
	ot->tp_doc = "User defined view class";
	ot->tp_traverse = 0;
	ot->tp_clear = 0;
	ot->tp_richcompare = 0;
	ot->tp_weaklistoffset = offsetof(PyGObject, weakreflist);
	ot->tp_iter = 0;
	ot->tp_iternext = 0;	
	ot->tp_methods = 0;
	//ot->tp_methods = NULL;
	ot->tp_members = 0;
	ot->tp_getset = 0;
	ot->tp_base = NULL;
	ot->tp_dict = NULL;
	ot->tp_descr_get = 0;
	ot->tp_descr_set = 0;
	ot->tp_dictoffset = offsetof(PyGObject, inst_dict);
	ot->tp_init = 0;		
	ot->tp_alloc = 0;
	ot->tp_new = 0;
	ot->tp_free = 0;
	ot->tp_is_gc = 0;

	ot->tp_bases = 0;
	ot->tp_mro = 0; /* method resolution order */
	ot->tp_cache = 0;
	ot->tp_subclasses = 0;
	ot->tp_weaklist = 0;
	ot->tp_del = 0;
	
	return ot;
}

void py_midgard_view_register_class(
		PyObject *d, gpointer pygobject_type)
{
	py_midgard_base_view_register_class(d, pygobject_type);

	guint n_types, i;
	const gchar *typename;
	GType *all_types = g_type_children(MIDGARD_TYPE_VIEW, &n_types);

	for (i = 0; i < n_types; i++) {

		typename = g_type_name(all_types[i]);

		PyTypeObject *ot = __new_view_struct(typename);
		pygobject_register_class(d,
				typename, all_types[i], ot,  
				/* FIXME, What is a static bases in this case? */
				Py_BuildValue("(O)", pygobject_type)); 
	}

	g_free(all_types);
}
