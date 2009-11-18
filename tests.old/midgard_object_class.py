import midgard
import midgard_config

pname = midgard.object_class.get_primary_property("midgard_article")

parent = midgard.object_class.get_property_parent("midgard_article")

up = midgard.object_class.get_property_up("midgard_article")

children = midgard.object_class.list_children("midgard_snippetdir")
#print(children)

guid = "63e54190969811dc95d991bf42eceab2eab2"
art = midgard.object_class.factory("midgard_article", guid)

article = midgard.object_class.get_object_by_guid(guid)
#article.update()

path = "new article created with midgard-python"
art = midgard.object_class.get_object_by_path("midgard_article", path)

if midgard.object_class.is_multilang("midgard_article") == False:
	raise SystemError("midgard_article class is multilang")
#print(article)
article.delete()

ud = midgard.object_class.undelete(guid)


print("done: midgard.object_class") 
