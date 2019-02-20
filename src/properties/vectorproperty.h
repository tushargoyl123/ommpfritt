#pragma once

#include "properties/numericproperty.h"
#include <Qt>

namespace omm
{

struct FloatVectorPropertyLimits
{
  static const arma::vec2 lower;
  static const arma::vec2 upper;
  static const arma::vec2 step;
};

class FloatVectorProperty
  : public NumericProperty<VectorPropertyValueType<arma::vec2>, FloatVectorPropertyLimits>
{
public:
  using NumericProperty::NumericProperty;
  std::string type() const override;
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;
  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
  static constexpr auto TYPE = QT_TR_NOOP("FloatVectorProperty");
  std::unique_ptr<Property> clone() const override;
};

struct IntegerVectorPropertyLimits
{
  static const arma::ivec2 lower;
  static const arma::ivec2 upper;
  static const arma::ivec2 step;
};

class IntegerVectorProperty
  : public NumericProperty<VectorPropertyValueType<arma::ivec2>, IntegerVectorPropertyLimits>
{
public:
  using NumericProperty::NumericProperty;
  std::string type() const override;
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;
  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
  static constexpr auto TYPE = QT_TR_NOOP("IntegerVectorProperty");
  std::unique_ptr<Property> clone() const override;
};

}  // namespace omm
