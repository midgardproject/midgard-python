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

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_constraint_simple_Type;
PyTypeObject G_GNUC_INTERNAL Pymidgard_query_constraint_Type;
PyTypeObject G_GNUC_INTERNAL Pymidgard_query_constraint_group_Type;

/* QUERY CONSTRAINT SIMPLE */

static PyObject *
pymidgard_query_constraint_simple_list_constraints (PyGObject *self, PyObject *args)
{
	guint n_objects;	
	MidgardQueryConstraintSimple *mqcs = MIDGARD_QUERY_CONSTRAINT_SIMPLE (self->obj);
	MidgardQueryConstraintSimple **constraints = midgard_query_constraint_simple_list_constraints (mqcs, &n_objects);

	PyObject *list = PyTuple_New(n_objects);

        if(!constraints)
		return list;
	                
        OBJECTS2LIST(constraints, list);
        g_free(constraints);

        return list;
}

static PyMethodDef pymidgard_query_constraint_simple_methods[] = {
	{ "list_constraints", (PyCFunction) pymidgard_query_constraint_simple_list_constraints, METH_NOARGS},
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_constraint_simple_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "midgard.query.constraint_simple",                   /* tp_name */
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
    pymidgard_query_constraint_simple_methods, /* tp_methods */
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
__MidgardQueryConstraintSimple__interface_init (MidgardQueryConstraintSimpleIFace *iface, PyTypeObject *pytype)
{

}

static const GInterfaceInfo __MidgardQueryConstraintSimple__iinfo = {
	(GInterfaceInitFunc)__MidgardQueryConstraintSimple__interface_init,
	NULL,
	NULL
};

void 
py_midgard_query_constraint_simple_register_interface (PyObject *d)
{
	pyg_register_interface (d, "constraint_simple", 
			MIDGARD_TYPE_QUERY_CONSTRAINT_SIMPLE, 
			&Pymidgard_query_constraint_simple_Type);

	//pyg_register_interface_info (MIDGARD_TYPE_QUERY_CONSTRAINT_SIMPLE, &__MidgardQueryConstraintSimple__iinfo);
}


/* QUERY CONSTRAINT */

static int
__constraint_constructor (PyGObject *self, PyObject *args, PyObject *kwargs)
{
	PyObject *p_property = NULL;
	gchar *p_op = NULL;
	PyObject *p_holder = NULL;
	PyObject *p_storage = NULL;
	if(!PyArg_ParseTuple (args, "OsO|O", &p_property, &p_op, &p_holder, &p_storage))
		return -1;

	MidgardQueryProperty *property = MIDGARD_QUERY_PROPERTY (((PyGObject *) p_property)->obj);
	MidgardQueryHolder *holder = MIDGARD_QUERY_HOLDER (((PyGObject *) p_holder)->obj);
	MidgardQueryStorage *storage = NULL; 
	if (p_storage)
		storage = MIDGARD_QUERY_STORAGE (((PyGObject *) p_storage)->obj);

	MidgardQueryConstraint *constraint = midgard_query_constraint_new (property, p_op, holder, storage);

	if (!constraint)
		return -1;

	self->obj = G_OBJECT (constraint);

	return 0;
}

static PyObject *
pymidgard_query_constraint_get_storage (PyGObject *self, PyObject *args)
{
	MidgardQueryConstraint *constraint = MIDGARD_QUERY_CONSTRAINT (self->obj);

	MidgardQueryStorage *storage = midgard_query_constraint_get_storage (constraint);
	if (!storage)
		Py_RETURN_NONE;

	return Py_BuildValue ("O", pygobject_new (G_OBJECT (storage)));
}

static PyObject *
pymidgard_query_constraint_set_storage (PyGObject *self, PyObject *args)
{
	PyObject *pobject;
	if(!PyArg_ParseTuple(args, "O", &pobject))
		return NULL;

	MidgardQueryConstraint *constraint = MIDGARD_QUERY_CONSTRAINT (self->obj);
	MidgardQueryStorage *storage = MIDGARD_QUERY_STORAGE (((PyGObject *)pobject)->obj);

	if (!storage)
		Py_RETURN_FALSE;

	if (midgard_query_constraint_set_storage (constraint, storage))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_query_constraint_get_property (PyGObject *self, PyObject *args)
{
	MidgardQueryConstraint *constraint = MIDGARD_QUERY_CONSTRAINT (self->obj);

	MidgardQueryProperty *property = midgard_query_constraint_get_property (constraint);
	if (!property)
		Py_RETURN_NONE;

	return Py_BuildValue ("O", pygobject_new (G_OBJECT (property)));
}

static PyObject *
pymidgard_query_constraint_set_property (PyGObject *self, PyObject *args)
{
	PyObject *pobject;
	if(!PyArg_ParseTuple(args, "O", &pobject))
		return NULL;

	MidgardQueryConstraint *constraint = MIDGARD_QUERY_CONSTRAINT (self->obj);
	MidgardQueryProperty *property = MIDGARD_QUERY_PROPERTY (((PyGObject *)pobject)->obj);

	if (!property)
		Py_RETURN_FALSE;

	if (midgard_query_constraint_set_property (constraint, property))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_query_constraint_get_operator (PyGObject *self, PyObject *args)
{
	MidgardQueryConstraint *constraint = MIDGARD_QUERY_CONSTRAINT (self->obj);

	const gchar *operator = midgard_query_constraint_get_operator (constraint);
	if (!operator)
		Py_RETURN_NONE;

	return Py_BuildValue ("s", operator);
}

static PyObject *
pymidgard_query_constraint_set_operator (PyGObject *self, PyObject *args)
{
	gchar *p_op = NULL;
	if(!PyArg_ParseTuple(args, "s", &p_op))
		return NULL;

	MidgardQueryConstraint *constraint = MIDGARD_QUERY_CONSTRAINT (self->obj);

	if (midgard_query_constraint_set_operator (constraint, p_op))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyMethodDef pymidgard_query_constraint_methods[] = {
	{ "get_storage", (PyCFunction)pymidgard_query_constraint_get_storage, METH_NOARGS},
	{ "set_storage", (PyCFunction)pymidgard_query_constraint_set_storage, METH_VARARGS },
	{ "get_property", (PyCFunction)pymidgard_query_constraint_get_property, METH_NOARGS},
	{ "set_property", (PyCFunction)pymidgard_query_constraint_set_property, METH_VARARGS },
	{ "get_operator", (PyCFunction)pymidgard_query_constraint_get_operator, METH_NOARGS},
	{ "set_operator", (PyCFunction)pymidgard_query_constraint_set_operator, METH_VARARGS },

	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_constraint_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "constraint",                   /* tp_name */
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
    pymidgard_query_constraint_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__constraint_constructor,              /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void 
py_midgard_query_constraint_register_class (PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, "constraint", 
			MIDGARD_TYPE_QUERY_CONSTRAINT, 
			&Pymidgard_query_constraint_Type,
			Py_BuildValue("(OO)", pygobject_type, &Pymidgard_query_constraint_simple_Type));
}

/* QUERY CONSTRAINT GROUP */

static int
__constraint_group_constructor (PyGObject *self, PyObject *args, PyObject *kwargs)
{
	MidgardQueryConstraintGroup *constraint_group = midgard_query_constraint_group_new ();

	if (!constraint_group)
		return -1;

	self->obj = G_OBJECT (constraint_group);

	return 0;
}

MidgardQueryConstraintSimple **
_py_midgard_constraints_from_args (PyObject *args, guint *n_params)
{
	if(!args) {

		*n_params = 0;
		return NULL;
	}

	gint i = 0;
	
	*n_params = (guint) PyDict_Size (args);

	if(*n_params < 1)
		return NULL;

#if PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION == 4
	int pos = 0;
#elif PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION > 4
	Py_ssize_t pos = 0;
#endif
	PyObject *key;
	PyObject *value;

	MidgardQueryConstraintSimple **constraints = g_new (MidgardQueryConstraintSimple*, *n_params);

	while (PyDict_Next (args, &pos, &key, &value)) {

		constraints[i] = MIDGARD_QUERY_CONSTRAINT_SIMPLE (((PyGObject *) value)->obj);		
		i++;
	}

	return constraints;
}

static PyObject *
pymidgard_query_constraint_group_new_with_constraints (PyGObject *self, PyObject *args)
{
	gchar *p_type = NULL;
	PyObject *p_constraints = NULL;
	if(!PyArg_ParseTuple (args, "sO", &p_type, &p_constraints))
		return NULL;

	guint n_constraints;
	MidgardQueryConstraintSimple **constraints = _py_midgard_constraints_from_args (p_constraints, &n_constraints);
	MidgardQueryConstraintGroup *constraint_group = 
		midgard_query_constraint_group_new_with_constraints (p_type, constraints, n_constraints);

	g_free (constraints);

	if (!constraint_group)
		Py_RETURN_NONE;

	return Py_BuildValue ("O", pygobject_new (G_OBJECT (constraint_group)));
}

static PyObject *
pymidgard_query_constraint_group_add_constraint (PyGObject *self, PyObject *args)
{
	PyObject *p_constraints = NULL;
	if(!PyArg_ParseTuple (args, "O", &p_constraints))
		return NULL;

	guint n_constraints;
	MidgardQueryConstraintSimple **constraints = _py_midgard_constraints_from_args (p_constraints, &n_constraints);

	MidgardQueryConstraintGroup *constraint_group = MIDGARD_QUERY_CONSTRAINT_GROUP (self->obj);

	guint i;
	gboolean ret = FALSE;
	for (i = 0; i < n_constraints; i++) {
		if (!midgard_query_constraint_group_add_constraint (constraint_group, constraints[i], NULL))
			break;
	}

	g_free (constraints);
		
	if (ret)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_query_constraint_group_get_type (PyGObject *self, PyObject *args)
{
	MidgardQueryConstraintGroup *constraint_group = MIDGARD_QUERY_CONSTRAINT_GROUP (self->obj);

	const gchar *type = midgard_query_constraint_group_get_group_type (constraint_group);
	if (!type)
		Py_RETURN_NONE;

	return Py_BuildValue ("s", type);
}

static PyObject *
pymidgard_query_constraint_group_set_type (PyGObject *self, PyObject *args)
{
	gchar *type;
	if(!PyArg_ParseTuple (args, "s", &type))
		return NULL;

	MidgardQueryConstraintGroup *constraint_group = MIDGARD_QUERY_CONSTRAINT_GROUP (self->obj);

	if (midgard_query_constraint_group_set_group_type (constraint_group, type))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyMethodDef pymidgard_query_constraint_group_methods[] = {
	{ "new_with_constraints", (PyCFunction)pymidgard_query_constraint_group_new_with_constraints, METH_VARARGS | METH_STATIC},
	{ "set_type", (PyCFunction)pymidgard_query_constraint_group_set_type, METH_VARARGS },
	{ "get_type", (PyCFunction)pymidgard_query_constraint_group_get_type, METH_NOARGS},
	{ "add_constraint", (PyCFunction)pymidgard_query_constraint_group_add_constraint, METH_VARARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_query_constraint_group_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "constraint_group",                   /* tp_name */
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
    pymidgard_query_constraint_group_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__constraint_group_constructor,              /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void 
py_midgard_query_constraint_group_register_class (PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class (d, "constraint_group", 
			MIDGARD_TYPE_QUERY_CONSTRAINT_GROUP, 
			&Pymidgard_query_constraint_group_Type, 
			Py_BuildValue ("(OO)", pygobject_type, &Pymidgard_query_constraint_simple_Type));
}

/* COMMON */

void
py_midgard_query_constraint_register_classes (PyObject *d, gpointer pygobject_type)
{
	py_midgard_query_constraint_simple_register_interface (d);
	py_midgard_query_constraint_register_class (d, pygobject_type);
	py_midgard_query_constraint_group_register_class (d, pygobject_type);
}

