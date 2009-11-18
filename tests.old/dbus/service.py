import gobject
import dbus
import dbus.service
import dbus.mainloop.glib
import _midgard as midgard
import local_connection

class midgard_listener(dbus.service.Object):

	@dbus.service.method("org.midgardproject.export_interface",
		in_signature='s', out_signature='b')
    
	def export(self, guid):
		
		object = midgard.object_class.get_object_by_guid(guid)
		imported = midgard.replicator.import_object(object)
		
		if imported != False:
			print "Imported object " + guid
			#xml = midgard.replicator.serialize(object)
			#print(xml)
			return True
		else:
			return False
		
def Exit(self):
        mainloop.quit()


if __name__ == '__main__':
	
	dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

    	session_bus = dbus.SessionBus()
    	name = dbus.service.BusName("org.midgardproject.midgard", session_bus)
    	listener = midgard_listener(session_bus, '/midgard_article')

	cnc = local_connection.quick_connection()
	cnc.connect("midgard")
	
	cnc.set_debuglevel("debug")

    	mainloop = gobject.MainLoop()
   	print "Wating for data..."
    	mainloop.run()
