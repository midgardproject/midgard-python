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

PyTypeObject G_GNUC_INTERNAL Pymidgard_rp_Type;

#define MRP_DEBUG(__name) \
	CHECK_MGD; \
        CLASS_METHOD_DEBUG(Pymidgard_rp_Type.tp_name, __name);

static int
__reflection_property_constructor(PyGObject *self, PyObject *args, PyObject *kwargs)
{
	MRP_DEBUG("__init__");
	const gchar *classname;
	
	if(!PyArg_ParseTuple(args, "s", &classname))
		return -1;

	GObjectClass *klass = g_type_class_peek(g_type_from_name(classname));
	if(klass == NULL)
		return -1; /* FIXME throw exception */

	MidgardReflectionProperty *mrp = 
		midgard_reflection_property_new(MIDGARD_DBOBJECT_CLASS(klass));

	if(!mrp) return -1;

	self->obj = G_OBJECT(mrp);

	return 0;
}

static PyObject *
pymidgard_reflection_property_get_midgard_type(PyGObject *self, PyObject *args) 
{
	MRP_DEBUG("get_midgard_type");
	const gchar *name;

	if(!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	MidgardReflectionProperty *mrp = MIDGARD_REFLECTION_PROPERTY(self->obj);
	GType mtype = midgard_reflection_property_get_midgard_type(mrp, name);
	
	return Py_BuildValue("i", (int)mtype);
}

static PyObject *
pymidgard_reflection_property_is_link(PyGObject *self, PyObject *args) 
{
	MRP_DEBUG("is_link");
	const gchar *name;

	if(!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	MidgardReflectionProperty *mrp = MIDGARD_REFLECTION_PROPERTY(self->obj);
	if(midgard_reflection_property_is_link(mrp, name))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_reflection_property_is_linked(PyGObject *self, PyObject *args) 
{
	MRP_DEBUG("is_linked");
	const gchar *name;

	if(!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	MidgardReflectionProperty *mrp = MIDGARD_REFLECTION_PROPERTY(self->obj);
	if(midgard_reflection_property_is_linked(mrp, name))
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_reflection_property_get_link_class(PyGObject *self, PyObject *args) 
{
	MRP_DEBUG("get_link_class");
	const gchar *name;

	if(!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	MidgardReflectionProperty *mrp = MIDGARD_REFLECTION_PROPERTY(self->obj);
	const MidgardDBObjectClass *klass = midgard_reflection_property_get_link_class(mrp, name);
	
	if(klass == NULL)
		Py_RETURN_NONE;
	
	return Py_BuildValue("s", G_OBJECT_CLASS_NAME(klass));
}

static PyObject *
pymidgard_reflection_property_get_link_name(PyGObject *self, PyObject *args) 
{
	MRP_DEBUG("get_link_name");
	const gchar *name;

	if(!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	MidgardReflectionProperty *mrp = MIDGARD_REFLECTION_PROPERTY(self->obj);
	const gchar *lname = 
		midgard_reflection_property_get_link_name(mrp, name);

	if(lname == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", lname);
}

static PyObject *
pymidgard_reflection_property_get_link_target(PyGObject *self, PyObject *args) 
{
	MRP_DEBUG("get_link_target");
	const gchar *name;

	if(!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	MidgardReflectionProperty *mrp = MIDGARD_REFLECTION_PROPERTY(self->obj);
	const gchar *target = 
		midgard_reflection_property_get_link_target(mrp, name);

	if(target == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", target);
}

static PyObject *
pymidgard_reflection_property_description(PyGObject *self, PyObject *args) 
{
	MRP_DEBUG("description");
	const gchar *name;

	if(!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	MidgardReflectionProperty *mrp = MIDGARD_REFLECTION_PROPERTY(self->obj);
	const gchar *description = 
		midgard_reflection_property_description(mrp, name);

	if(description == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", description);
}

static PyObject *
pymidgard_reflection_property_get_user_value(PyGObject *self, PyObject *args) 
{
	MRP_DEBUG("get_user_value");
	const gchar *property;
	const gchar *name;

	if(!PyArg_ParseTuple(args, "ss", &property, &name))
		return NULL;

	MidgardReflectionProperty *mrp = MIDGARD_REFLECTION_PROPERTY(self->obj);
	const gchar *value = midgard_reflection_property_get_user_value(mrp, property, name);

  	if(value == NULL)
		Py_RETURN_NONE;

        return Py_BuildValue("s", value);
}


static PyMethodDef pymidgard_rp_methods[] = {
	{ "get_midgard_type", (PyCFunction)pymidgard_reflection_property_get_midgard_type, METH_VARARGS },
	{ "is_link", (PyCFunction)pymidgard_reflection_property_is_link, METH_VARARGS },
	{ "is_linked", (PyCFunction)pymidgard_reflection_property_is_linked, METH_VARARGS },
	{ "get_link_class", (PyCFunction)pymidgard_reflection_property_get_link_class, METH_VARARGS },
	{ "get_link_name", (PyCFunction)pymidgard_reflection_property_get_link_name, METH_VARARGS },
	{ "get_link_target", (PyCFunction)pymidgard_reflection_property_get_link_target, METH_VARARGS },
	{ "description", (PyCFunction)pymidgard_reflection_property_description, METH_VARARGS },
	{ "get_user_value", (PyCFunction)pymidgard_reflection_property_get_user_value, METH_VARARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_rp_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "reflection_property",                   /* tp_name */
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
    pymidgard_rp_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__reflection_property_constructor,             /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void py_midgard_reflection_property_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"reflection_property", 
			MIDGARD_TYPE_REFLECTION_PROPERTY, 
			&Pymidgard_rp_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
