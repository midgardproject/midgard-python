import sys
import _midgard as midgard
import local_connection
import dbus
import midgard_dbus as mgdbus

def main():

	cnc = local_connection.quick_connection()
	cnc.connect("midgard")
	cnc.set_loglevel("debug")

	midgard.dbus.send("/midgard_article", "Hi there!");

	topic = midgard.mgdschema.midgard_topic(1)
	guid = topic.get_property("guid")

	mgdbus.midgard_dbus.send("/midgard_article", guid)
	
	sys.exit()
	
if __name__ == '__main__':
	main()
