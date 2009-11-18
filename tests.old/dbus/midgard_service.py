import gobject
import dbus
import dbus.service
import dbus.mainloop.glib
import _midgard as midgard
import local_connection
		
def Exit(self):
        mainloop.quit()

print(dir(midgard.mgdschema))

def mbus_callback(object, arg):
	print "Hi! I am midgard_dbus from midgard-python. I got message:" 
	print object.get_message()

if __name__ == '__main__':
	
	#dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

    	#s_bus = dbus.SystemBus()
    	#name = dbus.service.BusName("org.midgardproject", s_bus)
	#name = dbus.service.BusName("org.midgardproject", s_bus)
	#mgdbus.set_dbus(s_bus)

	cnc = midgard.connection()
	cnc.open("midgard")

	#print dir(midgard)

	#midgard._connection.set_loglevel("debug");

	#print dir(midgard)
	mbus = midgard.dbus("/mgdschema/midgard_article/create")
	mbus_a = midgard.dbus("/midgard_replicator/exporter")
	#mbus_b = midgard.dbus("/midgard_replicator/importer");

	mbus.connect("notified", mbus_callback, "foo")

	#article_service = mgdbus.midgard_dbus("/midgard_article")
	#article_service = mgdbus.midgard_dbus.factory("/midgard_article")
	#topic_service = mgdbus.midgard_dbus.factory("/midgard_topic")
	#replication_service = mgdbus.midgard_dbus.factory("/replicator_export")

	mainloop = gobject.MainLoop()
   	print "Wating for data..."
    	mainloop.run()
