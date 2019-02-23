#pragma once

#include "objects/abstractproceduralpath.h"

namespace omm
{

class Scene;

class ProceduralPath : public AbstractProceduralPath
{
public:
  explicit ProceduralPath(Scene* scene);
  std::string type() const override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("any-context", "ProceduralPath");
  std::unique_ptr<Object> clone() const override;
  Flag flags() const override;

  static constexpr auto IS_CLOSED_PROPERTY_KEY = "closed";
  static constexpr auto CODE_PROPERTY_KEY = "code";
  static constexpr auto COUNT_PROPERTY_KEY = "count";

private:
  std::vector<Point> points() override;
  bool is_closed() const override;
};

}  // namespace omm
