import ez_setup
ez_setup.use_setuptools()

from setuptools import setup, Extension, find_packages
import os

class PkgConfig(object):
    def __init__(self, names):
        def stripfirsttwo(string):
            return string[2:]
        self.libs = map(stripfirsttwo, os.popen("pkg-config --libs-only-l %s" % names).read().split())
        self.libdirs = map(stripfirsttwo, os.popen("pkg-config --libs-only-L %s" % names).read().split())
        self.incdirs = map(stripfirsttwo, os.popen("pkg-config --cflags-only-I %s" % names).read().split())
    


flags = PkgConfig("glib-2.0 pygobject-2.0 midgard2 gthread-2.0")

setup(
    
    name = 'python-midgard2',
    version = '10.05.2',
    
    author = "Midgard Project",
    author_email = "dev@lists.midgard-project.org",
    description = "Python bindings for Midgard2",
    license = "LGPL",
    url = "http://www.midgard-project.org/",
    
    packages = find_packages(),
    ext_modules = [
        Extension(
            '_midgard',
            [
                "midgardmodule.c",
                "py_midgard_blob.c",
                "py_midgard_collector.c",
                "py_midgard_config.c",
                "py_midgard_connection.c",
                "py_midgard_dbobject.c",
                "py_midgard_dbus.c",
                "py_midgard_key_config.c",
                "py_midgard_key_config_context.c",
                "py_midgard_key_config_file.c",
                "py_midgard_key_config_file_context.c",
                "py_midgard_object.c",
                "py_midgard_object_attachment.c", 
                "py_midgard_object_parameter.c",
                "py_midgard_query_builder.c",
            		"py_midgard_query_constraint.c",
                "py_midgard_query_executor.c",
                "py_midgard_query_holder.c",
                "py_midgard_query_storage.c",
                "py_midgard_reflector_property.c",
                "py_midgard_reflector_object.c",
                "py_midgard_replicator.c",
                "py_midgard_storage.c",
                "py_midgard_schema_object_tree.c",
                "py_midgard_schema_object_factory.c",
                "py_midgard_transaction.c",
                "py_midgard_user.c",
                "py_midgard_view.c",
            ],
            include_dirs = flags.incdirs + ['.'],
            libraries = flags.libs,
            library_dirs = flags.libdirs,
            runtime_library_dirs = flags.libdirs
        ),
    ],
)
