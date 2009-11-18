import midgard
import midgard_config

classname = "midgard_topic"

collector = midgard.collector(classname, "up", 0)
collector.set_key_property("name");
collector.set("new python key", "new python subkey", 3.14);
collector.add_constraint("name", "<>", "");
collector.execute()

print(dir(collector))

list = collector.list_keys()
print(list)

list = collector.get("new python key")
print(list)

value = collector.get_subkey("new python key", "new python subkey")
print(value)

mc = midgard.collector(classname, "up", 0)
mc.set_key_property("name")
mc.add_constraint_with_property("metadata.published", ">", "parameter.metadata.created")
mc.set_offset(1)
mc.set_limit(1)
mc.unset_languages()
mc.include_deleted()
mc.execute()

collector.merge(mc, True);
