import sys
import _midgard as midgard
import local_connection
import dbus

def main():
	
	bus = dbus.SessionBus()

	cnc = local_connection.quick_connection()
	cnc.connect("midgard")

	try:
		remote_object = bus.get_object("org.midgardproject.midgard",
			"/midgard_article")
	except dbus.DBusException:
		print ("No listener for midgard configuration")
		sys.exit(1)

	topic = midgard.mgdschema.midgard_topic(1)
	guid = topic.get_property("guid")

	reply = remote_object.export(guid,
		dbus_interface = "org.midgardproject.export_interface")
       
	if reply != False:
		print("Object identified by " + guid + " exported")
	
	sys.exit()
	
if __name__ == '__main__':
	main()
