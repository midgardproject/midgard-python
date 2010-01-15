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

#define MGDOBJECT_DEBUG(__name) \
	CHECK_MGD; \
	const gchar *cname = NULL; \
	if(self) { \
		CLASS_METHOD_DEBUG(G_OBJECT_TYPE_NAME(G_OBJECT(self->obj)), __name); \
	} else { \
	if(cname == NULL) \
		CLASS_METHOD_DEBUG("midgard_dbobject", __name); \
	}

PyTypeObject G_GNUC_INTERNAL Pymidgard_schema_object_Type;

PyTypeObject G_GNUC_INTERNAL Pymidgard_object_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "midgard_object",                   /* tp_name */
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

void py_midgard_base_object_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"midgard_object", 
			MIDGARD_TYPE_OBJECT, 
			&Pymidgard_object_Type, 
			Py_BuildValue("(O)", pygobject_type));

	pyg_set_object_has_new_constructor(MIDGARD_TYPE_OBJECT);
}

PyTypeObject G_GNUC_INTERNAL Pymidgard_metadata_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "metadata",                   /* tp_name */
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

void py_midgard_metadata_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"metadata", 
			MIDGARD_TYPE_METADATA, 
			&Pymidgard_metadata_Type, 
			Py_BuildValue("(O)", pygobject_type));
}

/* MgdSchema classes */
static int __schema_object_construct(PyGObject *self, PyObject *args, PyObject *kwargs)
{
	PyObject *pyval = NULL;
	int id;
        // long lid;
	const gchar *guid;
	if(!PyArg_ParseTuple(args, "|O", &pyval))
		return -1;
	
	if(pygobject_constructv(self, 0, NULL))
		PyErr_SetString(PyExc_RuntimeError, "could not create object");

	MidgardConnection *mgd = 
		_py_midgard_connection_singleton_get();
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(!mgd)
		PyErr_SetString(PyExc_RuntimeError, "Failed to get midgard connection");

	if(!mobj)
		PyErr_SetString(PyExc_RuntimeError, "Failed to get underlying object");
	
	midgard_object_set_connection(mobj, mgd);

	MGDOBJECT_DEBUG("__init__");

	/* FIXME
	 * I parse arguments again , but I really do not have idea how to 
	 * get PyObject type from pyval in this case.
	 * GType returned from pyval PyObject is always 0. */
	if(pyval) {

		if(PyString_Check(pyval)) {
			
			PyArg_ParseTuple(args, "s", &guid);
			if(!midgard_object_get_by_guid(mobj, guid)) {
				PyErr_SetString(PyExc_TypeError, "Could not create new instance");
				return -1;
			}
		}

		if(PyInt_Check(pyval)) {
			
			PyArg_ParseTuple(args, "i", &id);
			if(!midgard_object_get_by_id(mobj, id)) {
				PyErr_SetString(PyExc_TypeError, "Could not create new instance");
				return -1;
			}
		}

		if(PyLong_Check(pyval)) {
			
			PyArg_ParseTuple(args, "l", &id);
			if(!midgard_object_get_by_id(mobj, id)) {
				PyErr_SetString(PyExc_TypeError, "Could not create new instance");
				return -1;
			}
		}
	}	

	return (self->obj) ? 0 : -1;
}

