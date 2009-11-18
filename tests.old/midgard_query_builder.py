import midgard
import midgard_config

class my_article(midgard.midgard_article):
	def __init__(self):
		midgard.midgard_article.__init__(self)

# User must decide which classname he needs to pass as argument
classname = "midgard_article"
builder = midgard.query_builder(classname)
builder.add_constraint_with_property("metadata.created", "<", "metadata.revised")
builder.add_constraint("topic", "<>", 0)
builder.set_limit(1)
builder.set_offset(1)
builder.include_deleted()
list = builder.execute()


