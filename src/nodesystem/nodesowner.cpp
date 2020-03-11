#include "nodesystem/nodesowner.h"
#include "nodesystem/nodemodel.h"
#include "properties/triggerproperty.h"
#include "managers/nodemanager/nodemanager.h"
#include "mainwindow/application.h"

namespace omm
{

NodesOwner::NodesOwner(AbstractNodeCompiler::Language language, Scene& scene)
  : m_node_model(std::make_unique<NodeModel>(language, scene))
{
}

NodesOwner::NodesOwner(const NodesOwner& other)
  : m_node_model(std::make_unique<NodeModel>(other.node_model()))
{
}

NodesOwner::~NodesOwner()
{
}

std::unique_ptr<Property> NodesOwner::make_edit_nodes_property() const
{
  auto property = std::make_unique<TriggerProperty>();
  QObject::connect(property.get(), &Property::value_changed, property.get(), [this]() {
    Manager& manager = Application::instance().get_active_manager(NodeManager::TYPE);
    static_cast<NodeManager&>(manager).set_model(&node_model());
  });
  return property;
}

NodeModel& NodesOwner::node_model() const
{
  return *m_node_model;
}

}  // namespace omm
