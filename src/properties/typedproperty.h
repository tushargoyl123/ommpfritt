#pragma once

#include "properties/property.h"
#include "serializers/abstractserializer.h"

namespace omm
{

namespace TypedPropertyDetail
{

  static constexpr auto VALUE_POINTER = "value";
  static constexpr auto DEFAULT_VALUE_POINTER = "default_value";

} // namespace TypedPropertyDetail

template<typename ValueT>
class TypedProperty : public Property, public ObserverRegister<AbstractTypedPropertyObserver>
{
public:
  using value_type = ValueT;

  TypedProperty(ValueT defaultValue = ValueT())
    : m_value(defaultValue), m_default_value(defaultValue) {}

public:
  virtual ValueT value() const { return m_value; }
  virtual void set_value(const ValueT& value) { m_value = value; }

  virtual ValueT default_value() const { return m_default_value; }
  virtual void set_default_value(const ValueT& value) { m_default_value = value; }

  virtual void reset() {  m_value = m_default_value; }

  py::object get_py_object() const override { return py::object(); }
  void set_py_object(const py::object& value) override {}

  std::string type() const override = 0;

  void serialize(AbstractSerializer& serializer, const Pointer& root) const override
  {
    Property::serialize(serializer, root);
    serializer.set_value(m_value, make_pointer(root, TypedPropertyDetail::VALUE_POINTER));
    serializer.set_value( m_default_value,
                          make_pointer(root, TypedPropertyDetail::DEFAULT_VALUE_POINTER) );
  }

private:
  ValueT m_value;
  ValueT m_default_value;
};

template<typename ValueT> std::string TypedProperty<ValueT>::type() const
{
  // TypedProperty<ValueT> has no meaningful way to provide type(), which is realised
  // by overriding the virtual type() member in any of the derived types.
  LOG(FATAL) << "It is tempting to call me. However, I'm useless. Don't call me.";
  return "";
}

}  // namespace
