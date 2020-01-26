#pragma once

#include "properties/typedproperty.h"
#include "color/color.h"
#include <Qt>

namespace omm
{

class ColorProperty : public Property::Registrar<ColorProperty, TypedProperty<Color>>
{
public:
  explicit ColorProperty(const Color& default_value = Color());
  ColorProperty(const ColorProperty& other);
  QString type() const override { return TYPE; }
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;
  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("Property", "ColorProperty");
  std::unique_ptr<Property> clone() const override;
  static StaticPropertyInfo static_info();
};

}  // namespace omm
