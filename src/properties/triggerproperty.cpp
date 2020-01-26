#include "properties/triggerproperty.h"

namespace omm
{

StaticPropertyInfo TriggerProperty::static_info()
{
  return {
    nullptr
  };
}

TriggerProperty::TriggerProperty(const TriggerProperty& other)
  : Property::Registrar<TriggerProperty, TypedProperty<TriggerPropertyDummyValueType>>(other)
{
}

TriggerProperty::TriggerProperty(const TriggerPropertyDummyValueType& default_value)
  : Property::Registrar<TriggerProperty, TypedProperty<TriggerPropertyDummyValueType>>(default_value)
{
}

std::unique_ptr<Property> TriggerProperty::clone() const
{
  return std::make_unique<TriggerProperty>(*this);
}

void TriggerProperty::trigger()
{
  // TODO execute pre and post submit hooks
  // TODO implement set-action for python
  Q_EMIT value_changed(this);
}

}  // namespace omm
