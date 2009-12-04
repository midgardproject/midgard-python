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
//#include "frameobject.h"

PyTypeObject G_GNUC_INTERNAL Pymidgard_replicator_Type;

#define REPLICATOR_DEBUG(__name) \
	CHECK_MGD; \
	CLASS_METHOD_DEBUG(Pymidgard_replicator_Type.tp_name, __name);
	
static PyObject *
pymidgard_replicator_serialize(PyGObject *self, PyObject *args) 
{
	REPLICATOR_DEBUG("serialize");
        // MidgardObject *mobject; 
	PyObject *pobject; 

	if(!PyArg_ParseTuple(args, "O", &pobject))
		return NULL;

	gchar *srld = midgard_replicator_serialize(G_OBJECT(((PyGObject *)pobject)->obj));

	PyObject *ret = Py_BuildValue("s", srld);
	g_free(srld);

	return ret;
}

static PyObject *
pymidgard_replicator_export(PyGObject *self, PyObject *args) 
{	
	REPLICATOR_DEBUG("export");
	PyObject *pobject;

	if(!PyArg_ParseTuple(args, "O", &pobject))
		return NULL;
	
	gboolean exported =
		midgard_replicator_export(MIDGARD_DBOBJECT(((PyGObject *)pobject)->obj));

	if(exported)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_replicator_export_purged(PyGObject *self, PyObject *args) 
{
	REPLICATOR_DEBUG("export_purged");
	const gchar *classname, *startdate = NULL, *enddate = NULL;

	if(!PyArg_ParseTuple(args, "s|ss", &classname, &startdate, &enddate))
		return NULL;
	
	MidgardObjectClass *klass = 
		MIDGARD_OBJECT_GET_CLASS_BY_NAME(classname);

	if(!klass) { 
		PyErr_SetString(PyExc_SystemError, "Can not replicate object. Invalid class name");
		return NULL;
	}

	MidgardConnection *mgd = 
		_py_midgard_connection_singleton_get();

	gchar *xml = midgard_replicator_export_purged (mgd, klass, startdate, enddate);
	
	PyObject *ret = Py_BuildValue("s", xml);
	g_free(xml);

	return ret;
}

static PyObject *
pymidgard_replicator_export_blob(PyGObject *self, PyObject *args) 
{
	REPLICATOR_DEBUG("export_blob");
	PyObject *pobject;
	
	if(!PyArg_ParseTuple(args, "O", &pobject))
		return NULL;
	
	gchar *xml = midgard_replicator_export_blob(MIDGARD_OBJECT(((PyGObject *)pobject)->obj));

	PyObject *ret = Py_BuildValue("s", xml);
	g_free(xml);
	
	return ret;
}

static PyObject *
pymidgard_replicator_export_media(PyGObject *self, PyObject *args) 
{
        return NULL;
	REPLICATOR_DEBUG("export_media");
#warning "pymidgard_replicator_export_media() is not implemented"
	g_warning("FIXME, it's not implemented");
}

static PyObject *
pymidgard_replicator_unserialize(PyGObject *self, PyObject *args) 
{
	REPLICATOR_DEBUG("unserialize");
	const gchar *xml;
	gboolean force = FALSE;
	PyObject *list;

	if(!PyArg_ParseTuple(args, "s|b", &xml, &force))
		return NULL;

	MidgardConnection *mgd =
		_py_midgard_connection_singleton_get();

	guint i = 0;
	GObject **objects = midgard_replicator_unserialize(mgd, xml, force);

	if(objects == NULL) {
		
		list = PyTuple_New(i);
		return list;
	}

	while(objects[i] != NULL)
		i++;
	
	list = PyTuple_New(i);
	OBJECTS2LIST(objects, list);
	g_free(objects);
	
	return list;
}

int _py_midgard_convert_dbobject(PyObject *object, void *arg)
{
	PyObject *pclass = NULL, *pcname = NULL;
	int ret = 0;

	pclass = PyObject_GetAttrString(object, "__class__");
	if (!pclass) {

		PyErr_SetString(PyExc_TypeError, "Didn't find a class for given object.");
		return ret;
	}

	pcname = PyObject_GetAttrString(pclass, "__name__");
	if(!pcname) {

		PyErr_SetString(PyExc_TypeError, "Didn't find a class name for given object.");
		return ret;
	}

	GType type = g_type_from_name(PyString_AS_STRING(pcname));

	ret = 1;

	if(g_type_parent(type) != MIDGARD_TYPE_DBOBJECT) {

		if(g_type_parent(type) != MIDGARD_TYPE_OBJECT) {
		
			PyErr_SetString(PyExc_TypeError, "Expected midgard_dbobject (or derived) class instance.");
			ret = 0;
		
		} else {

			ret = 1;
		}
	}

	if(ret == 1) {

		if(G_OBJECT(((PyGObject *)object)->obj) == NULL) {
			
			PyErr_SetString(PyExc_TypeError, "Can not find underlying GObject object.");
			ret = 0;
		}
	}

	arg = (void *)MIDGARD_OBJECT(((PyGObject *)object)->obj);
	g_assert(arg != NULL);

	return ret;
}

static int __py_midgard_parse_dbobject(PyObject *object)
{
	PyObject *pclass = NULL, *pcname = NULL;
	int ret = 0;
	
	pclass = PyObject_GetAttrString(object, "__class__");
	if (!pclass) {

		PyErr_SetString(PyExc_TypeError, "Didn't find a class for given argument object.");
		return ret;
	}

	pcname = PyObject_GetAttrString(pclass, "__name__");
	if(!pcname) {

		PyErr_SetString(PyExc_TypeError, "Didn't find a class name for given argument object.");
		return ret;
	}

	GType type = g_type_from_name(PyString_AS_STRING(pcname));

	if(!type) {
		
		PyErr_SetString(PyExc_TypeError, "Expected argument object registered in GType system.");
		return ret;
	}

	if(g_type_parent(type) != MIDGARD_TYPE_DBOBJECT) {

		if(g_type_parent(type) != MIDGARD_TYPE_OBJECT) {
		
			PyErr_SetString(PyExc_TypeError, "Expected argument of type midgard_dbobject (or derived class).");
			ret = 0;
		
		} else {

			ret = 1;
		}
	}

	if(ret == 1) {

		if(G_OBJECT(((PyGObject *)object)->obj) == NULL) {
			
			PyErr_SetString(PyExc_TypeError, "Can not find underlying GObject object.");
			ret = 0;
		}
	}

	return ret;
}

static PyObject *
pymidgard_replicator_import_object(PyGObject *self, PyObject *args) 
{
	REPLICATOR_DEBUG("import_object");
	PyObject *pobject;
	gboolean force;

        // PyTypeObject mdbo = py_midgard_dbobject_get_type_object();

	if(!PyArg_ParseTuple(args, "O|b", &pobject, &force)) 
		return NULL;

	if(!__py_midgard_parse_dbobject(pobject))
		return NULL;

	gboolean imported =
		midgard_replicator_import_object(MIDGARD_DBOBJECT(((PyGObject *)pobject)->obj), force);
	
	if(imported)
		Py_RETURN_TRUE;
	
	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_replicator_import_from_xml(PyGObject *self, PyObject *args) 
{
	REPLICATOR_DEBUG("import_from_xml");
	const gchar *xml;
	gboolean force = FALSE;
	MidgardConnection *mgd = NULL;
	
	if(!PyArg_ParseTuple(args, "s|b", &xml, &force))
		return NULL;

	mgd = _py_midgard_connection_singleton_get();
	
	midgard_replicator_import_from_xml(mgd, xml, force);

	Py_RETURN_NONE;
}

static PyMethodDef pymidgard_replicator_methods[] = {
	{ "serialize", (PyCFunction)pymidgard_replicator_serialize, METH_VARARGS | METH_STATIC},
	{ "export", (PyCFunction)pymidgard_replicator_export, METH_VARARGS | METH_STATIC},
	{ "export_purged", (PyCFunction)pymidgard_replicator_export_purged, METH_VARARGS | METH_STATIC},
	{ "export_blob", (PyCFunction)pymidgard_replicator_export_blob, METH_VARARGS | METH_STATIC},
	{ "export_media", (PyCFunction)pymidgard_replicator_export_media, METH_VARARGS | METH_STATIC},
	{ "unserialize", (PyCFunction)pymidgard_replicator_unserialize, METH_VARARGS | METH_STATIC},
	{ "import_object", (PyCFunction)pymidgard_replicator_import_object, METH_VARARGS | METH_STATIC},
	{ "import_from_xml", (PyCFunction)pymidgard_replicator_import_from_xml, METH_VARARGS | METH_STATIC},
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_replicator_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "replicator",                   /* tp_name */
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
    pymidgard_replicator_methods, /* tp_methods */
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

void py_midgard_replicator_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"replicator", 
			MIDGARD_TYPE_REPLICATOR, 
			&Pymidgard_replicator_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
