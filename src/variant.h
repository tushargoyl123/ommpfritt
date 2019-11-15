#pragma once

#include "color/color.h"
#include <variant>
#include "geometry/vec2.h"
#include <QString>

namespace omm
{

class AbstractPropertyOwner;

class TriggerPropertyDummyValueType
{
public:
  bool operator==(const TriggerPropertyDummyValueType&) const { return true; }
  bool operator!=(const TriggerPropertyDummyValueType&) const { return false; }
};

using variant_type = std::variant< bool, double, Color, int, AbstractPropertyOwner*,
                                   QString, size_t, TriggerPropertyDummyValueType,
                                   Vec2f, Vec2i >;


//=== Channels
// Channels provide a unified way to read and write numerical values in a variant.

/**
 * @brief n_channels returns the number of channels in the type T.
 *  T == Color:            4 (hue, saturation, value, alpha)
 *  T == Vec2f, Vec2i:     2 (x, y)
 *  T == int, double bool: 1
 *  other:                 0
 */
template<typename T> std::size_t n_channels()
{
  if constexpr (std::is_same_v<T, double> || std::is_same_v<T, int> || std::is_same_v<T, bool>) {
    return 1;
  } else if constexpr (std::is_same_v<T, omm::Color>) {
    return 4;
  } else if constexpr (std::is_same_v<T, omm::Vec2f> || std::is_same_v<T, omm::Vec2i>) {
    return 2;
  } else {
    return 0;
  }
}

/**
 * @returns the numeric value of the given T at given channel
 * @note channel must not exceed the number of supported channels for a given type T.
 */
template<typename T> double get_channel_value(const T& v, std::size_t channel)
{
  if constexpr (std::is_same_v<T, double> || std::is_same_v<T, int>) {
    assert(channel == 0);
    return static_cast<double>(v);
  } else if constexpr (std::is_same_v<T, omm::Color>) {
    return v.components(v.model())[channel];
  } else if constexpr (std::is_same_v<T, omm::Vec2f> || std::is_same_v<T, omm::Vec2i>) {
    return static_cast<double>(v[channel]);
  } else if constexpr (std::is_same_v<T, bool>) {
    return v ? 1.0 : 0.0;
  } else {
    qFatal("tried get numeric value from non-numeric type.");
    Q_UNREACHABLE();
    return 0.0;
  }
}

/**
 * @brief sets the channel of the given value to value.
 * @note channel must not exceed the number of supported channels for a given type T.
 */
template<typename T> void set_channel_value(T& v, std::size_t channel, double value)
{
  if constexpr (std::is_same_v<T, double> || std::is_same_v<T, int>) {
    assert(channel == 0);
    v = value;
  } else if constexpr (std::is_same_v<T, omm::Color>) {
    const auto model = v.model();
    auto components = v.components(model);
    components[channel] = value;
    v.set_components(model, components);
  } else if constexpr (std::is_same_v<T, omm::Vec2f> || std::is_same_v<T, omm::Vec2i>) {
    v[channel] = value;
  } else if constexpr (std::is_same_v<T, bool>) {
    v = std::abs(value) > 0.5;
  } else {
    qFatal("tried set numeric value of non-numeric type.");
    Q_UNREACHABLE();
  }
}

/**
 * @brief n_channels returns the number of supported channels for the type hold by the given
 *  concrete variant.
 */
std::size_t n_channels(const variant_type& variant);

/**
 * @brief get_channel_value return the value of the given channel of the type stored in the given
 *  variant. @code channel must not exceed the number of supported channels for the currently stored
 *  variant type.
 */
double get_channel_value(const variant_type& variant, std::size_t channel);

/**
 * @brief set_channel_value set the value of the given channel of the type stored in the given
 *  variant. @code channel must not exceed the number of supported channels for the currently stored
 *  variant type.
 */
void set_channel_value(variant_type& variant, std::size_t channel, const double value);

std::ostream& operator<<(std::ostream& ostream, const TriggerPropertyDummyValueType& v);
std::ostream& operator<<(std::ostream& ostream, const variant_type& v);

}  // namespace omm