static PyObject *
pymidgard_object_create(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("create");

	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_create(mobj))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_get_by_id(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("get_by_id");
	guint id;
	if(!PyArg_ParseTuple(args, "i", &id))
		return NULL;

	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_get_by_id(mobj, id))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_get_by_guid(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("get_by_guid");
	const gchar *guid;
	if(!PyArg_ParseTuple(args, "s", &guid))
		return NULL;

	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_get_by_guid(mobj, guid))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_update(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("update");	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_update(mobj))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_delete(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("delete");
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_delete(mobj))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_purge(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("purge");
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_purge(mobj))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_is_in_parent_tree(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("is_in_parent_tree");
	guint root_id, id;

	if(!PyArg_ParseTuple(args, "ii", &root_id, &id))
		return NULL;

	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_is_in_parent_tree(mobj, root_id, id))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_is_in_tree(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("is_in_tree");
	guint root_id, id;

	if (!PyArg_ParseTuple(args, "ii", &root_id, &id))
		return NULL;

	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_is_in_tree(mobj, root_id, id))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_has_dependents(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("has_dependents");
	
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_has_dependents(mobj))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_get_parent(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("get_parent");
	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);
	MidgardObject *parent = 
		midgard_object_get_parent(mobj);
	
	if(parent) 
		return Py_BuildValue("O", pygobject_new(G_OBJECT(parent)));
	
	Py_RETURN_NONE;
}

static PyObject *
pymidgard_object_parent(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("parent");
	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);
	const gchar *parent = 
		midgard_object_parent(mobj);

	return Py_BuildValue("s", parent);	
}

static PyObject *
pymidgard_object_list(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("list");
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	guint i = 0;
	guint n_objects;
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);
	GObject **objects = midgard_object_list(mobj, &n_objects);
	
	if(objects == NULL) {
		
		PyObject *list = PyTuple_New(i);
		return list;
	}

	PyObject *list = PyTuple_New(n_objects);

	if(!objects) 
		return list;

	OBJECTS2LIST(objects, list);

	g_free(objects);

	return list;
}

static PyObject *
pymidgard_object_list_children(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("list_children");
	const gchar *childcname;
	if(!PyArg_ParseTuple(args, "s", &childcname))
		return NULL;

	guint i = 0;
	guint n_objects;
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);
	GObject **objects = midgard_object_list_children(mobj, childcname, &n_objects);

	if(objects == NULL) {
		
		PyObject *list = PyTuple_New(i);
		return list;
	}

	PyObject *list = PyTuple_New(n_objects);

	if(!objects) 
		return list;

	OBJECTS2LIST(objects, list);

	g_free(objects);

	return list;
}

