#include <QString>
#include <bitset>
#include <QApplication>
#include <set>

namespace omm
{

template<typename E> QString enum_name(E e, bool translate);

template<typename E> const char* enum_name_impl(E e);

template<typename E> QString enum_name(E e, bool translate)
{
  using U = std::underlying_type_t<E>;
  const auto u = static_cast<U>(e);
  const bool one_bit_set = u && !(u & (u-1));
  if (one_bit_set) {
    if (translate) {
      return QApplication::translate("Enum", enum_name_impl(e));
    } else {
      return enum_name_impl(e);
    }
  } else {
    static constexpr std::size_t n = std::numeric_limits<U>::digits;
    QStringList items;
    for (std::size_t i = 0; i < n; ++i) {
      const std::size_t r = 1 << i;
      if (u & r) {
        items.push_back(enum_name(static_cast<E>(r), translate));
      }
    }
    return items.join(" | ");
  }
}

template<typename E> std::set<E> enumerate_enum();

}  // namespace om
