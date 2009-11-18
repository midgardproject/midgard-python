import _midgard as midgard
import midgard_config

class my_blob(midgard.blob):
	def __init__(self):
		midgard.blob.__init__(self)

att = midgard.mgdschema.midgard_attachment()
blob = midgard.blob(att)

content = "This is my note"
blob.write_content(content)

content = blob.read_content()
print(content)

