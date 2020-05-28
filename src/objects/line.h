#pragma once
#include "objects/abstractpath.h"

namespace omm
{

class Line : public AbstractPath
{
public:
  explicit Line(Scene* scene);
  QString type() const override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("any-context", "Line");
  Flag flags() const override;

  static constexpr auto LENGTH_PROPERTY_KEY = "length";
  static constexpr auto ANGLE_PROPERTY_KEY = "angle";
  static constexpr auto CENTER_PROPERTY_KEY = "center";

protected:
  void on_property_value_changed(Property* property) override;

private:
  std::vector<Point> points() const override;
  bool is_closed() const override;
};

}  // namespace omm
