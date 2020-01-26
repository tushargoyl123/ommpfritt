#include "properties/vectorproperty.h"

namespace
{
  static constexpr auto inf = std::numeric_limits<double>::infinity();
  static constexpr auto int_high = std::numeric_limits<int>::max();
  static constexpr auto int_low = std::numeric_limits<int>::lowest();
}

namespace omm
{

StaticPropertyInfo FloatVectorProperty::static_info()
{
  return {
    [](const Property&, std::size_t channel) {
      assert(channel < 2);
      return std::vector {
        QObject::tr("x"), QObject::tr("y")
      }[channel];
    }
  };
}

StaticPropertyInfo IntegerVectorProperty::static_info()
{
  return {
    [](const Property&, std::size_t channel) {
      assert(channel < 2);
      return std::vector {
        QObject::tr("x"), QObject::tr("y")
      }[channel];
    }
  };
}

const Vec2f FloatVectorPropertyLimits::lower(-inf, -inf);
const Vec2f FloatVectorPropertyLimits::upper( inf,  inf);
const Vec2f FloatVectorPropertyLimits::step(1.0, 1.0);

FloatVectorProperty::FloatVectorProperty(const Vec2f& default_value)
  : Property::Registrar<FloatVectorProperty, NumericProperty<Vec2f>>(default_value)
{
}

FloatVectorProperty::FloatVectorProperty(const FloatVectorProperty& other)
  : Property::Registrar<FloatVectorProperty, NumericProperty<Vec2f>>(other)
{
}

void FloatVectorProperty::deserialize(AbstractDeserializer& deserializer, const Pointer& root)
{
  NumericProperty::deserialize(deserializer, root);
  set(deserializer.get_vec2f(make_pointer(root, TypedPropertyDetail::VALUE_POINTER)));
  if (is_user_property()) {
    set_default_value(
      deserializer.get_vec2f(make_pointer(root, TypedPropertyDetail::DEFAULT_VALUE_POINTER)));
  }
}

void FloatVectorProperty::serialize(AbstractSerializer& serializer, const Pointer& root) const
{
  NumericProperty::serialize(serializer, root);
  serializer.set_value( value(), make_pointer(root, TypedPropertyDetail::VALUE_POINTER));
  if (is_user_property()) {
    serializer.set_value( default_value(),
                          make_pointer(root, TypedPropertyDetail::DEFAULT_VALUE_POINTER) );
  }
}

std::unique_ptr<Property> FloatVectorProperty::clone() const
{
  return std::make_unique<FloatVectorProperty>(*this);
}

const Vec2i IntegerVectorPropertyLimits::lower(int_low,  int_low);
const Vec2i IntegerVectorPropertyLimits::upper(int_high, int_high);
const Vec2i IntegerVectorPropertyLimits::step(1, 1);

IntegerVectorProperty::IntegerVectorProperty(const Vec2i& default_value)
  : Property::Registrar<IntegerVectorProperty, NumericProperty<Vec2i>>(default_value)
{
}

IntegerVectorProperty::IntegerVectorProperty(const IntegerVectorProperty& other)
  : Property::Registrar<IntegerVectorProperty, NumericProperty<Vec2i>>(other)
{
}

QString IntegerVectorProperty::type() const { return TYPE; }

void IntegerVectorProperty::deserialize(AbstractDeserializer& deserializer, const Pointer& root)
{
  NumericProperty::deserialize(deserializer, root);
  set(deserializer.get_vec2i(make_pointer(root, TypedPropertyDetail::VALUE_POINTER)));
  if (is_user_property()) {
    set_default_value(
      deserializer.get_vec2i(make_pointer(root, TypedPropertyDetail::DEFAULT_VALUE_POINTER)));
  }
}

void IntegerVectorProperty::serialize(AbstractSerializer& serializer, const Pointer& root) const
{
  NumericProperty::serialize(serializer, root);
  serializer.set_value( value(), make_pointer(root, TypedPropertyDetail::VALUE_POINTER));
  if (is_user_property()) {
    serializer.set_value( default_value(),
                          make_pointer(root, TypedPropertyDetail::DEFAULT_VALUE_POINTER) );
  }
}

std::unique_ptr<Property> IntegerVectorProperty::clone() const
{
  return std::make_unique<IntegerVectorProperty>(*this);
}

}  // namespace omm
