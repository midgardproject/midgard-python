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

PyTypeObject G_GNUC_INTERNAL Pymidgard_qb_Type;

#define QB_DEBUG(__name) \
	CHECK_MGD; \
	CLASS_METHOD_DEBUG(Pymidgard_qb_Type.tp_name, __name);

typedef struct {
	PyObject_HEAD
	MidgardConfig *mgdcfg;
} PyMidgardQueryBuilder;

static int
__query_builder_constructor(PyGObject *self, PyObject *args, PyObject *kwargs)
{
	QB_DEBUG("__init__");
	const gchar *classname;
	if(!PyArg_ParseTuple(args, "s", &classname))
                return -1;

	MidgardConnection *mgd =
		_py_midgard_connection_singleton_get();

	MidgardQueryBuilder *builder = 
		midgard_query_builder_new(mgd, classname);
	
	if(!builder)
		return -1;

	self->obj = G_OBJECT(builder);	

	return 0;
}

static PyObject *
pymidgard_qb_add_constraint(PyGObject *self, PyObject *args)
{
	QB_DEBUG("add_constraint");	
	const gchar *prop, *op;
	PyObject *pvalue;
	gboolean added;
	if(!PyArg_ParseTuple(args, "ssO", &prop, &op, &pvalue))
		return NULL;

	MidgardQueryBuilder *builder = 
		MIDGARD_QUERY_BUILDER(self->obj);

	GValue gval = gvalue_from_pyobject(pvalue);	
	py_midgard_gvalue_from_pyobject(&gval, pvalue);

	added = midgard_query_builder_add_constraint(builder, prop, op, &gval);

	if(added)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_qb_add_constraint_with_property(PyGObject *self, PyObject *args)
{
	QB_DEBUG("add_constraint_with_property");	
	const gchar *prop_a, *op, *prop_b;
        // PyObject *pvalue;
	gboolean added;
	if(!PyArg_ParseTuple(args, "sss", &prop_a, &op, &prop_b))
		return NULL;

	MidgardQueryBuilder *builder = 
		MIDGARD_QUERY_BUILDER(self->obj);

	added = midgard_query_builder_add_constraint_with_property(
			builder, prop_a, op, prop_b);

	if(added)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_qb_begin_group(PyGObject *self, PyObject *args)
{
	QB_DEBUG("begin_group");
	const gchar *group_op;
	if(!PyArg_ParseTuple(args, "s", &group_op))
		return NULL;

	MidgardQueryBuilder *builder = 
		MIDGARD_QUERY_BUILDER(self->obj);

	if(midgard_query_builder_begin_group(builder, group_op))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_qb_end_group(PyGObject *self, PyObject *args)
{
	QB_DEBUG("end_group");
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardQueryBuilder *builder = 
		MIDGARD_QUERY_BUILDER(self->obj);
	
	if(midgard_query_builder_end_group(builder))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_qb_add_order(PyGObject *self, PyObject *args)
{
	QB_DEBUG("add_order");
	
	const gchar *prop, *order;
	if(!PyArg_ParseTuple(args, "ss", &prop, &order))
		return NULL;

	MidgardQueryBuilder *builder = 
		MIDGARD_QUERY_BUILDER(self->obj);

	if(midgard_query_builder_add_order(builder, prop, order))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_qb_set_offset(PyGObject *self, PyObject *args)
{
	QB_DEBUG("set_offset");
	
	guint offset;
	if(!PyArg_ParseTuple(args, "i", &offset))
		return NULL;

	MidgardQueryBuilder *builder = 
		MIDGARD_QUERY_BUILDER(self->obj);

	midgard_query_builder_set_offset(builder, offset);
	
	Py_RETURN_NONE;	
}

static PyObject *
pymidgard_qb_set_limit(PyGObject *self, PyObject *args)
{
	QB_DEBUG("set_limit");
	
	long limit;
	if(!PyArg_ParseTuple(args, "l", &limit))
		return NULL;

	MidgardQueryBuilder *builder = 
		MIDGARD_QUERY_BUILDER(self->obj);

	midgard_query_builder_set_limit(builder, limit);
	
	Py_RETURN_NONE;
}

static PyObject *
pymidgard_qb_count(PyGObject *self, PyObject *args)
{
	QB_DEBUG("count");
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardQueryBuilder *builder = 
		MIDGARD_QUERY_BUILDER(self->obj);

	guint counted = midgard_query_builder_count(builder);

	return Py_BuildValue("i", counted);
}

static PyObject *
pymidgard_qb_include_deleted(PyGObject *self, PyObject *args)
{
	QB_DEBUG("include_deleted");
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardQueryBuilder *builder = 
		MIDGARD_QUERY_BUILDER(self->obj);

	midgard_query_builder_include_deleted(builder);
	
	Py_RETURN_NONE;	
}

static PyObject *
pymidgard_qb_execute(PyGObject *self, PyObject *args)
{
	QB_DEBUG("execute");
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardQueryBuilder *builder = 
		MIDGARD_QUERY_BUILDER(self->obj);

	guint i = 0;
	guint n_objects;
	GObject **objects = 
		midgard_query_builder_execute(builder, &n_objects);

	if(objects == NULL) {
		
		PyObject *list = PyTuple_New(i);
		return list;
	}

	PyObject *list = PyTuple_New(n_objects);
	
	OBJECTS2LIST(objects, list);
	
	g_free(objects);
	
	return list;
}

static PyMethodDef pymidgard_qb_methods[] = {
	{ "add_constraint", (PyCFunction)pymidgard_qb_add_constraint, METH_VARARGS },
	{ "add_constraint_with_property", 
		(PyCFunction)pymidgard_qb_add_constraint_with_property, METH_VARARGS },
	{ "begin_group", (PyCFunction)pymidgard_qb_begin_group, METH_VARARGS },
	{ "end_group", (PyCFunction)pymidgard_qb_end_group, METH_VARARGS },
	{ "set_offset", (PyCFunction)pymidgard_qb_set_offset, METH_VARARGS },
	{ "set_limit", (PyCFunction)pymidgard_qb_set_limit, METH_VARARGS },
	{ "add_order", (PyCFunction)pymidgard_qb_add_order, METH_VARARGS },
	{ "include_deleted", (PyCFunction)pymidgard_qb_include_deleted, METH_VARARGS },
	{ "count", (PyCFunction)pymidgard_qb_count, METH_VARARGS },
	{ "execute", (PyCFunction)pymidgard_qb_execute, METH_VARARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_qb_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "query_builder",                   /* tp_name */
    sizeof(PyGObject),          /* tp_basicsize */
    0,                                 /* tp_itemsize */
    /* methods */
    (destructor)_py_midgard_gobject_destructor,  /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    NULL,                        /* Documentation string */
    (traverseproc)0,     /* tp_traverse */
    (inquiry)0,             /* tp_clear */
    (richcmpfunc)0,   /* tp_richcompare */
    offsetof(PyGObject, weakreflist),             /* tp_weaklistoffset */
    (getiterfunc)0,          /* tp_iter */
    (iternextfunc)0,     /* tp_iternext */
    pymidgard_qb_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__query_builder_constructor,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,            /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void py_midgard_query_builder_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"query_builder", 
			MIDGARD_TYPE_QUERY_BUILDER, 
			&Pymidgard_qb_Type, 
			Py_BuildValue("(O)", pygobject_type));

}

PyTypeObject py_midgard_query_builder_get_type_object(void)
{
	return Pymidgard_qb_Type;	
}
