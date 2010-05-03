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

#define G_LOG_DOMAIN "midgard-core"

#include "py_midgard.h"
#include <datetime.h>

static PyTypeObject *_MPyGObject_Type;
#define PyGObject_Type (*_MPyGObject_Type)

void py_midgard_init(void);
static MidgardSchema *midgard_global_schema = NULL;

static guint global_loghandler = 0;

void
py_midgard_register_classes(PyObject *d)
{
	PyObject *module;
	
	if ((module = PyImport_ImportModule("gobject")) != NULL) {
		
		_MPyGObject_Type = (PyTypeObject *)PyObject_GetAttrString(module, "GObject");
		
		if (_MPyGObject_Type == NULL) {
			
			PyErr_SetString(PyExc_ImportError,
					"cannot import name GObject from gobject");
			return ;
		}
	
	} else {
		
		PyErr_SetString(PyExc_ImportError, "could not import gobject");
		
		return ;
	}

	py_midgard_config_register_class(d, &PyGObject_Type);
	py_midgard_connection_register_class(d, &PyGObject_Type);
	py_midgard_query_builder_register_class(d, &PyGObject_Type);
	py_midgard_blob_register_class(d, &PyGObject_Type);
	py_midgard_reflection_property_register_class(d, &PyGObject_Type);
	py_midgard_replicator_register_class(d, &PyGObject_Type);
	py_midgard_dbus_register_class(d, &PyGObject_Type);
	py_midgard_transaction_register_class(d, &PyGObject_Type);
	py_midgard_storage_register_class(d, &PyGObject_Type);
	py_midgard_reflector_object_register_class(d, &PyGObject_Type);
	py_midgard_schema_object_tree_register_class(d, &PyGObject_Type);
	py_midgard_schema_object_factory_register_class(d, &PyGObject_Type);

	PyTypeObject pto = py_midgard_query_builder_get_type_object();

	if(&pto == NULL)
		g_warning("Can not handle query builder type pointer");

	PyObject *mmodule = PyImport_ImportModule("_midgard");
	PyTypeObject *qbot = (PyTypeObject *)PyObject_GetAttrString(mmodule, "query_builder");
	py_midgard_collector_register_class(d, &qbot);

	py_midgard_key_config_context_register_class(d, &PyGObject_Type);
	PyTypeObject *kccot = (PyTypeObject *)PyObject_GetAttrString(mmodule, "key_config_context");
	py_midgard_key_config_file_context_register_class(d, &kccot);
}

static void
py_midgard_register_schema_classes(PyObject *d)
{
	PyObject *module;
		
	if ((module = PyImport_ImportModule("gobject")) != NULL) {
		
		_MPyGObject_Type = (PyTypeObject *)PyObject_GetAttrString(module, "GObject");
		
		if (_MPyGObject_Type == NULL) {
			
			PyErr_SetString(PyExc_ImportError,
					"cannot import name GObject from gobject");
			return ;
		}
	
	} else {
		
		PyErr_SetString(PyExc_ImportError, "could not import gobject");
		
		return ;
	}

	py_midgard_object_register_class(d, &PyGObject_Type);
}

static void
py_midgard_register_view_classes(PyObject *d)
{
	PyObject *module;
		
	if ((module = PyImport_ImportModule("gobject")) != NULL) {
		
		_MPyGObject_Type = (PyTypeObject *)PyObject_GetAttrString(module, "GObject");
		
		if (_MPyGObject_Type == NULL) {
			
			PyErr_SetString(PyExc_ImportError,
					"cannot import name GObject from gobject");
			return ;
		}
	
	} else {
		
		PyErr_SetString(PyExc_ImportError, "could not import gobject");
		
		return ;
	}

	py_midgard_view_register_class(d, &PyGObject_Type);
}

void
py_midgard_register_db_classes(PyObject *d)
{
	PyObject *module;
	
	if ((module = PyImport_ImportModule("gobject")) != NULL) {
		
		_MPyGObject_Type = (PyTypeObject *)PyObject_GetAttrString(module, "GObject");
		
		if (_MPyGObject_Type == NULL) {
			
			PyErr_SetString(PyExc_ImportError,
					"cannot import name GObject from gobject");
			return ;
		}
	
	} else {
		
		PyErr_SetString(PyExc_ImportError, "could not import gobject");
		
		return ;
	}
	
	py_midgard_dbobject_register_class(d, &PyGObject_Type);
	py_midgard_user_register_class(d, &PyGObject_Type);
}

static const PyMethodDef py_midgard_functions[] = {
	{ NULL, NULL, 0, NULL }
};

