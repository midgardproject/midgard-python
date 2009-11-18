# midgard2 module initialization 
try:
	import gobject
except:
	pass

try:
	import _midgard 
except:
	pass

try:
	import datetime
except:
	pass

try:
	from _midgard import mgdschema
	from _midgard import db
except:
	pass
