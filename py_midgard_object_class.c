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

static PyObject *
pymidgard_object_class_factory(PyGObject *self, PyObject *args) 
{
	PyObject *pvalue = NULL;
	const gchar *classname;
        // const gchar *guid;
        // guint id;
	MidgardObject *mobject = NULL; 

	if(!PyArg_ParseTuple(args, "s|O", &classname, &pvalue))
		return NULL;

	MidgardConnection *mgd =
		_py_midgard_connection_singleton_get();

	if(pvalue == NULL) {

		mobject = midgard_object_new(mgd, classname, NULL);

	} else {

		GValue gval = gvalue_from_pyobject(pvalue);
		pyg_value_from_pyobject(&gval, pvalue);
		
		if(G_IS_VALUE(&gval)) 
			mobject = midgard_object_new(mgd, classname, &gval);
			
		g_value_unset(&gval);
	}

	if(mobject == NULL)
		PyErr_SetString(PyExc_SystemError, "Failed to initialize new object");

	/* TODO , check if this might be optimized */
	GValue gval = {0, };
	g_value_init(&gval, G_TYPE_OBJECT);
	g_value_set_object(&gval, G_OBJECT(mobject));
	PyObject *pval = pyg_value_as_pyobject((const GValue *)&gval, FALSE);	
	
	return pval;
}
	
