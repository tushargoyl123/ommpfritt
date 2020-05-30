#pragma once

#include <QMouseEvent>
#include "geometry/vec2.h"
#include "tools/handles/handle.h"
#include "renderers/painter.h"
#include "geometry/util.h"
#include "tools/tool.h"
#include "preferences/uicolors.h"

namespace omm
{

template<typename ToolT, AxisHandleDirection direction>
class ScaleAxisHandle : public Handle
{
public:
  ScaleAxisHandle(ToolT& tool)
    : Handle(tool)
    , m_direction(-Handle::axis_directions.at(direction))
  {
  }

  bool contains_global(const Vec2f& point) const override
  {
    const Vec2f global_point = this->tool.transformation().inverted().apply_to_position(point);
    Vec2f v = project_onto_axis(global_point);

    // clamp v between o and m_direction
    v = v.clamped(Vec2f::min(Vec2f::o(), m_direction), Vec2f::max(Vec2f::o(), m_direction));

    return (global_point - v).euclidean_norm() < this->interact_epsilon();
  }

  bool mouse_move(const Vec2f& delta, const Vec2f& pos, const QMouseEvent& e) override
  {
    Handle::mouse_move(delta, pos, e);
    if (this->status() == HandleStatus::Active) {
      const auto inv_tool_transformation = this->tool.transformation().inverted();
      const Vec2f s = [this, inv_tool_transformation, pos, &e] {
        if (!!(e.modifiers() & Qt::ControlModifier)) {
          auto total_delta = inv_tool_transformation.apply_to_direction(pos - this->press_pos());
          total_delta = -this->discretize(this->project_onto_axis(total_delta), true, 10.0);
          return Vec2f {
            std::pow(2.0, total_delta.x/120.0),
            std::pow(2.0, total_delta.y/120.0)
          };
        } else {
          const auto global_pos = inv_tool_transformation.apply_to_position(pos);
          const auto origin = inv_tool_transformation.apply_to_position(this->press_pos());
          auto s = project_onto_axis(global_pos - origin) / origin;
          s = this->discretize(s, true, 0.2) + Vec2f{1, 1};
          for (auto i : { 0u, 1u }) {
            if (constexpr auto eps = 10e-10; std::abs(s[i]) < eps) {
              s[i] = std::copysign(eps, s[i]);
            }
          }
          return s;
        }
      }();
      auto transformation = omm::ObjectTransformation().scaled(s);
      transformation = transformation.transformed(inv_tool_transformation);
      static_cast<ToolT&>(this->tool).transform_objects(transformation);
      const auto tool_info = QString("%1, %2").arg(s.x).arg(s.y);
      static_cast<ToolT&>(this->tool).tool_info = tool_info;
      return true;
    } else {
      return false;
    }
  }

  void draw(QPainter& painter) const override
  {
    painter.setTransform(this->tool.transformation().to_qtransform(), true);

    painter.save();
    const QString name = Handle::axis_names.at(direction);
    QPen pen;
    pen.setColor(this->ui_color(name + "-outline"));
    pen.setWidthF(2.0);
    painter.setPen(pen);
    painter.drawLine(QPointF{0, 0}, to_qpoint(m_direction));
    const auto size = Vec2{1.0, 1.0} * 0.1 * m_direction.euclidean_norm();
    const QRectF rect(to_qpoint(m_direction - size/2.0), QSizeF(size.x, size.y));
    painter.fillRect(rect, this->ui_color(name + "-fill"));
    painter.drawRect(rect);
    painter.restore();
  }

private:
  const Vec2f m_direction;

  Vec2f project_onto_axis(const Vec2f& vec) const
  {
    const Vec2f s = m_direction;

    // project v onto the line through o and s
    return Vec2f::dot(vec, s) / Vec2f::dot(s, s) * s;
  }
};

}  // namespace omm