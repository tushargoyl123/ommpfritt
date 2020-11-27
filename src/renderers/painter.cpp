#include "renderers/painter.h"
#include "geometry/util.h"
#include "renderers/style.h"
#include "scene/scene.h"
#include <QWidget>

namespace
{
QRectF get_roi(const omm::ObjectTransformation& viewport_transform,
               const QRectF bounding_box,
               const omm::Painter::Options& options)
{
  static const auto get_relative_roi = [](const QRectF& rect, const QRectF& absolute_roi) {
    const double x = rect.center().x();
    const double y = rect.center().y();
    const double left = (x - absolute_roi.left()) / (rect.left() - x);
    const double top = (y - absolute_roi.top()) / (rect.top() - y);
    const double right = (x + absolute_roi.right()) / (rect.right() - x);
    const double bottom = (y + absolute_roi.bottom()) / (rect.bottom() - y);
    return QRectF(QPointF(left, top), QPointF(right, bottom));
  };

  const QRectF vp_outline(0, 0, options.device.width(), options.device.height());
  const QPolygonF vp_outline_t = viewport_transform.to_qtransform().inverted().map(vp_outline);
  const QRectF absolute_roi = vp_outline_t.intersected(bounding_box).boundingRect();
  const QRectF relative_roi = get_relative_roi(bounding_box, absolute_roi);
  return relative_roi;
}

QTransform to_transformation(const omm::ObjectTransformation& transformation)
{
  const auto& m = transformation.to_mat();
  return QTransform(m.m[0][0],
                    m.m[1][0],
                    m.m[2][0],
                    m.m[0][1],
                    m.m[1][1],
                    m.m[2][1],
                    m.m[0][2],
                    m.m[1][2],
                    m.m[2][2]);
}

}  // namespace

namespace omm
{
omm::Painter::Painter(omm::Scene& scene, omm::Painter::Category filter)
    : scene(scene), category_filter(filter)
{
}

void Painter::render(Options options)
{
  options.default_style = &scene.default_style();
  scene.object_tree().root().draw_recursive(*this, options);
  assert(m_transformation_stack.empty());
}

void Painter::push_transformation(const ObjectTransformation& transformation)
{
  m_transformation_stack.push(current_transformation().apply(transformation));
  painter->setTransform(to_transformation(current_transformation()), false);
}

void Painter::pop_transformation()
{
  m_transformation_stack.pop();
  painter->setTransform(to_transformation(current_transformation()), false);
}

ObjectTransformation Painter::current_transformation() const
{
  if (m_transformation_stack.size() == 0) {
    return ObjectTransformation();
  } else {
    return m_transformation_stack.top();
  }
}

void Painter::toast(const Vec2f& pos, const QString& text) const
{
  static const QFont toast_font("Helvetica", 12, 0, false);
  static const QPen pen(Qt::black, 1.0);
  painter->save();
  painter->setFont(toast_font);
  painter->setPen(pen);
  const Vec2f gpos = current_transformation().apply_to_position(pos);
  painter->resetTransform();
  const QPointF top_left = to_qpoint(gpos);
  static constexpr double huge = 10.0e10;
  const QRectF rect(top_left, QSizeF(huge, huge));
  QRectF actual_rect;
  painter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, text, &actual_rect);
  const double margin = 10.0;
  actual_rect.adjust(-margin, -margin, margin, margin);

  painter->setBrush(QBrush(QColor(80, 60, 40, 120)));
  painter->setPen(QPen(Qt::white));
  painter->drawRoundedRect(actual_rect, 25, 25, Qt::AbsoluteSize);
  painter->restore();
}

QPainterPath Painter::path(const std::vector<Point>& points, bool closed)
{
  QPainterPath path;
  if (points.size() > 1) {
    path.moveTo(to_qpoint(points.front().position));

    for (size_t i = 1; i < points.size(); ++i) {
      path.cubicTo(to_qpoint(points.at(i - 1).right_position()),
                   to_qpoint(points.at(i).left_position()),
                   to_qpoint(points.at(i).position));
    }

    if (closed && points.size() > 2) {
      path.cubicTo(to_qpoint(points.back().right_position()),
                   to_qpoint(points.front().left_position()),
                   to_qpoint(points.front().position));
    }
  }
  return path;
}

