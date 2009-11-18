from twisted.words.protocols.jabber import client, jid
from twisted.words.xish import domish
from twisted.internet import glib2reactor
glib2reactor.install()

import dbus.mainloop.glib
import gobject
import _midgard as midgard
import feedparser
import time

class Testing:
    def __init__(self, replicationpackage):
        self.replicationpackage = replicationpackage
        self.runloop = True

    def connection(self, xmlstream):
        presence = domish.Element(('jabber:client', 'presence'))
        presence.addElement('status').addContent('Online')
        self.xmlstream = xmlstream
        self.xmlstream.send(presence)
        print "Connected"
        self.xmlstream.addObserver('/message', self.gotMessage)
        msg = domish.Element(("jabber:client", "message"))
        msg["to"] = "destination.jabber@someserver.org"
        body = msg.addElement("body", content = self.replicationpackage)
        self.xmlstream.send(msg)
        self.runloop = False
        
    def gotMessage(self, message):
        for e in message.elements():
            if e.name == "body":
                # Print it
                print u"from: %s" % message["from"]
                print unicode(e.__str__())
                
                # And reply back
                msg = domish.Element(("jabber:client", "message"))
                msg["to"] = message["from"]
                body = msg.addElement("body", content = "Hey, you said: " + e.__str__())
                self.xmlstream.send(msg)
      
                break
                
def rss_fetch(object, arg):
    feedserialized = object.get_message()
    feedlist = midgard.replicator.unserialize(feedserialized)

    print "Connecting"
    conn = Testing(feedserialized)
    me = jid.JID("tepheikk@zindium.org/Midgard")
    factory = client.basicClientFactory(me, "asdasd")
    factory.addBootstrap('//event/stream/authd', conn.connection)
    reactor.connectTCP("zindium.org", 5222, factory)
    reactor.startRunning()

    while conn.runloop:
        reactor.runUntilCurrent()
        reactor.doIteration(0)
        time.sleep(0.1)

    

cnc = midgard.connection()
cnc.open('midgard')
cnc.set_sitegroup('midgard')

listeners = []

for objectname in dir(midgard.mgdschema):
    mbus = midgard.dbus("/mgdschema/" + objectname + "/create")
    mbus.connect("notified", rss_fetch, "foo")
    listeners.append(mbus)




#mainloop = gobject.MainLoop()
reactor.run()
