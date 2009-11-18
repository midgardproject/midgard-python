/* 
 * Copyright (C) 2009 Piotr Pokora <piotrek.pokora@gmail.com>
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

PyTypeObject G_GNUC_INTERNAL Pymidgard_key_config_Type;

#define KC_DEBUG(__name) \
	CLASS_METHOD_DEBUG(Pymidgard_key_config_Type.tp_name, __name);

static PyObject *
pymidgard_key_config_set_value (PyGObject *self, PyObject *args) 
{
	KC_DEBUG ("set_value");

	gchar *group;
	gchar *key;
	gchar *value;

	if(!PyArg_ParseTuple (args, "sss", &group, &key, &value))
		return NULL;
	
	MidgardKeyConfig *mck = MIDGARD_KEY_CONFIG (self->obj);

	midgard_key_config_set_value (mck, group, key, value);

	Py_RETURN_NONE;
}

static PyObject *
pymidgard_key_config_get_value (PyGObject *self, PyObject *args) 
{
	KC_DEBUG ("get_value");

	gchar *group;
	gchar *key;

	if(!PyArg_ParseTuple (args, "ss", &group, &key))
		return NULL;
	
	MidgardKeyConfig *mck = MIDGARD_KEY_CONFIG (self->obj);

	gchar *value = midgard_key_config_get_value (mck, group, key);
	PyObject *pval = Py_BuildValue ("s", value);
	g_free (value);

	return pval;
}

static PyObject *
pymidgard_key_config_set_comment (PyGObject *self, PyObject *args) 
{
	KC_DEBUG ("set_comment");

	gchar *group;
	gchar *key;
	gchar *comment;

	if(!PyArg_ParseTuple (args, "sss", &group, &key, &comment))
		return NULL;
	
	MidgardKeyConfig *mck = MIDGARD_KEY_CONFIG (self->obj);

	midgard_key_config_set_comment (mck, group, key, comment);

	Py_RETURN_NONE;
}

static PyObject *
pymidgard_key_config_get_comment (PyGObject *self, PyObject *args) 
{
	KC_DEBUG ("get_comment");

	gchar *group;
	gchar *key;

	if(!PyArg_ParseTuple (args, "ss", &group, &key))
		return NULL;
	
	MidgardKeyConfig *mck = MIDGARD_KEY_CONFIG (self->obj);

	gchar *comment = midgard_key_config_get_comment (mck, group, key);
	PyObject *pval = Py_BuildValue ("s", comment);
	g_free (comment);

	return pval;
}

static PyObject *
pymidgard_key_config_list_groups (PyGObject *self, PyObject *args) 
{
	KC_DEBUG ("list_groups");

	gint n_grps;
	gchar **grps = midgard_key_config_list_groups (MIDGARD_KEY_CONFIG (self->obj), &n_grps);
	PyObject *list = PyTuple_New (n_grps);
	guint i;

	if (grps == NULL)
		return list;

	for (i = 0; i < n_grps; i++) {

		PyObject *strname = PyString_FromString (grps[i]);
		PyTuple_SetItem (list, i, strname);
	}

	g_strfreev (grps);

	return list;
}

static PyObject *
pymidgard_key_config_group_exists (PyGObject *self, PyObject *args) 
{
	KC_DEBUG ("group_exists");

	gchar *group;

	if(!PyArg_ParseTuple (args, "s", &group))
		return NULL;

	MidgardKeyConfig *mkc = MIDGARD_KEY_CONFIG (self->obj);

	if (midgard_key_config_group_exists (mkc, group))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_key_config_delete_group (PyGObject *self, PyObject *args) 
{
	KC_DEBUG ("delete_group");
	gchar *group;

	if(!PyArg_ParseTuple (args, "s", &group))
		return NULL;

	MidgardKeyConfig *mkc = MIDGARD_KEY_CONFIG (self->obj);

	if (midgard_key_config_delete_group (mkc, group))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_key_config_store (PyGObject *self, PyObject *args) 
{
	KC_DEBUG ("store");

	MidgardKeyConfig *mkc = MIDGARD_KEY_CONFIG (self->obj);

	if (midgard_key_config_store (mkc))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_key_config_to_data (PyGObject *self, PyObject *args) 
{
	KC_DEBUG ("to_data");

	MidgardKeyConfig *mkc = MIDGARD_KEY_CONFIG (self->obj);

	gchar *data = midgard_key_config_to_data (mkc);
	PyObject *ret = Py_BuildValue("s", data);
	g_free(data);

	return ret;
}

static PyObject *
pymidgard_key_config_load_from_data (PyGObject *self, PyObject *args) 
{
	KC_DEBUG ("load_from_data");

	gchar *data;

	if(!PyArg_ParseTuple (args, "s", &data))
		return NULL;

        MidgardKeyConfig *mkc = MIDGARD_KEY_CONFIG (self->obj);

	if (midgard_key_config_load_from_data (mkc, data))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyMethodDef pymidgard_key_config_methods[] = {
	{ "set_value", 	(PyCFunction)pymidgard_key_config_set_value, 		METH_VARARGS },
	{ "get_value", 		(PyCFunction)pymidgard_key_config_get_value, 	METH_VARARGS },
	{ "set_comment", 	(PyCFunction)pymidgard_key_config_set_comment, 	METH_VARARGS },
	{ "set_comment", 	(PyCFunction)pymidgard_key_config_get_comment, 	METH_VARARGS },
	{ "list_groups", 	(PyCFunction)pymidgard_key_config_list_groups, 	METH_NOARGS },
	{ "group_exists", 	(PyCFunction)pymidgard_key_config_group_exists,	METH_VARARGS },
	{ "delete_group",	(PyCFunction)pymidgard_key_config_delete_group, METH_VARARGS },
	{ "store", 		(PyCFunction)pymidgard_key_config_store, 	METH_NOARGS },
	{ "to_data", 		(PyCFunction)pymidgard_key_config_to_data, 	METH_NOARGS },
	{ "load_from_data", 	(PyCFunction)pymidgard_key_config_load_from_data, METH_VARARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_key_config_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "key_config",                   /* tp_name */
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
    "MidgardKeyConfig abstract class",                        /* Documentation string */
    (traverseproc)0,     /* tp_traverse */
    (inquiry)0,             /* tp_clear */
    (richcmpfunc)0,   /* tp_richcompare */
    offsetof(PyGObject, weakreflist),             /* tp_weaklistoffset */
    (getiterfunc)0,          /* tp_iter */
    (iternextfunc)0,     /* tp_iternext */
    pymidgard_key_config_methods, /* tp_methods */
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

void 
py_midgard_key_config_register_class (PyObject *d, gpointer ot)
{
	pygobject_register_class (d, 
			"key_config", 
			MIDGARD_TYPE_KEY_CONFIG, 
			&Pymidgard_key_config_Type, NULL);
			//Py_BuildValue ("(O)", ot));

}

PyTypeObject 
py_midgard_key_config_get_type_object (void)
{
	return Pymidgard_key_config_Type;
}