QBrush
Painter::make_brush(const Style& style, const Object& object, const Painter::Options& options)
{
  if (style.property(omm::Style::BRUSH_IS_ACTIVE_KEY)->value<bool>()) {
    if (style.property("gl-brush")->value<bool>()) {
      const auto l_bb = object.bounding_box(ObjectTransformation());
      const auto v_bb = object.bounding_box(object.global_transformation(Space::Viewport));
      const double fx = std::abs(v_bb.width() / l_bb.width());
      const double fy = std::abs(v_bb.height() / l_bb.height());
      const double f = std::max(fx, fy);
      QSize size = (f * QSizeF(l_bb.width(), l_bb.height())).toSize();
      const QRectF roi = get_roi(object.global_transformation(Space::Viewport), l_bb, options);
      Texture texture = style.render_texture(object, size, roi, options);
      QPixmap pixmap = QPixmap::fromImage(texture.image);
      QBrush brush(pixmap);
      QTransform t;
      t.scale(1.0 / f, 1.0 / f);
      t.translate(-size.width() / 2.0 + texture.offset.x(),
                  -size.height() / 2.0 + texture.offset.y());
      brush.setTransform(t);
      return brush;
    } else {
      return make_simple_brush(style);
    }
  } else {
    return QBrush(Qt::NoBrush);
  }
}

QBrush Painter::make_simple_brush(const Style& style)
{
  if (style.property(omm::Style::BRUSH_IS_ACTIVE_KEY)->value<bool>()) {
    QBrush brush(Qt::SolidPattern);
    const auto color = style.property(omm::Style::BRUSH_COLOR_KEY)->value<omm::Color>();
    brush.setColor(color.to_qcolor());
    return brush;
  } else {
    return QBrush(Qt::NoBrush);
  }
}

QPen Painter::make_pen(const Style& style, const Object& object)
{
  Q_UNUSED(object);
  return make_simple_pen(style);
}

QPen Painter::make_simple_pen(const Style& style)
{
  if (style.property(omm::Style::PEN_IS_ACTIVE_KEY)->value<bool>()) {
    QPen pen;
    pen.setWidthF(style.property(omm::Style::PEN_WIDTH_KEY)->value<double>());
    pen.setColor(style.property(omm::Style::PEN_COLOR_KEY)->value<omm::Color>().to_qcolor());
    pen.setCosmetic(style.property(omm::Style::COSMETIC_KEY)->value<bool>());
    switch (style.property(omm::Style::CAP_STYLE_KEY)->value<std::size_t>()) {
    case 0:
      pen.setCapStyle(Qt::SquareCap);
      break;
    case 1:
      pen.setCapStyle(Qt::FlatCap);
      break;
    case 2:
      pen.setCapStyle(Qt::RoundCap);
      break;
    }
    switch (style.property(omm::Style::JOIN_STYLE_KEY)->value<std::size_t>()) {
    case 0:
      pen.setJoinStyle(Qt::BevelJoin);
      break;
    case 1:
      pen.setJoinStyle(Qt::MiterJoin);
      break;
    case 2:
      pen.setJoinStyle(Qt::RoundJoin);
      break;
    }
    const auto pen_style = style.property(omm::Style::STROKE_STYLE_KEY)->value<Qt::PenStyle>();
    pen.setStyle(static_cast<Qt::PenStyle>(pen_style + 1));
    return pen;
  } else {
    return QPen(Qt::NoPen);
  }
}

void Painter::set_style(const Style& style, const Object& object, const Painter::Options& options)
{
  painter->setPen(make_pen(style, object));
  painter->setBrush(make_brush(style, object, options));
}

Painter::Options::Options(const QWidget& viewport) : device_is_viewport(true), device(viewport)
{
}

Painter::Options::Options(const QPaintDevice& device) : device_is_viewport(false), device(device)
{
}

}  // namespace omm
