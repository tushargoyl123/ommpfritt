#include "properties/floatproperty.h"

namespace omm
{

StaticPropertyInfo FloatProperty::static_info()
{
  return {
    [](const Property&, std::size_t) { return ""; }
  };
}

FloatProperty::FloatProperty(const FloatProperty& other)
  : Property::Registrar<FloatProperty, NumericProperty<double>>(other)
{

}

FloatProperty::FloatProperty(double default_value)
  : Property::Registrar<FloatProperty, NumericProperty<double>>(default_value)
{

}

void FloatProperty::deserialize(AbstractDeserializer& deserializer, const Pointer& root)
{
  NumericProperty::deserialize(deserializer, root);
  set(deserializer.get_double(make_pointer(root, TypedPropertyDetail::VALUE_POINTER)));
  if (is_user_property()) {
    set_default_value(
      deserializer.get_double(make_pointer(root, TypedPropertyDetail::DEFAULT_VALUE_POINTER)));
  }
}

void FloatProperty::serialize(AbstractSerializer& serializer, const Pointer& root) const
{
  NumericProperty::serialize(serializer, root);
  serializer.set_value( value(), make_pointer(root, TypedPropertyDetail::VALUE_POINTER));
  if (is_user_property()) {
    serializer.set_value( default_value(),
                          make_pointer(root, TypedPropertyDetail::DEFAULT_VALUE_POINTER) );
  }
}

std::unique_ptr<Property> FloatProperty::clone() const
{
  return std::make_unique<FloatProperty>(*this);
}

}  // namespace omm
