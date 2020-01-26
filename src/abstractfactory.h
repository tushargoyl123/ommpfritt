#pragma once

#include <algorithm>
#include <set>
#include "aspects/typed.h"
#include <unordered_map>
#include <memory>
#include <utility>
#include <set>

namespace omm {

template<typename Base, typename StaticInfo, typename Key, typename... Args> class Factory
    : public virtual omm::Typed
{
private:
  static constexpr bool has_static_info = !std::is_same_v<StaticInfo, void>;
  using InfoMapType = std::unordered_map<Key, StaticInfo>;

public:
  template<typename... T> static std::unique_ptr<Base> make(const Key& name, T&&... args)
  {
    return creator_map().at(name)(std::forward<T>(args)...);
  }

  static const std::add_lvalue_reference_t<StaticInfo> static_info(const Key& key)
  {
    return info_map().at(key);
  }

  friend Base;
  template<typename T, typename InjectedBase = Base> struct Registrar : InjectedBase
  {
    friend T;
    static_assert(std::is_base_of_v<Base, InjectedBase>);
    static bool register_T()
    {
      const Key name = T::TYPE;
      Factory::creator_map()[name] = [](Args... args) -> std::unique_ptr<Base> {
        return std::make_unique<T>(std::forward<Args>(args)...);
      };
      if constexpr (has_static_info) {
        Factory::info_map().insert({ name, T::static_info() });
      }
      return true;
    }
    static bool registered;
    // The constructor must be instanciated explicitely. Inheriting constructor (using) does not
    // register the class and is best avoided.
    template<typename... S>Registrar(S&&... args)
      : InjectedBase(std::forward<S>(args)...)
    {
    }

    virtual ~Registrar()
    {
      (void) registered;
    }
  };

  static auto keys()
  {
    const auto& map = creator_map();
    std::set<Key> keys;
    std::transform(map.begin(), map.end(), std::inserter(keys, keys.end()), [](const auto& pair) {
      return pair.first;
    });
    return keys;
  }

  Factory() = default;
private:
  using creator_type = std::unique_ptr<Base>(*)(Args...);

  static auto& creator_map()
  {
    static std::unordered_map<Key, creator_type> s;
    return s;
  }

  static auto& info_map()
  {
    static InfoMapType s;
    return s;
  }
};

template<typename Base, typename StaticInfo, typename Key, typename... Args>
template<typename T, typename InjectedBase>
bool Factory<Base, StaticInfo, Key, Args...>::Registrar<T, InjectedBase>::registered
  = Factory<Base, StaticInfo, Key, Args...>::template Registrar<T, InjectedBase>::register_T();

}  // namespace omm
