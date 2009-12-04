/* 
 * Copyright (C) 2008 Piotr Pokora <piotrek.pokora@gmail.com>
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

#define ATTACHMENT_DEBUG(__name) \
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
pymidgard_object_delete_attachments(PyGObject *self, PyObject *args) 
{
	ATTACHMENT_DEBUG("delete_attachments");

	PyObject *props;
	
	if(!PyArg_ParseTuple(args, "O", &props))
		return NULL;

	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);

	guint n_params;
	GParameter *params = _py_midgard_parameters_from_args(props, &n_params);
	
	gboolean rv = midgard_object_delete_attachments(mobject, n_params, params);

	_FREE_PARAMETERS;

	if(rv)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

PyObject *
pymidgard_object_has_attachments(PyGObject *self, PyObject *args) 
{
	ATTACHMENT_DEBUG("has_attachments");

	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);
	
	gboolean rv = midgard_object_has_attachments(mobject);

	if(rv)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}


PyObject *
pymidgard_object_purge_attachments(PyGObject *self, PyObject *args) 
{
	ATTACHMENT_DEBUG("purge_attachments");

	PyObject *props;
	gboolean delete_blobs = TRUE;
	if(!PyArg_ParseTuple(args, "|bO", &delete_blobs, &props))
		return NULL;

	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);

	guint n_params;
	GParameter *params = _py_midgard_parameters_from_args(props, &n_params);
	
	gboolean rv = midgard_object_purge_attachments(mobject, delete_blobs, n_params, params);

	_FREE_PARAMETERS;

	if(rv)
		Py_RETURN_TRUE;

	Py_RETURN_FALSE;
}

PyObject *
pymidgard_object_list_attachments(PyGObject *self, PyObject *args) 
{
	ATTACHMENT_DEBUG("list_attachments");

	guint i = 0;
	
	if(!PyArg_ParseTuple(args, ""))
		return NULL;

	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);

	MidgardObject **objects = midgard_object_list_attachments(mobject);

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
pymidgard_object_find_attachments(PyGObject *self, PyObject *args) 
{
	ATTACHMENT_DEBUG("find_attachments");

	PyObject *props;

	if(!PyArg_ParseTuple(args, "O", &props))
		return NULL;
	
	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);

	guint n_params, i = 0;
	GParameter *params = _py_midgard_parameters_from_args(props, &n_params);

	MidgardObject **objects = midgard_object_find_attachments(mobject, n_params, params);

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

PyObject *
pymidgard_object_create_attachment(PyGObject *self, PyObject *args) 
{
	ATTACHMENT_DEBUG("create_attachment");

	gchar *name = NULL;
	gchar *title = NULL;
	gchar *mimetype = NULL;

	if(!PyArg_ParseTuple(args, "|s|s|s", &name, &title, &mimetype))
		return NULL;
	
	MidgardObject *mobject = MIDGARD_OBJECT(self->obj);
	MidgardObject *att = midgard_object_create_attachment(mobject, name, title, mimetype);

	if(att)
		return Py_BuildValue("O", pygobject_new(G_OBJECT(att)));

	Py_RETURN_NONE;
}