static PyObject *
pymidgard_object_class_get_primary_property(PyGObject *self, PyObject *args) 
{
	//CHECK_MGD;
	const gchar *classname;

	if(!PyArg_ParseTuple(args, "s", &classname))
		return NULL;
	
	GObjectClass *klass = g_type_class_peek(g_type_from_name(classname));
	if(klass == NULL)
		return NULL;

	const gchar *pname = 
		midgard_object_class_get_primary_property(MIDGARD_OBJECT_CLASS(klass));

	if(pname == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", pname); 
}

static PyObject *
pymidgard_object_class_get_property_parent(PyGObject *self, PyObject *args) 
{
	//CHECK_MGD;
	const gchar *classname;

	if(!PyArg_ParseTuple(args, "s", &classname))
		return NULL;
	
	GObjectClass *klass = g_type_class_peek(g_type_from_name(classname));
	if(klass == NULL)
		return NULL;

	const gchar *pname = 
		midgard_object_class_get_property_parent(MIDGARD_OBJECT_CLASS(klass));

	if(pname == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", pname); 
}

static PyObject *
pymidgard_object_class_get_property_up(PyGObject *self, PyObject *args) 
{
	//CHECK_MGD;
	const gchar *classname;

	if(!PyArg_ParseTuple(args, "s", &classname))
		return NULL;
	
	GObjectClass *klass = g_type_class_peek(g_type_from_name(classname));
	if(klass == NULL)
		return NULL;

	const gchar *pname = 
		midgard_object_class_get_property_up(MIDGARD_OBJECT_CLASS(klass));

	if(pname == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", pname); 
}

static PyObject *
pymidgard_object_class_list_children(PyGObject *self, PyObject *args) 
{
	//CHECK_MGD;
	const gchar *classname;

	if(!PyArg_ParseTuple(args, "s", &classname))
		return NULL;

	GObjectClass *klass = g_type_class_peek(g_type_from_name(classname));
	if(klass == NULL)
		return NULL;

	MidgardObjectClass **klases =
		midgard_object_class_list_children(MIDGARD_OBJECT_CLASS(klass));

	guint i = 0;
	if(klases == NULL) {

		PyObject *list = PyTuple_New(i);
		return list;
	}

	while(klases[i] != NULL)
		i++;
	
	PyObject *list = PyTuple_New(i);
	i = 0;
	while(klases[i] != NULL) {
	
		const gchar *cname = G_OBJECT_CLASS_NAME(klases[i]);
		PyObject *strname = PyString_FromString(cname);
		PyTuple_SetItem(list, i, strname);
		i++;
	}

	return list;
}

static PyObject *
pymidgard_object_class_get_object_by_guid(PyGObject *self, PyObject *args)
{
	const gchar *guid;
	
	if(!PyArg_ParseTuple(args, "s", &guid))
		return NULL;

	MidgardConnection *mgd =
		_py_midgard_connection_singleton_get();

	MidgardObject *mobject = midgard_object_class_get_object_by_guid(mgd, guid);

	/* TODO , check if this migh tbe optimized */
	GValue gval = {0, };
	g_value_init(&gval, G_TYPE_OBJECT);
	g_value_set_object(&gval, G_OBJECT(mobject));
	PyObject *pvalue = pyg_value_as_pyobject((const GValue *)&gval, FALSE);

	return pvalue;
}

static PyObject *
pymidgard_object_class_get_object_by_path(PyGObject *self, PyObject *args)
{
	const gchar *path;
	const gchar *classname;
	
	if(!PyArg_ParseTuple(args, "ss", &classname, &path))
		return NULL;

	MidgardConnection *mgd =
		_py_midgard_connection_singleton_get();

	MidgardObject *mobject = 
		midgard_object_class_get_object_by_path(mgd, classname, path);

	/* TODO , check if this might be optimized */
	GValue gval = {0, };
	g_value_init(&gval, G_TYPE_OBJECT);
	g_value_set_object(&gval, G_OBJECT(mobject));
	PyObject *pvalue = pyg_value_as_pyobject((const GValue *)&gval, FALSE);

	return pvalue;
}

static PyObject *
pymidgard_object_class_undelete(PyGObject *self, PyObject *args)
{	
	const gchar *guid;

	if(!PyArg_ParseTuple(args, "s", &guid))
		return NULL;

	MidgardConnection *mgd =
		_py_midgard_connection_singleton_get();

	if(midgard_object_class_undelete(mgd, guid))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_class_has_metadata(PyGObject *self, PyObject *args) 
{
	const gchar *classname;

	if (!PyArg_ParseTuple (args, "s", &classname))
		return NULL;
	
	GObjectClass *klass = g_type_class_peek (g_type_from_name (classname));
	if (klass == NULL)
		return NULL;

	if (midgard_object_class_has_metadata (MIDGARD_OBJECT_CLASS (klass)))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_class_get_schema_value (PyGObject *self, PyObject *args) 
{
	//CHECK_MGD;
	const gchar *classname;
	const gchar *name;

	if (!PyArg_ParseTuple (args, "ss", &classname, &name))
		return NULL;
	
	GObjectClass *klass = g_type_class_peek (g_type_from_name (classname));
	if (klass == NULL)
		return NULL;

	const gchar *value = 
		midgard_object_class_get_schema_value (MIDGARD_OBJECT_CLASS (klass), name);

	if (value == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue ("s", value); 
}

static PyMethodDef pymidgard_object_class_methods[] = {
	{ "factory",
		(PyCFunction)pymidgard_object_class_factory, METH_VARARGS | METH_STATIC},
	{ "get_primary_property", 
		(PyCFunction)pymidgard_object_class_get_primary_property, METH_VARARGS | METH_STATIC},
	{ "get_property_parent", 
		(PyCFunction)pymidgard_object_class_get_property_parent, METH_VARARGS | METH_STATIC},
	{ "get_property_up", 
		(PyCFunction)pymidgard_object_class_get_property_up, METH_VARARGS | METH_STATIC},
	{ "list_children",
		(PyCFunction)pymidgard_object_class_list_children, METH_VARARGS | METH_STATIC},
	{ "get_object_by_guid",
		(PyCFunction)pymidgard_object_class_get_object_by_guid, METH_VARARGS | METH_STATIC},
	{ "get_object_by_path",
		(PyCFunction)pymidgard_object_class_get_object_by_path, METH_VARARGS | METH_STATIC},
	{ "undelete",
		(PyCFunction)pymidgard_object_class_undelete, METH_VARARGS | METH_STATIC},
	{ "has_metadata",
		(PyCFunction)pymidgard_object_class_has_metadata, METH_VARARGS | METH_STATIC},
	{ "get_schema_value", 
		(PyCFunction)pymidgard_object_class_get_schema_value, METH_VARARGS | METH_STATIC},
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_object_class_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "object_class",                   /* tp_name */
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
    pymidgard_object_class_methods, /* tp_methods */
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

void py_midgard_object_class_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"object_class", 
			MIDGARD_TYPE_OBJECT, 
			&Pymidgard_object_class_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
