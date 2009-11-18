import _midgard as midgard
import midgard_config

print(dir(midgard))

midgard._connection.set_loglevel("debug");
user = midgard.db.user.auth("admin", "password");
midgard._connection.set_loglevel("info");

person = midgard.mgdschema.midgard_person(1)
midgard._connection.set_loglevel("warn");

user_a = midgard._connection.get_user()
if midgard.user.is_root(user_a):
	midgard._connection.set_loglevel("debug");
	user.password('admin', 'password', midgard.USER_HASH_LEGACY_PLAIN);
midgard._connection.set_loglevel("warn");
error_string = midgard._connection.get_error_string()
person = user.get_person()
#print(person.get_property("firstname")+" "+person.get_property("lastname"))
#print(error_string)
