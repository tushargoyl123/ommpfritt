#pragma once

#include "managers/manager.h"
#include "managers/itemmanager.h"
#include "managers/objectmanager/objecttreeview.h"
#include "keybindings/commandinterface.h"

namespace omm
{

class ObjectManager : public Manager::Registrar<ObjectManager, ItemManager<ObjectTreeView>>
{
  Q_OBJECT
public:
  explicit ObjectManager(Scene& scene);
  bool perform_action(const QString& name) override;
  static constexpr auto TYPE = QT_TRANSLATE_NOOP("any-context", "ObjectManager");
  QString type() const override;

protected:
  void contextMenuEvent(QContextMenuEvent* event) override;
};

}  // namespace omm
