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

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_holder_Type;
PyTypeObject G_GNUC_INTERNAL Pymidgard_query_value_Type;
PyTypeObject G_GNUC_INTERNAL Pymidgard_query_property_Type;

/* QUERY HOLDER */

static PyObject *
pymidgard_query_holder_set_value (PyGObject *self, PyObject *args)
{
	PyObject *pvalue;
	if(!PyArg_ParseTuple(args, "O", &pvalue))
		return NULL;

	GValue gval = gvalue_from_pyobject(pvalue);
	py_midgard_gvalue_from_pyobject(&gval, pvalue);

	MidgardQueryHolder *holder = MIDGARD_QUERY_HOLDER (self->obj);

	if (midgard_query_holder_set_value (holder, (const GValue *) &gval))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_query_holder_get_value (PyGObject *self, PyObject *args)
{
	MidgardQueryHolder *holder = MIDGARD_QUERY_HOLDER (self->obj);
	GValue val = {0, };

	midgard_query_holder_get_value (holder, &val);
	PyObject *pvalue = pyg_value_as_pyobject((const GValue *)&val, FALSE);

	return pvalue;
}

static PyMethodDef pymidgard_query_holder_methods[] = {
	{ "set_value", (PyCFunction) pymidgard_query_holder_set_value, METH_VARARGS},
	{ "get_value", (PyCFunction) pymidgard_query_holder_get_value, METH_VARARGS},
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_holder_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "midgard.query.holder",                   /* tp_name */
    sizeof(PyObject),          /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)0,        /* tp_dealloc */
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
    0, /* tp_getattro */
    0, /* tp_setattro */
    (PyBufferProcs*)0,  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                      /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)0,     /* tp_traverse */
    (inquiry)0,             /* tp_clear */
    (richcmpfunc)0,   /* tp_richcompare */
    0,             /* tp_weaklistoffset */
    (getiterfunc)0,          /* tp_iter */
    (iternextfunc)0,     /* tp_iternext */
    pymidgard_query_holder_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    0,                 /* tp_dictoffset */
    (initproc)0,              /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

static void
__MidgardQueryHolder__interface_init (MidgardQueryHolderIFace *iface, PyTypeObject *pytype)
{

}

static const GInterfaceInfo __MidgardQueryHolder__iinfo = {
	(GInterfaceInitFunc)__MidgardQueryHolder__interface_init,
	NULL,
	NULL
};

void 
py_midgard_query_holder_register_interface (PyObject *d)
{
	pyg_register_interface (d, "holder", 
			MIDGARD_TYPE_QUERY_HOLDER, 
			&Pymidgard_query_holder_Type);

}

/* QUERY CONSTRAINT */

static int
__qvalue_constructor (PyGObject *self, PyObject *args, PyObject *kwargs)
{
	PyObject *pvalue;
	if(!PyArg_ParseTuple(args, "O", &pvalue))
		return -1;

	GValue gval = gvalue_from_pyobject(pvalue);
	py_midgard_gvalue_from_pyobject(&gval, pvalue);
	MidgardQueryValue *qvalue = midgard_query_value_new ((const GValue *)&gval);

	if (!qvalue)
		return -1;

	self->obj = G_OBJECT (qvalue);

	return 0;
}

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_value_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "value",                   /* tp_name */
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
    0, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__qvalue_constructor,              /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void 
py_midgard_query_value_register_class (PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, "value", 
			MIDGARD_TYPE_QUERY_VALUE, 
			&Pymidgard_query_value_Type,
			Py_BuildValue("(OO)", pygobject_type, &Pymidgard_query_value_Type));
}

/* QUERY PROPERTY */

static int
__qproperty_constructor (PyGObject *self, PyObject *args, PyObject *kwargs)
{
	gchar *property;
	PyObject *pystorage = NULL;
	if(!PyArg_ParseTuple(args, "s|O", &property, &pystorage))
		return -1;

	MidgardQueryStorage *storage = MIDGARD_QUERY_STORAGE (((PyGObject*) (pystorage))->obj);

	MidgardQueryProperty *qproperty = midgard_query_property_new (property, storage);
	if (!qproperty)
		return -1;

	self->obj = G_OBJECT (qproperty);

	return 0;
}

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_property_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "property",                   /* tp_name */
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
    0, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__qproperty_constructor,              /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void 
py_midgard_query_property_register_class (PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class (d, "property", 
			MIDGARD_TYPE_QUERY_PROPERTY, 
			&Pymidgard_query_property_Type, 
			Py_BuildValue ("(OO)", pygobject_type, &Pymidgard_query_holder_Type));
}

/* COMMON */

void
py_midgard_query_holder_register_classes (PyObject *d, gpointer pygobject_type)
{
	py_midgard_query_holder_register_interface (d);
	py_midgard_query_value_register_class (d, pygobject_type);
	py_midgard_query_property_register_class (d, pygobject_type);
}
