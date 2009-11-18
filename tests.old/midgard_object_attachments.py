import _midgard as midgard
import midgard_config

topic = midgard.mgdschema.midgard_topic()

topic.get_by_id(1)

topic.create_attachment("dummy atachment", "attachment_title", "image/jpeg")

atts = topic.list_attachments()

topic.purge_attachments()
