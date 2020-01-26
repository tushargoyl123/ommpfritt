#pragma once

#include "properties/numericproperty.h"
#include <Qt>

namespace omm
{

class FloatProperty : public Property::Registrar<FloatProperty, NumericProperty<double>>
{
public:
  FloatProperty(const FloatProperty& other);
  explicit FloatProperty(double default_value = 0);
  QString type() const override { return TYPE; }
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;
  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("Property", "FloatProperty");
  std::unique_ptr<Property> clone() const override;
  static StaticPropertyInfo static_info();
};

}  // namespace omm
