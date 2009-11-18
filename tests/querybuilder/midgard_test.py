#!/usr/bin/env python
# encoding: utf-8
"""
midgard.py

Created by Jimi Dini on 2009-10-23.
Copyright (c) 2009 Milk Farm Software, ltd.. All rights reserved.
"""

import unittest, os
import _midgard as midgard

class inherited_object(midgard.mgdschema.midgard_page):
    def __init__(self):
        midgard.mgdschema.midgard_page.__init__(self)
        

class qbtest(unittest.TestCase):

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

    def test_inherited_querybuilder(self):
        inherited = inherited_object()
        inherited.name = 'tests';
        inherited.title = 'tests';
        inherited.create()
                
        if (inherited.id > 0):
            assert True
        else:
            assert False
        
        qb = midgard.query_builder('inherited_object')
        qb.add_constraint('name', '=', 'tests')
        if len(qb.execute) == 1:
            assert True



#if __name__ == '__main__':
