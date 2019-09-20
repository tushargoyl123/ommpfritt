#pragma once

#include "aspects/abstractpropertyowner.h"
#include "aspects/animatablepropertyowner.h"

namespace omm
{

template<AbstractPropertyOwner::Kind kind_> class PropertyOwner : public AbstractPropertyOwner
{
public:
  using AbstractPropertyOwner::AbstractPropertyOwner;
  virtual ~PropertyOwner() = default;
  static constexpr AbstractPropertyOwner::Kind KIND = kind_;
  AbstractPropertyOwner::Kind kind() const override { return KIND; }
};

std::ostream& operator<<(std::ostream& ostream, const AbstractPropertyOwner* apo);

template<typename T, typename S> T kind_cast(S s)
{
  if (s != nullptr && s->kind() == std::remove_pointer_t<T>::KIND) {
    return static_cast<T>(s);
  } else {
    return nullptr;
  }
}

template<typename T, template<typename...> class ContainerT>
ContainerT<T*> kind_cast(const ContainerT<AbstractPropertyOwner*>& ss)
{
  return ::filter_if(::transform<T*>(ss, [](auto* s) { return kind_cast<T*>(s); }), ::is_not_null);
}

template<typename T, template<typename...> class ContainerT>
ContainerT<AbstractPropertyOwner*> down_cast(const ContainerT<T*>& ss)
{
  return ::transform<AbstractPropertyOwner*>(ss, ::identity);
}

}  // namespace omm

template<> struct omm::EnableBitMaskOperators<omm::AbstractPropertyOwner::Kind> : std::true_type {};
template<> struct omm::EnableBitMaskOperators<omm::AbstractPropertyOwner::Flag> : std::true_type {};
