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

PyTypeObject G_GNUC_INTERNAL Pymidgard_collector_Type;

#define COLLECTOR_DEBUG(__name) \
	CHECK_MGD; \
	CLASS_METHOD_DEBUG(Pymidgard_collector_Type.tp_name, __name);

static int
__collector_constructor(PyGObject *self, PyObject *args, PyObject *kwargs)
{
	COLLECTOR_DEBUG("__init__");
	const gchar *classname;
	const gchar *domain;
	PyObject *pvalue = NULL;
	GValue *cvalue = NULL;
	if(!PyArg_ParseTuple(args, "ssO", &classname, &domain, &pvalue))
		return -1;
	
	MidgardConnection *mgd = 
		_py_midgard_connection_singleton_get();

	if(pvalue != NULL) {
		
		GValue gval = gvalue_from_pyobject(pvalue);
		py_midgard_gvalue_from_pyobject(&gval, pvalue);
		cvalue = g_new0(GValue, 1);
		g_value_init(cvalue, G_VALUE_TYPE(&gval));
		g_value_copy((const GValue *) &gval, cvalue);
		g_value_unset(&gval);
	}

	MidgardCollector *mc =
		midgard_collector_new(mgd, classname, domain, cvalue);

	if(!mc) return -1;
	
	self->obj = G_OBJECT(mc);
	
	return 0;
}

