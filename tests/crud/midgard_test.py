#!/usr/bin/env python
# encoding: utf-8
"""
midgard.py

Created by Jimi Dini on 2009-10-23.
Copyright (c) 2009 Milk Farm Software, ltd.. All rights reserved.
"""

import unittest, os
import _midgard as midgard

class inherited_page(midgard.mgdschema.midgard_page):
    def __init__(self):
        midgard.mgdschema.midgard_page.__init__(self)

class crud(unittest.TestCase):

    def setUp(self):
        try:
            self.open_connection()
        except:
            pass
        pass

    def open_connection(self):
        config_name = 'my_midgardtest_db'
        configuration = midgard.config()
        configuration.dbtype = 'SQLite'
        configuration.database = 'my_midgardtest_db'
        configuration.blobdir = os.path.join(os.path.expanduser("~"), "midgardblobs");
        configuration.loglevel = 'message'
        connection = midgard.connection()
        
        connection.open_config(configuration)

        
    def test_create(self):
        host = midgard.mgdschema.midgard_page()
        host.name = 'test'
        try:
            assert host.create()
            if host.id > 0:
                assert True
        except:
            assert False

    def test_inherited_create(self):
        inherited_obj = inherited_page()
        inherited_obj.title = 'sometitle'
        inherited_obj.name = 'something else'
        assert inherited_obj.create()
        if inherited_obj.id > 0:
            assert True
        else:
            assert False

    def test_doublecreate(self):
        host = midgard.mgdschema.midgard_page()
        host.create()
        try:
            host.create()
            assert False
        except:
            assert True


#if __name__ == '__main__':
