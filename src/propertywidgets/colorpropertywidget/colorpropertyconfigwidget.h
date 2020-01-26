#pragma once

#include "propertywidgets/propertyconfigwidget.h"
#include "properties/colorproperty.h"

namespace omm
{

class ColorPropertyConfigWidget : public PropertyConfigWidget::Registrar<ColorPropertyConfigWidget>
{
public:
  using Registrar<ColorPropertyConfigWidget>::Registrar;
  static constexpr auto TYPE = "ColorPropertyConfigWidget";
  QString type() const override { return TYPE; }
  void init(const Property::Configuration&) override { }
  void update(Property::Configuration&) const override { }
};

}  // namespace omm