static void py_midgard_define_constants(PyObject *m)
{
	/* Register properties' midgard types */
	PyModule_AddIntConstant(m, "TYPE_NONE", MGD_TYPE_NONE);
	PyModule_AddIntConstant(m, "TYPE_STRING", MGD_TYPE_STRING);
	PyModule_AddIntConstant(m, "TYPE_INT", MGD_TYPE_INT);
	PyModule_AddIntConstant(m, "TYPE_UINT", MGD_TYPE_UINT);
	PyModule_AddIntConstant(m, "TYPE_FLOAT", MGD_TYPE_FLOAT);
	PyModule_AddIntConstant(m, "TYPE_BOOLEAN", MGD_TYPE_BOOLEAN);
	PyModule_AddIntConstant(m, "TYPE_TIMESTAMP", MGD_TYPE_TIMESTAMP);
	PyModule_AddIntConstant(m, "TYPE_LONGTEXT", MGD_TYPE_LONGTEXT);
	PyModule_AddIntConstant(m, "TYPE_GUID", MGD_TYPE_GUID);

	/* Register midgard_user types */
	PyModule_AddIntConstant(m, "USER_TYPE_NONE", MIDGARD_USER_TYPE_NONE);
	PyModule_AddIntConstant(m, "USER_TYPE_USER", MIDGARD_USER_TYPE_USER);
	PyModule_AddIntConstant(m, "USER_TYPE_ADMIN", MIDGARD_USER_TYPE_ADMIN);

	/* Register errcode constants */
	PyModule_AddIntConstant(m, "ERR_OK", MGD_ERR_OK);
        PyModule_AddIntConstant(m, "ERR_ERROR", MGD_ERR_ERROR);
        PyModule_AddIntConstant(m, "ERR_ACCESS_DENIED", MGD_ERR_ACCESS_DENIED);
	PyModule_AddIntConstant(m, "ERR_NO_METADATA", MGD_ERR_NO_METADATA);
        PyModule_AddIntConstant(m, "ERR_NOT_OBJECT", MGD_ERR_NOT_OBJECT);
        PyModule_AddIntConstant(m, "ERR_NOT_EXISTS", MGD_ERR_NOT_EXISTS);
        PyModule_AddIntConstant(m, "ERR_INVALID_NAME", MGD_ERR_INVALID_NAME);
        PyModule_AddIntConstant(m, "ERR_DUPLICATE", MGD_ERR_DUPLICATE);
        PyModule_AddIntConstant(m, "ERR_HAS_DEPENDANTS", MGD_ERR_HAS_DEPENDANTS);
        PyModule_AddIntConstant(m, "ERR_RANGE", MGD_ERR_RANGE);
        PyModule_AddIntConstant(m, "ERR_NOT_CONNECTED", MGD_ERR_NOT_CONNECTED);
        PyModule_AddIntConstant(m, "ERR_SG_NOTFOUND", MGD_ERR_SG_NOTFOUND);
        PyModule_AddIntConstant(m, "ERR_INVALID_OBJECT", MGD_ERR_INVALID_OBJECT);
        PyModule_AddIntConstant(m, "ERR_QUOTA", MGD_ERR_QUOTA);
        PyModule_AddIntConstant(m, "ERR_INTERNAL", MGD_ERR_INTERNAL);
        PyModule_AddIntConstant(m, "ERR_OBJECT_NAME_EXISTS", MGD_ERR_OBJECT_NAME_EXISTS);
        PyModule_AddIntConstant(m, "ERR_OBJECT_NO_STORAGE", MGD_ERR_OBJECT_NO_STORAGE);
        PyModule_AddIntConstant(m, "ERR_OBJECT_NO_PARENT", MGD_ERR_OBJECT_NO_PARENT);
        PyModule_AddIntConstant(m, "ERR_INVALID_PROPERTY_VALUE", MGD_ERR_INVALID_PROPERTY_VALUE);
	PyModule_AddIntConstant(m, "ERR_INVALID_PROPERTY", MGD_ERR_INVALID_PROPERTY);
        PyModule_AddIntConstant(m, "ERR_USER_DATA", MGD_ERR_USER_DATA);
        PyModule_AddIntConstant(m, "ERR_OBJECT_DELETED", MGD_ERR_OBJECT_DELETED);
        PyModule_AddIntConstant(m, "ERR_OBJECT_PURGED", MGD_ERR_OBJECT_PURGED);
        PyModule_AddIntConstant(m, "ERR_OBJECT_EXPORTED", MGD_ERR_OBJECT_EXPORTED);
        PyModule_AddIntConstant(m, "ERR_OBJECT_IMPORTED", MGD_ERR_OBJECT_IMPORTED);
        PyModule_AddIntConstant(m, "ERR_MISSED_DEPENDENCE", MGD_ERR_MISSED_DEPENDENCE);
        PyModule_AddIntConstant(m, "ERR_TREE_IS_CIRCULAR", MGD_ERR_TREE_IS_CIRCULAR);
}

