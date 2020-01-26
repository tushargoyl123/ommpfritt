#pragma once

#include "properties/numericproperty.h"
#include <Qt>

namespace omm
{

struct FloatVectorPropertyLimits
{
  static const Vec2f lower;
  static const Vec2f upper;
  static const Vec2f step;
};

class FloatVectorProperty
  : public Property::Registrar<FloatVectorProperty, NumericProperty<Vec2f>>
{
public:
  explicit FloatVectorProperty(const Vec2f& default_value = Vec2f());
  FloatVectorProperty(const FloatVectorProperty& other);
  QString type() const override { return TYPE; }
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;
  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("Property", "FloatVectorProperty");
  std::unique_ptr<Property> clone() const override;
  static StaticPropertyInfo static_info();
};

struct IntegerVectorPropertyLimits
{
  static const Vec2i lower;
  static const Vec2i upper;
  static const Vec2i step;
};

class IntegerVectorProperty
    : public Property::Registrar<IntegerVectorProperty, NumericProperty<Vec2i>>
{
public:
  explicit IntegerVectorProperty(const Vec2i& default_value = Vec2i());
  IntegerVectorProperty(const IntegerVectorProperty& other);
  QString type() const override;
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;
  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("IntegerVectorProperty", "IntegerVectorProperty");
  std::unique_ptr<Property> clone() const override;
  static StaticPropertyInfo static_info();
};

}  // namespace omm
