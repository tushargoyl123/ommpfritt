#include "animation/animator.h"

#include "logging.h"
#include "serializers/abstractserializer.h"
#include "animation/fcurve.h"

#include "aspects/propertyowner.h"

namespace omm
{

Animator::Animator()
{
  m_timer.setInterval(1000.0/30.0);
  connect(&m_timer, SIGNAL(timeout()), this, SLOT(advance()));
}

void Animator::serialize(AbstractSerializer &serializer, const Serializable::Pointer &pointer) const
{
  Serializable::serialize(serializer, pointer);

  serializer.set_value(m_start_frame, make_pointer(pointer, START_FRAME_POINTER));
  serializer.set_value(m_end_frame, make_pointer(pointer, END_FRAME_POINTER));
  serializer.set_value(m_current_frame, make_pointer(pointer, CURRENT_FRAME_POINTER));

  const auto fcurves_pointer = make_pointer(pointer, FCURVES_POINTER);
  serializer.start_array(m_fcurves.size(), fcurves_pointer);
  int i = 0;
  for (const auto& fcurve : m_fcurves) {
    serializer.set_value(fcurve->type(), make_pointer(pointer, AbstractFCurve::TYPE_KEY));
    fcurve->serialize(serializer, make_pointer(fcurves_pointer, i));
    i += 1;
  }
  serializer.end_array();
}

void Animator::deserialize(AbstractDeserializer &deserializer, const Pointer &pointer)
{
  Serializable::deserialize(deserializer, pointer);

  set_start(deserializer.get_int(make_pointer(pointer, START_FRAME_POINTER)));
  set_end(deserializer.get_int(make_pointer(pointer, END_FRAME_POINTER)));
  set_current(deserializer.get_int(make_pointer(pointer, CURRENT_FRAME_POINTER)));

  const auto fcurves_pointer = make_pointer(pointer, FCURVES_POINTER);
  const std::size_t n = deserializer.array_size(fcurves_pointer);
  for (std::size_t i = 0; i < n; ++i) {
    const std::string type_pointer = make_pointer(fcurves_pointer, AbstractFCurve::TYPE_KEY);
    const std::string type = deserializer.get_string(type_pointer);
    auto fcurve = AbstractFCurve::make(type);
    m_fcurves.insert(std::move(fcurve));
  }
}

std::string Animator::map_property_to_fcurve_type(const std::string &property_type)
{
  // <TYPE>Property -> <TYPE>FCurve
  static const std::string property_suffix = "property";
  static const std::string fcurve_suffix = "fcurve";

  assert(property_type.size() > property_suffix.size());
  const std::string type = property_type.substr(0, property_type.size() - property_suffix.size());
  return type + fcurve_suffix;
}

AbstractFCurve &Animator::get_fcurve(AbstractPropertyOwner &owner, const std::string &property_key)
{
  const auto it = std::find_if(m_fcurves.begin(), m_fcurves.end(),
                               [&owner, &property_key](const auto& fcurve)
  {
    return fcurve->owner() == &owner && fcurve->property_key() == property_key;
  });

  if (it != m_fcurves.end()) {
    return **it;
  } else {
    Property* property = owner.property(property_key);
    auto fcurve = AbstractFCurve::make(map_property_to_fcurve_type(property->type()));
    AbstractFCurve& fcurve_ref = *fcurve;
    m_fcurves.insert(std::move(fcurve));
    return fcurve_ref;
  }
}

void Animator::set_start(int start)
{
  if (m_start_frame != start) {
    m_start_frame = start;
    Q_EMIT start_changed(start);
  }
}

void Animator::set_end(int end)
{
  if (m_end_frame != end) {
    m_end_frame = end;
    Q_EMIT end_changed(end);
  }
}

void Animator::set_current(int current)
{
  if (m_current_frame != current) {
    m_current_frame = current;
    Q_EMIT current_changed(current);
  }
}

void Animator::toggle_play_pause(bool play)
{
  if (m_is_playing != play) {
    m_is_playing = play;
    if (play) {
      m_timer.start();
    } else {
      m_timer.stop();
    }
    Q_EMIT play_pause_toggled(play);
  }
}

void Animator::advance()
{
  int next = m_current_frame + 1;
  if (next > m_end_frame) {
    if (m_play_mode == PlayMode::Repeat) {
      next = m_start_frame;
    } else if (m_play_mode == PlayMode::Stop) {
      next = m_end_frame;
      m_timer.stop();
    }
  }

  if (next != m_current_frame) {
    set_current(next);
  }
}

}  // namespace omm