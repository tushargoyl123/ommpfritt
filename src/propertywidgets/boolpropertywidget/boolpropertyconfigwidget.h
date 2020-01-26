#pragma once

#include "propertywidgets/propertyconfigwidget.h"
#include "properties/boolproperty.h"

namespace omm
{

class BoolPropertyConfigWidget : public PropertyConfigWidget::Registrar<BoolPropertyConfigWidget>
{
public:
  using Registrar<BoolPropertyConfigWidget>::Registrar;
  static constexpr auto TYPE = "BoolPropertyConfigWidget";
  QString type() const override { return TYPE; }
  void init(const Property::Configuration&) override { }
  void update(Property::Configuration&) const override { }
};

}  // namespace omm
