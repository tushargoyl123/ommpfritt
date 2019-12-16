#pragma once

#include <memory>
#include "tags/tag.h"
#include <Qt>

namespace omm
{

class NodeModel;

class NodesTag : public Tag
{
public:
  explicit NodesTag(Object& owner);
  NodesTag(const NodesTag& other);
  ~NodesTag();
  QString type() const override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("any-context", "NodesTag");
  static constexpr auto UPDATE_MODE_PROPERTY_KEY = "update";
  static constexpr auto TRIGGER_UPDATE_PROPERTY_KEY = "trigger";
  static constexpr auto EDIT_NODES_KEY = "edit-nodes";
  static constexpr auto NODES_POINTER = "nodes";

  std::unique_ptr<Tag> clone() const override;
  void on_property_value_changed(Property* property) override;
  void evaluate() override;
  void force_evaluate();
  Flag flags() const override;

  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;

private:
  std::unique_ptr<NodeModel> m_nodes;
};

}  // namespace omm
