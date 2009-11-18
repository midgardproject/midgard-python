import gobject
import dbus
import dbus.service
import dbus.mainloop.glib

__dbus = None
__bus_initialized = False

def set_dbus(dbus):
	global __dbus
	__dbus = dbus
	set_bus_initialized()

def get_dbus():
	global __dbus
	return __dbus;

def initialize_dbus():
	dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
	s_bus = dbus.SystemBus()
	name = dbus.service.BusName("org.midgardproject", s_bus)
	set_dbus(s_bus)

def bus_is_initialized():
	global __bus_initialized
	return __bus_initialized

def set_bus_initialized():
	global __bus_initialized
	__bus_initialized = True

class midgard_dbus_signal(gobject.GObject):

	__gsignals__ = {   
		'notified': (gobject.SIGNAL_RUN_FIRST, gobject.TYPE_NONE,
		(gobject.TYPE_STRING,))         
	}

class midgard_bus(dbus.service.Object):
	
	@dbus.service.method("org.midgardproject.notify_sb_interface", in_signature='s', out_signature='b')

	def notify(self, data):		
		mdbs = midgard_dbus_signal()
		mdbs.emit('notified', data)
		print "midgard_bus::notify"+data
		return 

class midgard_dbus():

	bus_object = None

	def __init__(self, path):
		self.bus_object = midgard_bus(get_dbus(), path)

	def factory(path):	
		mbus = midgard_bus(get_dbus(), path)
		return mbus

	factory = staticmethod(factory)

	def send(path, data):
		object = midgard_dbus.get_object(path);
		object.notify(data, dbus_interface = "org.midgardproject.notify_sb_interface")
		print "midgard_dbus::notify"
		return data

	send = staticmethod(send)

	def get_object(path):
		bus = dbus.SystemBus()
		return bus.get_object("org.midgardproject", path)
	
	get_object = staticmethod(get_object)

class remote(midgard_dbus):

	def send(path, data):
		print "NOTIFIED"
		midgard_dbus.send(path, True)

	send = staticmethod(send)
