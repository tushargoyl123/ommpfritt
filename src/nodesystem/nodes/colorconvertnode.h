#pragma once

#include "nodesystem/node.h"

namespace omm
{
template<PortType> class OrdinaryPort;

class ColorConvertNode : public Node
{
  Q_OBJECT
public:
  explicit ColorConvertNode(NodeModel& model);
  static constexpr auto CONVERSION_PROPERTY_KEY = "conversion";
  static constexpr auto COLOR_PROPERTY_KEY = "color";
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("any-context", "ColorConvertNode");

  QString output_data_type(const OutputPort& port) const override;
  QString title() const override;
  QString type() const override
  {
    return TYPE;
  }
  static const Detail detail;

private:
  OutputPort* m_vector_output_port;
};

}  // namespace omm
