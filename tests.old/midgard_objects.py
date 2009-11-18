import _midgard as midgard
import midgard_config
import logging
import sys
import random 

el = midgard.mgdschema.midgard_element(1);

topic = midgard.mgdschema.midgard_topic()
rn = random.randint(1, 100000)
topic.name = "new topic created with midgard-python %d" % (rn, )
created = topic.create();

if created == False:
	raise SystemError("Could not create default topic object")

print "\nmidgard_topic (" + topic.name + ") created";

id = topic.get_property('id')
guid = topic.get_property('guid')
newtopic = midgard.mgdschema.midgard_topic(id)

article = midgard.mgdschema.midgard_article()
article.topic = id
article.name = "new article created with midgard-python %d" % (rn)

article_created = article.create()

if article_created == False:
	raise SystemError("Could not create default article object")

print "midgard_article (" + article.name + ") created \n"

article_id = article.id

newtopic = midgard.mgdschema.midgard_topic()
got_by_guid = newtopic.get_by_guid(guid)

if got_by_guid == False:
	raise SystemError("Failed to fetch newly created topic identified by guid (%s)", guid)

newtopic.delete()

# probably this is wrong 
topic.delete()

new_article = midgard.mgdschema.midgard_article(article_id)

id = new_article.get_property("id")
guid = new_article.get_property("guid")

new_article.purge()

#topic.emit("action_loaded")