static PyObject *
pymidgard_object_get_by_path(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("get_by_path");
	const gchar *path;
	if(!PyArg_ParseTuple(args, "s", &path))
		return NULL;
	
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_get_by_path(mobj, path))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_approve(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("approve");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_approve(mobj))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_is_approved(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("is_approved");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_is_approved(mobj))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_unapprove(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("unapprove");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_unapprove(mobj))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_lock(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("lock");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_lock(mobj))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_is_locked(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("is_locked");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_is_locked(mobj))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_object_unlock(PyGObject *self, PyObject *args)
{
	MGDOBJECT_DEBUG("unlock");

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	MidgardObject *mobj = MIDGARD_OBJECT(self->obj);

	if(midgard_object_unlock(mobj))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyMethodDef pymidgard_object_methods[] = {
	{ "get_by_id", (PyCFunction)pymidgard_object_get_by_id, METH_VARARGS, NULL },
	{ "get_by_guid", (PyCFunction)pymidgard_object_get_by_guid, METH_VARARGS, NULL },
	{ "create", (PyCFunction)pymidgard_object_create, METH_NOARGS, NULL },
	{ "update", (PyCFunction)pymidgard_object_update, METH_VARARGS, NULL },
	{ "delete", (PyCFunction)pymidgard_object_delete, METH_VARARGS, NULL },	
	{ "purge", (PyCFunction)pymidgard_object_purge, METH_VARARGS, NULL },
	{ "is_in_parent_tree", (PyCFunction)pymidgard_object_is_in_parent_tree, METH_VARARGS },
	{ "is_in_tree", (PyCFunction)pymidgard_object_is_in_tree, METH_VARARGS },
	{ "get_parent", (PyCFunction)pymidgard_object_get_parent, METH_VARARGS },
	{ "parent", (PyCFunction)pymidgard_object_parent, METH_VARARGS },
	{ "list", (PyCFunction)pymidgard_object_list, METH_VARARGS },
	{ "list_children", (PyCFunction)pymidgard_object_list_children, METH_VARARGS },
	{ "has_dependents", (PyCFunction)pymidgard_object_has_dependents, METH_NOARGS },
	{ "get_by_path", (PyCFunction)pymidgard_object_get_by_path, METH_VARARGS },
	{ "get_parameter", (PyCFunction)pymidgard_object_get_parameter, METH_VARARGS },
	{ "set_parameter", (PyCFunction)pymidgard_object_set_parameter, METH_VARARGS },
	{ "has_parameters", (PyCFunction)pymidgard_object_has_parameters, METH_NOARGS },
	{ "delete_parameters", (PyCFunction)pymidgard_object_delete_parameters, METH_VARARGS },
	{ "purge_parameters", (PyCFunction)pymidgard_object_purge_parameters, METH_VARARGS },
	{ "list_parameters", (PyCFunction)pymidgard_object_list_parameters, METH_VARARGS },
	{ "find_parameters", (PyCFunction)pymidgard_object_find_parameters, METH_VARARGS },
	{ "has_attachments", (PyCFunction)pymidgard_object_has_attachments, METH_NOARGS },
	{ "delete_attachments", (PyCFunction)pymidgard_object_delete_attachments, METH_VARARGS },
	{ "purge_attachments", (PyCFunction)pymidgard_object_purge_attachments, METH_VARARGS },
	{ "list_attachments", (PyCFunction)pymidgard_object_list_attachments, METH_VARARGS },
	{ "find_attachments", (PyCFunction)pymidgard_object_find_attachments, METH_VARARGS },
	{ "create_attachment", (PyCFunction)pymidgard_object_create_attachment, METH_VARARGS },
	{ "approve", (PyCFunction)pymidgard_object_approve, METH_NOARGS },
	{ "is_approved", (PyCFunction)pymidgard_object_is_approved, METH_NOARGS },
	{ "unapprove", (PyCFunction)pymidgard_object_unapprove, METH_NOARGS },
	{ "lock", (PyCFunction)pymidgard_object_lock, METH_NOARGS },
	{ "is_locked", (PyCFunction)pymidgard_object_is_locked, METH_NOARGS },
	{ "unlock", (PyCFunction)pymidgard_object_unlock, METH_NOARGS },
	{ NULL, NULL, 0 }
};

PyObject *
_py_midgard_get_object_attribute(PyObject *self, PyObject *attr)
{
	GObject *object = G_OBJECT(((PyGObject*)self)->obj);

	/* Now we do fallback to generic GetAttr */
	if(!object) 
		return PyObject_GenericGetAttr(self, attr);

	/* There's underlying GObject so we can run custom routines and get property*/
	GObjectClass *klass = G_OBJECT_GET_CLASS(object);
	GParamSpec *pspec;
	const gchar *attr_name = PyString_AsString(attr);
        // PyTypeObject *tp = self->ob_type;

	pspec = g_object_class_find_property(klass, attr_name);

	if(!pspec) {

		/* Try Generic Fallback once again */
		return PyObject_GenericGetAttr(self, attr);		
	}

	GValue pval = {0, };
	g_value_init(&pval, pspec->value_type);
	g_object_get_property(object, attr_name, &pval);

	PyObject *pvalue = py_midgard_pyobject_from_gvalue((const GValue*) &pval, FALSE);
	g_value_unset(&pval);
	return pvalue;
}

int
_py_midgard_set_object_attribute(PyObject *self, PyObject *attr, PyObject *value)
{
	GObject *object = G_OBJECT(((PyGObject*)self)->obj);	

	/* Now we do fallback to generic SetAttr */
	if(!object) 
		return PyObject_GenericSetAttr(self, attr, value);

	/* There's underlying GObject so we can run custom routines and set property*/
	GObjectClass *klass = G_OBJECT_GET_CLASS(object);
	GParamSpec *pspec;
	const gchar *attr_name = PyString_AsString(attr);
	PyTypeObject *tp = self->ob_type;

	pspec = g_object_class_find_property(klass, attr_name);

	if(!pspec) {
		
		PyErr_Format(PyExc_AttributeError,
				"'%.50s' object has no attribute '%.400s'",
				tp->tp_name, attr_name);
		return -1;
	}

	GValue pval = {0, };
	g_value_init(&pval, pspec->value_type);

	if(pspec->value_type == G_TYPE_STRING) {
		
		const gchar *strv = PyString_AsString(value);
		gchar *utf8_string;
		gboolean is_utf8 = TRUE;
		
		if(strv != NULL)
			is_utf8 = g_utf8_validate(strv, -1, NULL);
		
		if(!is_utf8) {
			
			utf8_string = g_locale_to_utf8(strv, -1, NULL, NULL, NULL);
			
			if(utf8_string == NULL) {
				g_warning("UTF-8 converion failed");
				return -1;
			}
			
			g_value_set_string(&pval, utf8_string);
			g_object_set_property(object, attr_name, &pval);
			g_value_unset(&pval);

			return 0;
		}
	}

	py_midgard_gvalue_from_pyobject(&pval, value);
	g_object_set_property(object, attr_name, &pval);
	g_value_unset(&pval);

	return 0;
}

PyTypeObject *__new_object_struct(const gchar *typename)
{
	/* initialize object type */
	PyTypeObject *ot = g_new0(PyTypeObject, 1);
	ot->ob_refcnt = 1;
	ot->ob_type = NULL;
	ot->tp_name = g_strdup (typename);

	ot->ob_size = 0;
	ot->tp_basicsize = sizeof(PyGObject);
	ot->tp_itemsize = 0;
	ot->tp_dealloc = _py_midgard_gobject_destructor;
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
	ot->tp_doc = "User defined MgdSchema class";
	ot->tp_traverse = 0;
	ot->tp_clear = 0;
	ot->tp_richcompare = 0;
	ot->tp_weaklistoffset = offsetof(PyGObject, weakreflist);
	ot->tp_iter = 0;
	ot->tp_iternext = 0;	
	ot->tp_methods = pymidgard_object_methods;
	//ot->tp_methods = NULL;
	ot->tp_members = 0;
	ot->tp_getset = 0;
	ot->tp_base = NULL;
	ot->tp_dict = NULL;
	ot->tp_descr_get = 0;
	ot->tp_descr_set = 0;
	ot->tp_dictoffset = offsetof(PyGObject, inst_dict);
	ot->tp_init = (initproc)__schema_object_construct;		
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

GHashTable *_pytype_hash;

void py_midgard_object_register_class(
		PyObject *d, gpointer pygobject_type)
{
	py_midgard_base_object_register_class(d, pygobject_type);

	_pytype_hash = g_hash_table_new(g_str_hash, g_str_equal);

	guint n_types, i;
	const gchar *typename;
	GType *all_types = g_type_children(MIDGARD_TYPE_OBJECT, &n_types);

	for (i = 0; i < n_types; i++) {

		typename = g_type_name(all_types[i]);

		PyTypeObject *ot = __new_object_struct(typename);
		pygobject_register_class(d,
				typename, all_types[i], ot,  
				/* FIXME, What is a static bases in this case? */
				Py_BuildValue("(O)", pygobject_type)); 

		/* pyg_set_object_has_new_constructor(all_types[i]); */

		g_hash_table_insert(_pytype_hash, 
				(gpointer*)typename, (gpointer *)ot);
	}

	g_free(all_types);

	py_midgard_metadata_register_class(d, pygobject_type);
}

PyTypeObject *py_midgard_lookup_schema_type(const gchar *name)
{
	g_assert(name != NULL);
	g_assert(_pytype_hash != NULL);

	return (PyTypeObject *)g_hash_table_lookup(_pytype_hash, name);
}