DL_EXPORT(void)
init_midgard(void)
{
	PyObject *m, *d, *tuple, *sm, *dm, *vm;

	/* initialise pygobject */
	init_pygobject_check(2, 12, 0);
	g_assert(pygobject_register_class != NULL);

	midgard_init();

	global_loghandler =
		g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_MASK,
				midgard_error_default_log, NULL);

	/* Import required datatime module */
	PyObject *module = PyImport_ImportModule("datetime");
	if (module == NULL) 
		g_error("Failed to load datetime module. Required by midgard_timestamp."); 

	if(!g_type_from_name("midgard_language")
			&& midgard_global_schema == NULL) {
		
		/* FIXME, we need to unref it when module is unloaded! */ 
		midgard_global_schema = g_object_new(MIDGARD_TYPE_SCHEMA, NULL);
		midgard_schema_init((MidgardSchema *) midgard_global_schema, NULL);
		midgard_schema_read_dir((MidgardSchema *) midgard_global_schema, NULL);
	}

	m = Py_InitModule("_midgard", (PyMethodDef*)py_midgard_functions);
	
	if(!m) g_warning("InitModule FAILED");

	d = PyModule_GetDict(m);

	if(!d) g_warning("Module_GetDict FAILED");

	tuple = Py_BuildValue("s", midgard_version());
	PyDict_SetItemString(d, "version", tuple);
	Py_DECREF(tuple);

	tuple = Py_BuildValue("(iii)", 0, 0, 1);
	PyDict_SetItemString(d, "module_version", tuple);
	Py_DECREF(tuple);

	py_midgard_define_constants(m);
	py_midgard_register_classes(d);
	
	/* Core DB objects */
	dm = Py_InitModule("__db__", NULL);
	d = PyModule_GetDict(dm);
	PyModule_AddObject(m, "db", dm);
	py_midgard_register_db_classes(d);

	/* Mgdschema module */
	sm = Py_InitModule("__mgdschema__", NULL);
	if(!sm) 
		g_warning("Failed to initialize mgdschema module");
	PyObject *sd = PyModule_GetDict(sm);
	PyModule_AddObject(m, "mgdschema", sm);
	py_midgard_register_schema_classes(sd);
	
	/* Mgdschema view module */
	vm = Py_InitModule("__view__", NULL);
	if(!vm) 
		g_warning("Failed to initialize mgdschema view module");
	PyObject *vd = PyModule_GetDict(vm);
	PyModule_AddObject(m, "view", vm);
	py_midgard_register_view_classes(vd);

	/* Globals */

	/* Set null connection "superglobal" */
	tuple = Py_BuildValue("i", 0);
	d = PyModule_GetDict(m);
	PyDict_SetItemString(d, "_connection", tuple);
}

/* ROUTINES */

guint get_global_log_handler(void)
{
	return global_loghandler;
}

GValue gvalue_from_pyobject(PyObject *pyval)
{
	GValue value = {0, };
	
	if(PyString_Check(pyval)) {
		g_value_init(&value, G_TYPE_STRING);
		return value;
	
	} else 	if (PyInt_Check(pyval)) {
		g_value_init(&value, G_TYPE_INT);
		return value;
	
	} else if (PyLong_Check(pyval)) {
		g_value_init(&value, G_TYPE_INT);
		return value;
	
	} else if (PyBool_Check(pyval)) {
		g_value_init(&value, G_TYPE_BOOLEAN);
		return value;
	
	} else if (PyFloat_Check(pyval)) {
		g_value_init(&value, G_TYPE_FLOAT);
		return value;
		
	/* I have no idea how to check if given PyObject is an instance of *some* class.
   	 * PyInstance_Check and PyCObject_Check doesn't work */	   
	} else if (PyObject_TypeCheck(pyval, &PyGObject_Type)) {
		g_value_init(&value, G_TYPE_OBJECT);
		return value;

	/* We will initialize correct GType a bit later. For lists and tuples it's probably array. */ 
	} else if (PyList_Check(pyval)) {
		/* g_value_init(&value, G_TYPE_BOXED); */ /* DO NOT INITIALIZE BOXED TYPE */
		return value;
	
	} else if (PyTuple_Check(pyval)) {
		/* g_value_init(&value, G_TYPE_BOXED); */ /* DO NOT INITIALIZE BOXED TYPE */
		return value;

	} else if (PyUnicode_Check(pyval)) {
		g_value_init(&value, G_TYPE_STRING);
		return value;

	} else if (PyDateTime_Check(pyval)) {
		g_value_init(&value, MGD_TYPE_TIMESTAMP);
		return value;

	} else {
		g_warning("Unhandled value type");
		g_value_init(&value, G_TYPE_NONE);
		return value;
	}
}

