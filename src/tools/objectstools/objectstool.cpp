#include "tools/objectstools/objectstool.h"

#include <memory>
#include "renderers/abstractrenderer.h"
#include "objects/object.h"
#include "properties/optionsproperty.h"
#include "scene/scene.h"
#include "common.h"
#include "tools/positionvariant.h"

namespace omm
{

template<typename PositionVariant> ObjectsTool<PositionVariant>
::ObjectsTool(Scene& scene, std::vector<std::unique_ptr<Handle>> handles)
  : Tool(scene)
  , m_position_variant(scene)
{
  this->handles = std::move(handles);
  add_property<OptionsProperty>(ALIGNMENT_PROPERTY_KEY)
    .set_options({ "global", "local" })
    .set_label(QObject::tr("Alignment").toStdString())
    .set_category(QObject::tr("Tool").toStdString());
}

template<typename PositionVariant>
ObjectTransformation ObjectsTool<PositionVariant>::transformation() const
{
  ObjectTransformation transformation;
  transformation.translate(m_position_variant.selection_center());
  if (property(ALIGNMENT_PROPERTY_KEY).template value<size_t>() == 1) {
    transformation.rotate(m_position_variant.selection_rotation());
  }
  return transformation;
}

template<typename PositionVariant>
void ObjectsTool<PositionVariant>::draw(AbstractRenderer& renderer) const
{
  if (m_position_variant.is_empty()) {
    return;
  }

  renderer.push_transformation(this->transformation());
  Tool::draw(renderer);
  renderer.pop_transformation();
}

template<typename PositionVariant>
void ObjectsTool<PositionVariant>::transform_objects(const ObjectTransformation& transformation)
{
  const ObjectTransformation::Mat h = this->transformation().to_mat();
  const ObjectTransformation::Mat h_inv = h.i();
  const ObjectTransformation::Mat t = transformation.to_mat();
  m_position_variant.transform(ObjectTransformation(h * t * h_inv));
}

template<typename PositionVariant>
bool ObjectsTool<PositionVariant>::mouse_move(const arma::vec2& delta, const arma::vec2& pos)
{
  if (m_position_variant.is_empty()) {
    return false;
  } else {
    const auto t_inv = transformation().inverted();
    const auto local_pos = t_inv.apply_to_position(pos);
    const auto local_delta = t_inv.apply_to_direction(delta);
    return Tool::mouse_move(local_delta, local_pos);
  }
}

template<typename PositionVariant>
bool ObjectsTool<PositionVariant>::mouse_press(const arma::vec2& pos)
{
  if (!m_position_variant.is_empty()) {
    const auto t_inv = transformation().inverted();
    const auto local_pos = t_inv.apply_to_position(pos);
    return Tool::mouse_press(local_pos);
  } else {
    return false;
  }
}

template<typename PositionVariant>
void ObjectsTool<PositionVariant>::mouse_release()
{
  if (!m_position_variant.is_empty()) {
    for (auto&& handle : handles) {
      handle->mouse_release();
    }
  }
}

template class ObjectsTool<ObjectPositions>;
template class ObjectsTool<PointPositions>;

}  // namespace omm
