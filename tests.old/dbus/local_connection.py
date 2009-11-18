import _midgard as midgard

class quick_connection(midgard.connection):
	
	def connect(self, name):
		
		config = midgard.config()
		opened = config.read_file('midgard', True);

		if opened == False:
			raise SystemError("Failed to open default configuration file")
		# try to connect
		self.open_config(config);