static PyObject *
pymidgard_collector_set_key_property(PyGObject *self, PyObject *args) 
{
	COLLECTOR_DEBUG("set_key_property");
	const gchar *key;
	PyObject *pvalue = NULL;
	GValue *cvalue = NULL;

	if(!PyArg_ParseTuple(args, "s|O", &key, &pvalue))
		return NULL;
	
	if(pvalue != NULL) {

		GValue gval = gvalue_from_pyobject(pvalue);
		py_midgard_gvalue_from_pyobject(&gval, pvalue);
		cvalue = g_new0(GValue, 1);
		g_value_init(cvalue, G_VALUE_TYPE(&gval));
		g_value_copy((const GValue *) &gval, cvalue);
		g_value_unset(&gval);
	}

	if(midgard_collector_set_key_property(
				MIDGARD_COLLECTOR(self->obj), key, cvalue))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_collector_add_value_property(PyGObject *self, PyObject *args) 
{
	COLLECTOR_DEBUG("add_value_property");
	const gchar *property;

	if(!PyArg_ParseTuple(args, "s", &property))
		return NULL;

	if(midgard_collector_add_value_property(
				MIDGARD_COLLECTOR(self->obj), property))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}
	
static PyObject *
pymidgard_collector_set(PyGObject *self, PyObject *args) 
{
	COLLECTOR_DEBUG("set");
	const gchar *key, *subkey;
	PyObject *pvalue = NULL;
	GValue *cvalue = NULL;

	if(!PyArg_ParseTuple(args, "ssO", &key, &subkey, &pvalue))
		return NULL;
	
	if(pvalue != NULL) {

		GValue gval = gvalue_from_pyobject(pvalue);
		py_midgard_gvalue_from_pyobject(&gval, pvalue);
		cvalue = g_new0(GValue, 1);
		g_value_init(cvalue, G_VALUE_TYPE(&gval));
		g_value_copy((const GValue *) &gval, cvalue);
		g_value_unset(&gval);
	}

	if(midgard_collector_set(MIDGARD_COLLECTOR(self->obj), key, subkey, cvalue))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static void __count_gdata_keys(GQuark key_id, 
		gpointer data, gpointer user_data) 
{
	MidgardTypeHolder *holder = (MidgardTypeHolder *) user_data;
	holder->elements++;
}

static void __update_list(GQuark key_id,
		gpointer data, gpointer user_data)
{
	PyObject *list = (PyObject *) user_data;
	PyObject *pvalue = NULL;
	GValue *gvalue = (GValue *) data;

	if (gvalue == NULL)
		return;
	
	py_midgard_gvalue_from_pyobject(gvalue, pvalue);
	gint i = PyTuple_Size(list);
	PyTuple_SetItem(list, i++, pvalue);
}

static PyObject *
pymidgard_collector_get(PyGObject *self, PyObject *args) 
{
	COLLECTOR_DEBUG("get");
	const gchar *key;

	if(!PyArg_ParseTuple(args, "s", &key))
		return NULL;

	GData *keyslist = 
		midgard_collector_get(MIDGARD_COLLECTOR(self->obj), key);

	if(keyslist == NULL)
		Py_RETURN_NONE;

	/* Initialize tuple list */
	MidgardTypeHolder *holder = g_new(MidgardTypeHolder, 1);
	g_datalist_foreach(&keyslist, __count_gdata_keys, holder);
	PyObject *list = PyTuple_New(holder->elements);

	/* Fill list */
	g_datalist_foreach(&keyslist, __update_list, list);
	return list;
}

static PyObject *
pymidgard_collector_get_subkey(PyGObject *self, PyObject *args) 
{
	COLLECTOR_DEBUG("get_subkey");
	const gchar *key, *subkey;

	if(!PyArg_ParseTuple(args, "ss", &key, &subkey))
		return NULL;

	GValue *gval = 
		midgard_collector_get_subkey(MIDGARD_COLLECTOR(self->obj), key, subkey);

	if(gval == NULL)
		Py_RETURN_NONE;

	PyObject *pvalue = pyg_value_as_pyobject((const GValue *)gval, FALSE);

	return pvalue;
}

static PyObject *
pymidgard_collector_list_keys(PyGObject *self, PyObject *args) 
{
	COLLECTOR_DEBUG("list_keys");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	guint i = 0;
	PyObject *list;
	gchar **keys = 
		midgard_collector_list_keys(MIDGARD_COLLECTOR(self->obj));

	if(keys == NULL) {
		
		list = PyTuple_New(i);
		return list;
	}

	while(keys[i] != NULL)
		i++;

	list = PyTuple_New(i);

	i = 0;
	while(keys[i] != NULL) {
		
		PyObject *strname = PyString_FromString(keys[i]);
		PyTuple_SetItem(list, i, strname);
		i++;
	}

	g_free(keys);

	return list;
}

static PyObject *
pymidgard_collector_merge(PyGObject *self, PyObject *args) 
{
	COLLECTOR_DEBUG("merge");
	PyObject *pmc;
	gboolean overwrite = FALSE;
	
	if(!PyArg_ParseTuple(args, "O|b", &pmc, &overwrite))
		return NULL;

	MidgardCollector *mc = MIDGARD_COLLECTOR(((PyGObject *)pmc)->obj);
	MidgardCollector *uc = MIDGARD_COLLECTOR(self->obj);

	if(midgard_collector_merge(uc, mc, overwrite))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_collector_remove_key(PyGObject *self, PyObject *args) 
{
	COLLECTOR_DEBUG("remove_key");
	const gchar *key;

	if(!PyArg_ParseTuple(args, "s", &key))
		return NULL;

	if(midgard_collector_remove_key(MIDGARD_COLLECTOR(self->obj), key))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

/* QB overwritten methods */

static PyObject *
pymidgard_collector_add_constraint(PyGObject *self, PyObject *args)
{
	COLLECTOR_DEBUG("add_constraint");	
	const gchar *prop, *op;
	PyObject *pvalue;
	gboolean added;
	if(!PyArg_ParseTuple(args, "ssO", &prop, &op, &pvalue))
		return NULL;

	MidgardCollector *collector = 
		MIDGARD_COLLECTOR(self->obj);

	GValue gval = gvalue_from_pyobject(pvalue);	
	py_midgard_gvalue_from_pyobject(&gval, pvalue);

	added = midgard_collector_add_constraint(collector, prop, op, &gval);

	if(added)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_collector_add_constraint_with_property(PyGObject *self, PyObject *args)
{
	COLLECTOR_DEBUG("add_constraint_with_property");	
	const gchar *prop_a, *op, *prop_b;
        // PyObject *pvalue;
	gboolean added;
	if(!PyArg_ParseTuple(args, "sss", &prop_a, &op, &prop_b))
		return NULL;

	MidgardCollector *collector = 
		MIDGARD_COLLECTOR(self->obj);

	added = midgard_collector_add_constraint_with_property(
			collector, prop_a, op, prop_b);

	if(added)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_collector_begin_group(PyGObject *self, PyObject *args)
{
	COLLECTOR_DEBUG("begin_group");
	const gchar *group_op;
	if(!PyArg_ParseTuple(args, "s", &group_op))
		return NULL;

	MidgardCollector *collector = 
		MIDGARD_COLLECTOR(self->obj);

	if(midgard_collector_begin_group(collector, group_op))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_collector_end_group(PyGObject *self, PyObject *args)
{
	COLLECTOR_DEBUG("end_group");
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardCollector *collector = 
		MIDGARD_COLLECTOR(self->obj);
	
	if(midgard_collector_end_group(collector))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_collector_add_order(PyGObject *self, PyObject *args)
{
	COLLECTOR_DEBUG("add_order");
	
	const gchar *prop, *order;
	if(!PyArg_ParseTuple(args, "ss", &prop, &order))
		return NULL;

	MidgardCollector *collector = 
		MIDGARD_COLLECTOR(self->obj);

	if(midgard_collector_add_order(collector, prop, order))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_collector_set_offset(PyGObject *self, PyObject *args)
{
	COLLECTOR_DEBUG("set_offset");
	
	guint offset;
	if(!PyArg_ParseTuple(args, "i", &offset))
		return NULL;

	MidgardCollector *collector = 
		MIDGARD_COLLECTOR(self->obj);

	midgard_collector_set_offset(collector, offset);

	Py_RETURN_NONE;	
}

static PyObject *
pymidgard_collector_set_limit(PyGObject *self, PyObject *args)
{
	COLLECTOR_DEBUG("set_limit");
	
	long limit;
	if(!PyArg_ParseTuple(args, "l", &limit))
		return NULL;

	MidgardCollector *collector = 
		MIDGARD_COLLECTOR(self->obj);

	midgard_collector_set_limit(collector, limit);

	Py_RETURN_NONE;
}

static PyObject *
pymidgard_collector_count(PyGObject *self, PyObject *args)
{
	COLLECTOR_DEBUG("count");
	
        Py_RETURN_NONE;
	/* FIXME 
	MidgardCollector *collector = MIDGARD_COLLECTOR(self->obj);

	guint counted = midgard_collector_count(collector);

	return Py_BuildValue("i", counted);
	*/
}

static PyObject *
pymidgard_collector_include_deleted(PyGObject *self, PyObject *args)
{
	COLLECTOR_DEBUG("include_deleted");
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardCollector *collector = 
		MIDGARD_COLLECTOR(self->obj);

	midgard_collector_include_deleted(collector);

	Py_RETURN_NONE;	
}

static PyObject *
pymidgard_collector_execute(PyGObject *self, PyObject *args) 
{
	COLLECTOR_DEBUG("execute");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	midgard_collector_execute(MIDGARD_COLLECTOR(self->obj));

	Py_RETURN_NONE;
}

static PyMethodDef pymidgard_collector_methods[] = {
	{ "set_key_property", (PyCFunction)pymidgard_collector_set_key_property, METH_VARARGS },
	{ "add_value_property", (PyCFunction)pymidgard_collector_add_value_property, METH_VARARGS },
	{ "set", (PyCFunction)pymidgard_collector_set, METH_VARARGS },
	{ "get", (PyCFunction)pymidgard_collector_get, METH_VARARGS },
	{ "get_subkey", (PyCFunction)pymidgard_collector_get_subkey, METH_VARARGS },
	{ "list_keys", (PyCFunction)pymidgard_collector_list_keys, METH_VARARGS },
	{ "merge", (PyCFunction)pymidgard_collector_merge, METH_VARARGS },
	{ "remove_key", (PyCFunction)pymidgard_collector_remove_key, METH_VARARGS },	
	{ "add_constraint", (PyCFunction)pymidgard_collector_add_constraint, METH_VARARGS },
	{ "add_constraint_with_property", 
		(PyCFunction)pymidgard_collector_add_constraint_with_property, METH_VARARGS },
	{ "begin_group", (PyCFunction)pymidgard_collector_begin_group, METH_VARARGS },
	{ "end_group", (PyCFunction)pymidgard_collector_end_group, METH_VARARGS },
	{ "set_offset", (PyCFunction)pymidgard_collector_set_offset, METH_VARARGS },
	{ "set_limit", (PyCFunction)pymidgard_collector_set_limit, METH_VARARGS },
	{ "add_order", (PyCFunction)pymidgard_collector_add_order, METH_VARARGS },
	{ "include_deleted", (PyCFunction)pymidgard_collector_include_deleted, METH_VARARGS },
	{ "count", (PyCFunction)pymidgard_collector_count, METH_NOARGS },

	{ "execute", (PyCFunction)pymidgard_collector_execute, METH_VARARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_collector_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "collector",                   /* tp_name */
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
    "MidgardCollector",                        /* Documentation string */
    (traverseproc)0,     /* tp_traverse */
    (inquiry)0,             /* tp_clear */
    (richcmpfunc)0,   /* tp_richcompare */
    offsetof(PyGObject, weakreflist),             /* tp_weaklistoffset */
    (getiterfunc)0,          /* tp_iter */
    (iternextfunc)0,     /* tp_iternext */
    pymidgard_collector_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__collector_constructor,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void py_midgard_collector_register_class(
		PyObject *d, gpointer ot)
{
	pygobject_register_class(d, 
			"collector", 
			MIDGARD_TYPE_COLLECTOR, 
			&Pymidgard_collector_Type, NULL); /* FIXME, collector is not subclass of QB */ 
			//Py_BuildValue("(O)", ot));

}

PyTypeObject py_midgard_collector_get_type_object(void)
{
	return Pymidgard_collector_Type;
}

