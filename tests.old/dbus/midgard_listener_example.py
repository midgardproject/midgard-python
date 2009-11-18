import gobject
import dbus.mainloop.glib
import _midgard as midgard

def mbus_callback(object, arg):
	print "Hi! I am midgard_dbus from midgard-python. I got message:"
	print object.get_message()

cnc = midgard.connection()
cnc.open("midgard")

mbus = midgard.dbus("/midgard_article")
mbus.connect("notified", mbus_callback, "foo")

mainloop = gobject.MainLoop()
mainloop.run()
