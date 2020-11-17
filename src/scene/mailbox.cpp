#include "scene/mailbox.h"
#include "objects/object.h"
#include "renderers/style.h"
#include "tags/tag.h"

namespace omm
{
MailBox::MailBox()
{
  connect(this, &MailBox::transformation_changed, this, qOverload<>(&MailBox::appearance_changed));
  connect(this,
          qOverload<Object&>(&MailBox::appearance_changed),
          this,
          qOverload<>(&MailBox::appearance_changed));
  connect(this,
          qOverload<Tool&>(&MailBox::appearance_changed),
          this,
          qOverload<>(&MailBox::appearance_changed));
  connect(this,
          qOverload<Style&>(&MailBox::appearance_changed),
          this,
          qOverload<>(&MailBox::appearance_changed));
  connect(this, &MailBox::scene_reseted, this, qOverload<>(&MailBox::appearance_changed));
  connect(this,
          qOverload<Tool&>(&MailBox::appearance_changed),
          this,
          qOverload<>(&MailBox::appearance_changed));
  connect(this, &MailBox::transformation_changed, [this](Object& o) {
    if (!o.is_root()) {
      Q_EMIT appearance_changed(o.tree_parent());
    }
  });

  connect(this, &MailBox::object_inserted, [this](Object& parent, Object& o) {
    Q_UNUSED(o);
    Q_EMIT appearance_changed(parent);
    Q_EMIT abstract_property_owner_inserted(o);
  });
  connect(this, &MailBox::object_removed, [this](Object& parent, Object& o) {
    Q_UNUSED(o);
    Q_EMIT appearance_changed(parent);
    Q_EMIT abstract_property_owner_removed(o);
  });
  connect(this, &MailBox::object_moved, [this](Object& old_parent, Object& new_parent, Object& o) {
    Q_UNUSED(o);
    Q_EMIT appearance_changed(old_parent);
    Q_EMIT appearance_changed(new_parent);
  });
  connect(this, &MailBox::tag_removed, [this](Object& owner, Tag& tag) {
    Q_UNUSED(tag);
    Q_EMIT appearance_changed(owner);
    Q_EMIT abstract_property_owner_removed(tag);
  });
  connect(this, &MailBox::tag_inserted, [this](Object& owner, Tag& tag) {
    Q_UNUSED(tag);
    Q_EMIT appearance_changed(owner);
    Q_EMIT abstract_property_owner_inserted(tag);
  });

  connect(this, &MailBox::style_removed, [this](Style& style) {
    Q_EMIT abstract_property_owner_removed(style);
  });
  connect(this, &MailBox::style_inserted, [this](Style& style) {
    Q_EMIT abstract_property_owner_inserted(style);
  });
}

}  // namespace omm
