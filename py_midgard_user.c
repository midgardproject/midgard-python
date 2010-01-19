/* 
 * Copyright (C) 2007, 2009 Piotr Pokora <piotrek.pokora@gmail.com>
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

PyTypeObject G_GNUC_INTERNAL Pymidgard_user_Type;

#define USER_DEBUG(__name) \
	CHECK_MGD; \
        CLASS_METHOD_DEBUG(Pymidgard_user_Type.tp_name, __name);

static int __pyobject_to_person(PyObject *arg, gpointer *ptr)
{
	if(!PyObject_IsTrue(arg))
		goto THROW_EXCEPTION;

	PyGObject *pobj = (PyGObject *)arg;
	if(pobj && pobj->obj != NULL) {
		
		if(!G_IS_OBJECT(pobj->obj))
			goto THROW_EXCEPTION;

		const gchar *cname = 
			G_OBJECT_TYPE_NAME(G_OBJECT(pobj->obj));	
		if(!g_str_equal(cname, "midgard_person"))
			goto THROW_EXCEPTION;
			
	} else {
		
		goto THROW_EXCEPTION;
	}

	return 1;

THROW_EXCEPTION:
	PyErr_SetString(PyExc_TypeError, "Expected object");
	return 0;
}

static int
__user_constructor(PyGObject *self, PyObject *args, PyObject *kwargs)
{
	USER_DEBUG("__init__");

	PyObject *props = NULL;
	if(!PyArg_ParseTuple(args, "|O", &props))
		return -1;

	guint n_params;
	GParameter *params = _py_midgard_parameters_from_args(props, &n_params);
	MidgardConnection *mgd = _py_midgard_connection_singleton_get();

	MidgardUser *user = midgard_user_new (mgd, n_params, params);
	FREE_PARAMETERS (n_params, params);

	if(!user) return -1;

	self->obj = G_OBJECT(user);

	return 0;
}

static PyObject *
pymidgard_user_auth(PyGObject *self, PyObject *args) 
{
	USER_DEBUG("auth");
	const gchar *login, *pass, *sitegroup;
	gboolean trusted = FALSE;
	if(!PyArg_ParseTuple(args, "ss|zb", &login, &pass, &sitegroup, &trusted))
		return NULL;

	MidgardConnection *mgd = 
		_py_midgard_connection_singleton_get();

	MidgardUser *user = 
		midgard_user_auth(mgd, login, pass, sitegroup, trusted);

	if(user == NULL)
		Py_RETURN_NONE;
	
	GValue gval = {0, };
	g_value_init(&gval, G_TYPE_OBJECT);
	g_value_set_object(&gval, G_OBJECT(user));
	PyObject *pval = pyg_value_as_pyobject((const GValue *)&gval, FALSE);
	
	return pval;
}

static PyObject *
pymidgard_user_is_user(PyGObject *self, PyObject *args)
{
	USER_DEBUG("is_user");
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardUser *user = MIDGARD_USER(self->obj);

	g_assert(user != NULL);

	if(midgard_user_is_user(user))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_user_is_admin(PyGObject *self, PyObject *args)
{
	USER_DEBUG("is_admin");
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardUser *user = MIDGARD_USER(self->obj);

	g_assert(user != NULL);

	if(midgard_user_is_admin(user))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_user_set_active(PyGObject *self, PyObject *args)
{
	USER_DEBUG("set_active");
	gboolean flag = FALSE;
	if(!PyArg_ParseTuple(args, "b", &flag))
		return NULL;

	MidgardUser *user = MIDGARD_USER(self->obj);

	g_assert(user != NULL);

	if(midgard_user_set_active(user, flag))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_user_get_person(PyGObject *self, PyObject *args)
{
	USER_DEBUG("get_person");
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	
	MidgardObject *person = midgard_user_get_person(MIDGARD_USER(self->obj));
	
	if(person == NULL)
		Py_RETURN_NONE;

	GValue gval = {0, };
	g_value_init(&gval, G_TYPE_OBJECT);
	g_value_set_object(&gval, G_OBJECT(person));
	PyObject *pval = pyg_value_as_pyobject((const GValue *)&gval, FALSE);
	
	return pval;
}

static PyObject *
pymidgard_user_get(PyGObject *self, PyObject *args)
{
	USER_DEBUG("get");

	PyObject *props;
	if(!PyArg_ParseTuple(args, "O", &props))
		return NULL;

	guint n_params;
	GParameter *params = _py_midgard_parameters_from_args(props, &n_params);
	MidgardConnection *mgd = _py_midgard_connection_singleton_get();

	MidgardUser *user = midgard_user_get (mgd, n_params, params);
	FREE_PARAMETERS (n_params, params);

	if(user == NULL)
		Py_RETURN_NONE;

	GValue gval = {0, };
	g_value_init(&gval, G_TYPE_OBJECT);
	g_value_set_object(&gval, G_OBJECT(user));
	PyObject *pval = pyg_value_as_pyobject((const GValue *)&gval, FALSE);

	return pval;
}

static PyObject *
pymidgard_user_log_in(PyGObject *self, PyObject *args)
{
	USER_DEBUG("login");
g_print ("LOGIN ");	
	MidgardUser *user = MIDGARD_USER(self->obj);

	if(midgard_user_login(user))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_user_logout(PyGObject *self, PyObject *args)
{
	USER_DEBUG("logout");
	
	MidgardUser *user = MIDGARD_USER(self->obj);

	if(midgard_user_logout(user))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_user_query(PyGObject *self, PyObject *args)
{
	USER_DEBUG("query");
      
       	PyObject *props;	
        if(!PyArg_ParseTuple(args, "O", &props))
	                return NULL;

        // MidgardObject *mobject = MIDGARD_OBJECT(self->obj);
        guint n_params;
        GParameter *params = _py_midgard_parameters_from_args(props, &n_params);
	MidgardConnection *mgd = _py_midgard_connection_singleton_get();
        
	MidgardUser **users = midgard_user_query(mgd, n_params, params);
        FREE_PARAMETERS(n_params, params);

	PyObject *list = PyTuple_New(n_params);

        if(!users)
		return list;
	                
        OBJECTS2LIST(users, list);
        g_free(users);

        return list;
}

static PyObject *
pymidgard_user_create(PyGObject *self, PyObject *args)
{
	USER_DEBUG("create");

	MidgardUser *user = MIDGARD_USER(self->obj);

	if(midgard_user_create(user))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_user_update(PyGObject *self, PyObject *args)
{
	USER_DEBUG("update");

	MidgardUser *user = MIDGARD_USER(self->obj);

	if(midgard_user_update(user))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_user_delete (PyGObject *self, PyObject *args)
{
	USER_DEBUG ("delete");

	MidgardUser *user = MIDGARD_USER (self->obj);

	if(midgard_user_delete (user))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyObject *
pymidgard_user_set_person(PyGObject *self, PyObject *args)
{
	USER_DEBUG("set_person");

	PyObject *pobject;
	if(!PyArg_ParseTuple(args, "O", &pobject))
		return NULL;

	MidgardUser *user = MIDGARD_USER(self->obj);
	MidgardObject *person = MIDGARD_OBJECT (((PyGObject *)pobject)->obj);

	if (!person)
		Py_RETURN_FALSE;

	if (midgard_user_set_person(user, person))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

static PyMethodDef pymidgard_user_methods[] = {
	{ "get", (PyCFunction)pymidgard_user_get, METH_VARARGS | METH_STATIC },
	{ "log_in", (PyCFunction)pymidgard_user_log_in, METH_NOARGS },
	{ "logout", (PyCFunction)pymidgard_user_logout, METH_NOARGS },
	{ "query", (PyCFunction)pymidgard_user_query, METH_VARARGS | METH_STATIC },
	{ "create", (PyCFunction)pymidgard_user_create, METH_NOARGS },
	{ "update", (PyCFunction)pymidgard_user_update, METH_NOARGS },
	{ "delete", (PyCFunction)pymidgard_user_delete, METH_NOARGS },
	{ "auth", (PyCFunction)pymidgard_user_auth, METH_VARARGS | METH_STATIC },
	{ "is_user", (PyCFunction)pymidgard_user_is_user, METH_VARARGS },
	{ "is_admin", (PyCFunction)pymidgard_user_is_admin, METH_VARARGS },
	{ "set_active", (PyCFunction)pymidgard_user_set_active, METH_VARARGS },
	{ "set_person", (PyCFunction)pymidgard_user_set_person, METH_VARARGS },
	{ "get_person", (PyCFunction)pymidgard_user_get_person, METH_VARARGS },
	{ NULL, NULL, 0 }
};

PyTypeObject G_GNUC_INTERNAL Pymidgard_user_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                 /* ob_size */
    "user",                   /* tp_name */
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
    pymidgard_user_methods, /* tp_methods */
    (struct PyMemberDef*)0,              /* tp_members */
    (struct PyGetSetDef*)0,  /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    (descrgetfunc)0,    /* tp_descr_get */
    (descrsetfunc)0,    /* tp_descr_set */
    offsetof(PyGObject, inst_dict),                 /* tp_dictoffset */
    (initproc)__user_constructor,              /* tp_init */
    (allocfunc)0,           /* tp_alloc */
    (newfunc)0,               /* tp_new */
    (freefunc)0,             /* tp_free */
    (inquiry)0              /* tp_is_gc */
};

void py_midgard_user_register_class(
		PyObject *d, gpointer pygobject_type)
{
	pygobject_register_class(d, 
			"user", 
			MIDGARD_TYPE_USER, 
			&Pymidgard_user_Type, 
			Py_BuildValue("(O)", pygobject_type));
}
