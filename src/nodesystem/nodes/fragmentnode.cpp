#include "nodesystem/nodes/fragmentnode.h"
#include "nodesystem/ordinaryport.h"
#include "properties/floatproperty.h"
#include "nodesystem/propertyport.h"
#include "variant.h"
#include "scene/scene.h"
#include "properties/optionsproperty.h"

namespace omm
{

const Node::Detail FragmentNode::detail {
  {
    {
      AbstractNodeCompiler::Language::GLSL,
      ""
    },
  },
  {
    // No Menu Path
  }
};

FragmentNode::FragmentNode(NodeModel& model)
  : Node(model)
{
  const QString category = tr("Node");
  m_input_port = &add_port<OrdinaryPort<PortType::Input>>(tr("color"));
}

bool FragmentNode::accepts_input_data_type(const QString& type, const InputPort& port) const
{
  assert(&port == m_input_port);
  return type == NodeCompilerTypes::COLOR_TYPE;
}

}  // namespace