#pragma once

#include <vector>
#include "properties/typedproperty.h"
#include <Qt>

namespace omm
{

class OptionsProperty : public TypedProperty<size_t>
{
public:
  using TypedProperty::TypedProperty;
  QString type() const override { return TYPE; }
  void deserialize(AbstractDeserializer& deserializer, const Pointer& root) override;
  void serialize(AbstractSerializer& serializer, const Pointer& root) const override;
  void set(const variant_type& variant) override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("Property", "OptionsProperty");
  std::unique_ptr<Property> clone() const override;

  std::vector<QString> options() const;
  OptionsProperty& set_options(const std::vector<QString>& options);

  static constexpr auto OPTIONS_POINTER = "options";
  bool is_compatible(const Property& other) const override;
  void revise() override;
  static const PropertyDetail detail;
};

}  // namespace omm
