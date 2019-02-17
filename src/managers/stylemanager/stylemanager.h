#pragma once

#include "managers/itemmanager.h"
#include "managers/stylemanager/stylelistview.h"
#include "keybindings/commandinterface.h"

namespace omm
{

class StyleManager : public ItemManager<StyleListView>, public CommandInterface
{
  DECLARE_MANAGER_TYPE(StyleManager)

public:
  explicit StyleManager(Scene& scene);
  void call(const std::string& command) override;
  static std::map<std::string, QKeySequence> default_bindings();

protected:
  void keyPressEvent(QKeyEvent* event) override;
};

}  // namespace omm