PyObject *py_midgard_pyobject_from_gvalue(const GValue *gvalue, gboolean copy_boxed)
{
	g_return_val_if_fail(gvalue != NULL, NULL);

	PyObject *pyobject = NULL;

	if (G_VALUE_TYPE(gvalue) == MGD_TYPE_TIMESTAMP) {
	
		/* WARNING! */
		/* Whenever user reads timestamp property we return newly allocated PyObject. */
		MidgardTimestamp *mt = (MidgardTimestamp *) g_value_get_boxed(gvalue);
		
		PyDateTime_IMPORT;
		pyobject= PyDateTime_FromDateAndTime(mt->year, mt->month, mt->day,
				mt->hour, mt->minute, mt->second, 0);
		
	} else {

		pyobject = pyg_value_as_pyobject((const GValue *)gvalue, copy_boxed);
	}

	return pyobject;
}

void py_midgard_gvalue_from_pyobject(GValue *gvalue, const PyObject *pyvalue)
{
	g_return_if_fail(pyvalue != NULL);
	g_return_if_fail(gvalue != NULL);

	GValueArray *array = NULL;
	guint elements = 0;
	guint i = 0;

	if (PyTuple_Check(pyvalue)) {
		
		/* TODO, validate gvalue */

		elements = (guint) PyTuple_Size((PyObject *)pyvalue);
		array = g_value_array_new(elements);

		for (i = 0; i < elements; i++) {
			
			PyObject *pval = PyTuple_GetItem((PyObject *)pyvalue, i); 
			GValue gval = gvalue_from_pyobject(pval);
			py_midgard_gvalue_from_pyobject(&gval, pval);
			g_value_array_append(array, &gval);
			g_value_unset(&gval);
		}

		g_value_init(gvalue, G_TYPE_VALUE_ARRAY);
		g_value_take_boxed(gvalue, array);
	}

	if (PyList_Check(pyvalue)) {
		
		/* TODO, validate gvalue */

		elements = (guint) PyList_Size((PyObject *)pyvalue);
		array = g_value_array_new(elements);

		for (i = 0; i < elements; i++) {
			
			PyObject *pval = PyList_GetItem((PyObject *)pyvalue, i); 
			GValue gval = gvalue_from_pyobject(pval);
			py_midgard_gvalue_from_pyobject(&gval, pval);
			g_value_array_append(array, &gval);
			g_value_unset(&gval);
		}

		g_value_init(gvalue, G_TYPE_VALUE_ARRAY);
		g_value_take_boxed(gvalue, array);
	}

	if (G_VALUE_TYPE(gvalue) == MGD_TYPE_TIMESTAMP) {
	
		MidgardTimestamp *mt = (MidgardTimestamp *) g_value_get_boxed(gvalue);
		
		PyDateTime_IMPORT;
		mt->year = PyDateTime_GET_YEAR(pyvalue);
		mt->month = PyDateTime_GET_MONTH(pyvalue);
		mt->day = PyDateTime_GET_DAY(pyvalue);
		mt->hour = PyDateTime_DATE_GET_HOUR(pyvalue);
		mt->minute = PyDateTime_DATE_GET_MINUTE(pyvalue);
		mt->second = PyDateTime_DATE_GET_SECOND(pyvalue);

	} else {

		pyg_value_from_pyobject(gvalue, (PyObject *)pyvalue);
	}

	return;
}

GParameter *_py_midgard_parameters_from_args(PyObject *args, guint *n_params)
{
	if(!args) {

		*n_params = 0;
		return NULL;
	}

	gint i = 0;
	GParameter *params = NULL;
	
	*n_params = (guint) PyDict_Size(args);

	if(*n_params < 1)
		return NULL;

#if PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION == 4
	int pos = 0;
#elif PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION > 4
	Py_ssize_t pos = 0;
#endif
	PyObject *key;
	PyObject *value;

	params = g_new0(GParameter, *n_params);

	while (PyDict_Next (args, &pos, &key, &value)) {
		
		const gchar *key_str = PyString_AsString (key);
		params[i].name = key_str;

		params[i].value = gvalue_from_pyobject(value);
		pyg_value_from_pyobject(&params[i].value, value);

		i++;
	}

	return params;
}

gchar *
_py_midgard_format_string (const gchar *msg , ...)    
{
	va_list args;
	va_start(args, msg);
	gchar *_msg = g_strdup_vprintf(msg, args);
	va_end(args);
	
	return _msg;
}

void 
_py_midgard_gobject_destructor (PyObject *self)
{	
	PyGObject *pygobj = (PyGObject *) self;
	if (pygobj->obj && G_IS_OBJECT (pygobj->obj))
		g_object_unref (pygobj->obj);

	PyObject_GC_Del (self);
	//_Py_ForgetReference (self);
}
