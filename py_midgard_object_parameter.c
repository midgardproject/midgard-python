/* 
 * Copyright (C) 2007, 2008 Piotr Pokora <piotrek.pokora@gmail.com>
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

#define PARAM_DEBUG(__name) \
	CHECK_MGD; \
	const gchar *cname = NULL; \
	if(self) { \
		CLASS_METHOD_DEBUG(G_OBJECT_TYPE_NAME(G_OBJECT(self->obj)), __name); \
	} \
	if(cname == NULL) \
		CLASS_METHOD_DEBUG("midgard_dbobject", __name);

#define _FREE_PARAMETERS \
	        guint _i; \
        for(_i = 0; _i < n_params; _i++) { \
		                g_value_unset(&params[_i].value); \
		        } \
        g_free(params); 

PyObject *
pymidgard_object_get_parameter(PyGObject *self, PyObject *args) 
{
	PARAM_DEBUG("get_parameter");
	const gchar *domain = NULL, *name = NULL;

	if(!PyArg_ParseTuple(args, "ss", &domain, &name))
		return NULL;

	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);

	const GValue *gval = 
		midgard_object_get_parameter(mobject, domain, name);

	if(gval == NULL)
		Py_RETURN_NONE;

	PyObject *ret = pyg_value_as_pyobject(gval, FALSE);

	return ret;
}

PyObject *
pymidgard_object_set_parameter(PyGObject *self, PyObject *args) 
{
	PARAM_DEBUG("set_parameter");
	const gchar *domain = NULL, *name = NULL;
	PyObject *pvalue;
	GValue *cvalue = NULL;

	if(!PyArg_ParseTuple(args, "ss|O", &domain, &name, &pvalue))
		                return NULL;

	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);
	
	if(pvalue != NULL) {
		
		GValue gval = gvalue_from_pyobject(pvalue);
		pyg_value_from_pyobject(&gval, pvalue);
		cvalue = g_new0(GValue, 1);
		g_value_init(cvalue, G_VALUE_TYPE(&gval));
		g_value_copy((const GValue *) &gval, cvalue);
		g_value_unset(&gval);
	}
	
	if(midgard_object_set_parameter(mobject, domain, name, cvalue))
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

PyObject *
pymidgard_object_has_parameters(PyGObject *self, PyObject *args) 
{
	PARAM_DEBUG("has_parameters");
	
	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);
	
	gboolean rv = midgard_object_has_parameters(mobject);

	if(rv)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

PyObject *
pymidgard_object_delete_parameters(PyGObject *self, PyObject *args) 
{
	PARAM_DEBUG("delete_parameters");

	PyObject *props;
	
	if(!PyArg_ParseTuple(args, "O", &props))
		return NULL;

	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);

	guint n_params;
	GParameter *params = _py_midgard_parameters_from_args(props, &n_params);
	
	gboolean rv = midgard_object_delete_parameters(mobject, n_params, params);

	_FREE_PARAMETERS;

	if(rv)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

PyObject *
pymidgard_object_purge_parameters(PyGObject *self, PyObject *args) 
{
	PARAM_DEBUG("purge_parameters");

	PyObject *props;
	if(!PyArg_ParseTuple(args, "O", &props))
		return NULL;

	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);

	guint n_params;
	GParameter *params = _py_midgard_parameters_from_args(props, &n_params);
	
	gboolean rv = midgard_object_purge_parameters(mobject, n_params, params);

	_FREE_PARAMETERS;

	if(rv)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

PyObject *
pymidgard_object_list_parameters(PyGObject *self, PyObject *args) 
{
	PARAM_DEBUG("list_parameters");

	gchar *domain = NULL;
	guint i = 0;
	
	if(!PyArg_ParseTuple(args, "|s", &domain))
		return NULL;

	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);

	MidgardObject **objects = midgard_object_list_parameters(mobject, domain);

	if(!objects)
		return PyTuple_New(i);

	while(objects[i] != NULL)
		i++;
	
	PyObject *list = PyTuple_New(i);
	
	OBJECTS2LIST(objects, list);
	
	g_free(objects);
	
	return list;
}

PyObject *
pymidgard_object_find_parameters(PyGObject *self, PyObject *args) 
{
	PARAM_DEBUG("find_parameters");

	PyObject *props;

	if(!PyArg_ParseTuple(args, "O", &props))
		return NULL;
	
	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);

	guint n_params, i = 0;
	GParameter *params = _py_midgard_parameters_from_args(props, &n_params);

	MidgardObject **objects = midgard_object_find_parameters(mobject, n_params, params);

	_FREE_PARAMETERS;

	if(!objects)
		return PyTuple_New(i);

	while(objects[i] != NULL)
		i++;
	
	PyObject *list = PyTuple_New(i);
	
	OBJECTS2LIST(objects, list);
	
	g_free(objects);
	
	return list;
}

