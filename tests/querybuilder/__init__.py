import _midgard as midgard
import os

config_name = 'my_midgardtest_db'
configuration = midgard.config()
configuration.dbtype = 'SQLite'
configuration.database = 'my_midgardtest_db'
configuration.blobdir = os.path.join(os.path.expanduser("~"), "midgardblobs");
configuration.loglevel = 'message'

connection = midgard.connection()
connection.open_config(configuration)
midgard.storage.create_base_storage()
midgard.storage.create_class_storage('midgard_page')