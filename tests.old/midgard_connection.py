import _midgard as midgard
import sys

class my_connection (midgard.connection):
	def __init__(self):
		midgard.connection.__init__(self);
				
	def connect(self):
		c = self.open('midgardm')
		return c
		
cnc = my_connection()

try:
	c = cnc.connect()
except SystemError, msg:
	print msg
	sys.exit(1)

if c == False:
	raise SystemError("Failed to connect to midgard database")

# check if singleton implementation is ok 
try:
	cn = my_connection()
	c = cn.connect()
	raise SystemError("midgard.connection sigleton failed")
except TypeError:
	pass

# get sitegroup
sgname = cnc.get_sitegroup()

# set sitegroup
sgname = ''
sgset = cnc.set_sitegroup(sgname)

if(sgset == False):
	raise SystemError("Failed to set sitegroup")

# set language 
dlang = 'pl'
cnc.set_lang(dlang)

# get language
lang = cnc.get_lang()
if lang != dlang:
	raise SystemError("Language returned does not match one being set")
