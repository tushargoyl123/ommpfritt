#pragma once

#include "properties/numericproperty.h"
#include <Qt>

namespace omm
{

class IntegerProperty : public Property::Registrar<IntegerProperty, NumericProperty<int>>
{
public:
  explicit IntegerProperty(int default_value = 0);
  IntegerProperty(const IntegerProperty& other);
  QString type() const override { return TYPE; }
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;
  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("Property", "IntegerProperty");
  std::unique_ptr<Property> clone() const override;
  static StaticPropertyInfo static_info();
};

}  // namespace omm
