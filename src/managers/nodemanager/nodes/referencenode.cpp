#include "managers/nodemanager/nodes/referencenode.h"
#include "keybindings/menu.h"
#include "properties/referenceproperty.h"
#include "variant.h"
#include "scene/scene.h"
#include "properties/optionsproperty.h"

namespace
{

QString to_string(omm::PortType port_type)
{
  switch (port_type) {
  case omm::PortType::Input:
    return QT_TRANSLATE_NOOP("ReferenceNode", "input");
  case omm::PortType::Output:
    return QT_TRANSLATE_NOOP("ReferenceNode", "output");
  default:
    Q_UNREACHABLE();
    return "";
  }
}

}  // namespace

namespace omm
{

ReferenceNode::ReferenceNode(Scene* scene)
  : Node(scene)
{
  const QString category = tr("Node");
  create_property<ReferenceProperty>(REFERENCE_PROPERTY_KEY)
    .set_label(tr("Reference")).set_category(category);
}

std::unique_ptr<Menu> ReferenceNode::make_menu()
{
  auto menu = std::make_unique<Menu>(tr("Forwarded Ports"));
  AbstractPropertyOwner* apo = reference();
  if (apo == nullptr) {
    menu->addAction(tr("No properties."))->setEnabled(false);
  } else {
    for (auto key : apo->properties().keys()) {
      Property* property = apo->property(key);
      auto property_menu = std::make_unique<Menu>(property->label());
      property_menu->addAction(make_property_action(PortType::Input, key).release());
      property_menu->addAction(make_property_action(PortType::Output, key).release());
      menu->addMenu(property_menu.release());
    }
  }
  return menu;
}

void
ReferenceNode::deserialize(AbstractDeserializer& deserializer, const Serializable::Pointer& root)
{
  Node::deserialize(deserializer, root);
  for (auto type : { PortType::Input, PortType::Output }) {
    auto pointer = make_pointer(root, type == PortType::Input ? "input" : "output");
    std::set<QString> keys;
    deserializer.get(keys, pointer);
    for (const QString& key : keys) {
      add_forwarding_port(type, key);
    }
  }
}

void
ReferenceNode::serialize(AbstractSerializer& serializer, const Serializable::Pointer& root) const
{
  Node::serialize(serializer, root);
  for (auto [type, map] : m_forwarded_ports) {
    auto pointer = make_pointer(root, type == PortType::Input ? "input" : "output");
    serializer.set_value(::get_keys(map), pointer);
  }
}

void ReferenceNode::on_property_value_changed(Property* property)
{
}

AbstractPropertyOwner* ReferenceNode::reference() const
{
  return property(REFERENCE_PROPERTY_KEY)->value<AbstractPropertyOwner*>();
}

std::unique_ptr<QAction>
ReferenceNode::make_property_action(PortType port_type, const QString& key)
{
  const QString label = QCoreApplication::translate("ReferenceNode",
                                                    to_string(port_type).toStdString().c_str());
  auto action = std::make_unique<QAction>(label);
  action->setCheckable(true);
  auto map = m_forwarded_ports[port_type];
  action->setChecked(map.find(key) != map.end());
  connect(action.get(), &QAction::triggered, [this, port_type, key](bool checked) {
    if (checked) {
      add_forwarding_port(port_type, key);
    } else {
      remove_port(*m_forwarded_ports[port_type][key]);
    }
  });
  return action;
}

AbstractPort& ReferenceNode::add_forwarding_port(PortType port_type, const QString& key)
{
  const auto get_property = [this, key]() -> Property* {
    AbstractPropertyOwner* reference = this->reference();
    if (reference == nullptr) {
      return nullptr;
    } else if (reference->has_property(key)) {
      return reference->property(key);
    } else {
      return nullptr;
    }
  };
  auto* port = [port_type, get_property, this]() -> AbstractPort* {
    switch (port_type) {
    case PortType::Input:
      return &add_port<PropertyPort<PortType::Input>>(get_property);
    case PortType::Output:
      return &add_port<PropertyPort<PortType::Output>>(get_property);
    default:
      Q_UNREACHABLE();
      return nullptr;
    }
  }();
  m_forwarded_ports[port_type][key] = port;
  return *port;
}

}  // namespace