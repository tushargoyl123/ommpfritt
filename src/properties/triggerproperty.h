#pragma once

#include "properties/typedproperty.h"
#include <Qt>

namespace omm
{

class TriggerProperty
    : public Property::Registrar<TriggerProperty, TypedProperty<TriggerPropertyDummyValueType>>
{
public:
  TriggerProperty(const TriggerProperty& other);
  explicit TriggerProperty(const TriggerPropertyDummyValueType& default_value = {});
  QString type() const override { return TYPE; }
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("Property", "TriggerProperty");
  std::unique_ptr<Property> clone() const override;
  void trigger();
  static StaticPropertyInfo static_info();
};

}  // namespace omm
