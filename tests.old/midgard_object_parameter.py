import midgard
import midgard_config

host = midgard.midgard_host()
host.set_property("name", "foo.midgard-project.org")
host.set_property("port", 80)

host.create()

host.get_parameter("midcom", "root_topic")
host.set_parameter("midcom", "root_topic", '1')
val = host.get_parameter("midcom", "root_topic")

print(val)

print("done: midgard.midgard_object.parameter")
