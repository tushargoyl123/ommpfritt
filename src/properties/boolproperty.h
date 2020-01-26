#pragma once

#include "properties/typedproperty.h"
#include <Qt>

namespace omm
{

class BoolProperty : public Property::Registrar<BoolProperty, TypedProperty<bool>>
{
public:
  explicit BoolProperty(bool default_value = false);
  BoolProperty(const BoolProperty& other);
  QString type() const override { return TYPE; }
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;
  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("Property", "BoolProperty");
  std::unique_ptr<Property> clone() const override;
  static StaticPropertyInfo static_info();
};

}  // namespace omm
