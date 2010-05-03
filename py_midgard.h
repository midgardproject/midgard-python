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

#ifndef PY_MIDGARD_H
#define PY_MIDGARD_H

#ifndef PY_MIDGARD_H_INT
#define PY_MIDGARD_H_INT

#include <Python.h>
#include <pygobject.h>
#include <midgard/midgard.h>

#endif 

#ifdef G_LOG_DOMAIN
#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "midgard-core"
#endif

extern guint get_global_log_handler(void);
extern void py_midgard_dbobject_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_object_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_config_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_connection_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_query_builder_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_blob_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_collector_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_reflector_property_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_replicator_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_user_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_dbus_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_transaction_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_storage_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_view_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_key_config_context_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_key_config_file_context_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_reflector_object_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_schema_object_tree_register_class(PyObject *d, gpointer pygobject_type);
extern void py_midgard_schema_object_factory_register_class(PyObject *d, gpointer pygobject_type);


/* PARAMETERS */
extern PyObject *pymidgard_object_get_parameter(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_set_parameter(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_delete_parameters(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_purge_parameters(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_list_parameters(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_find_parameters(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_has_parameters(PyGObject *self, PyObject *args);

/* ATTACHMENTS */
extern PyObject *pymidgard_object_create_attachment(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_delete_attachments(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_purge_attachments(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_list_attachments(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_find_attachments(PyGObject *self, PyObject *args);
extern PyObject *pymidgard_object_has_attachments(PyGObject *self, PyObject *args);

/* GLOBALS */

/* MidgardConnection is defined as global value.
 * Singleton pattern should be kept for Apache environment */
extern MidgardConnection *MGDCG;

/* Types' pointers */
PyTypeObject py_midgard_dbobject_get_type_object(void);
PyTypeObject py_midgard_query_builder_get_type_object(void);
PyTypeObject py_midgard_collector_get_type_object(void);

extern void _py_midgard_connection_singleton_set(MidgardConnection *mgd);
extern MidgardConnection *_py_midgard_connection_singleton_get(void);
extern PyTypeObject *py_midgard_lookup_schema_type(const gchar *name);

/* GType routines */
extern GValue gvalue_from_pyobject(PyObject *obj);
extern gchar *_py_midgard_format_string(const gchar *msg , ...);
extern GParameter *_py_midgard_parameters_from_args(PyObject *args, guint *n_params);

void py_midgard_gvalue_from_pyobject(GValue *gvalue, const PyObject *pyvalue);
PyObject *py_midgard_pyobject_from_gvalue(const GValue *gvalue, gboolean copy_boxed);

/* Object routines */
PyObject *_py_midgard_get_object_attribute(PyObject *self, PyObject *attr);
int _py_midgard_set_object_attribute(PyObject *self, PyObject *attr, PyObject *value);
void _py_midgard_gobject_destructor (PyObject *self);

/* ROUTINES */
#define CHECK_MGD \
	if(1 < 0) { \
		PyTypeObject *tp = self->ob_type; \
		PyObject *__class = PyObject_GetAttrString((PyObject *)self, "__class__"); \
		PyObject *__name = PyObject_GetAttrString(__class, "__name__"); \
		PyObject *__string = PyObject_Str(__name); \
		Py_DECREF(__name); \
		g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, "%s.%s(...)", tp->tp_name, PyString_AS_STRING(__string)); \
	}

#define CLASS_METHOD_DEBUG(__classname, __name) \
	g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, "%s.%s(...)", __classname, __name); 
	
#define OBJECTS2LIST(__gobjects, __rlist)		\
	guint k = 0; 					\
	while(__gobjects[k] != NULL) { 			\
		PyObject *__pobject =			\
			Py_BuildValue("O", pygobject_new(G_OBJECT(__gobjects[k]))); \
		((PyGObject *)__pobject)->obj = G_OBJECT(__gobjects[k]); \
		PyTuple_SetItem(__rlist, k, __pobject);	\
		k++;					\
	}

#define FREE_PARAMETERS(_n_params, _params) \
	guint _i; \
	for(_i = 0; _i < _n_params; _i++) { \
		g_value_unset(&_params[_i].value); \
        } \
        g_free(_params); 

#endif  /* PY_MIDGARD_H */
