import _midgard as midgard
import sys

class my_config(midgard.config):
	def __init__(self):
		midgard.config.__init__(self)

# new config instance
config = midgard.config();

# list configuration files
cfgs = config.list_files(True)

if len(cfgs) == 0:
	raise SysteError("No midgard configuration found")

# read default configuration
try:
	opened = config.read_file('midgard', True);
except SystemError, msg:
	print msg
	sys.exit(1)

print "Created configuration "

# try to connect
cnc = midgard.connection()
connected = cnc.open_config(config);

#if connected == False:
#	raise SystemError("Failed to connect using default configuration object")
