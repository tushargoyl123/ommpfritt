#pragma once

#include "properties/typedproperty.h"

namespace omm
{

class StringProperty : public TypedProperty<std::string>
{
public:
  using TypedProperty::TypedProperty;
  std::string type() const override;
  std::string widget_type() const override;
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;
  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
};

}  // namespace omm
