import midgard

mrp = midgard.reflection_property("midgard_article")

sgtype = mrp.get_midgard_type("sitegroup")

if mrp.is_link("name") == True:
	raise SystemError("Property 'name' is not a link")

if mrp.is_linked("topic") == False:
	raise SystemError("Property 'topic' is linked") 

lname = mrp.get_link_name("up")

tname = mrp.get_link_target("up")

desc = mrp.description("sitegroup")

if mrp.is_multilang("title") == False:
	raise SystemError("'title' property is multilang")
