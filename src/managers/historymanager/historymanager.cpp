#include "managers/historymanager/historymanager.h"
#include "scene/history/historymodel.h"
#include <QListView>
#include "scene/scene.h"

namespace omm
{

HistoryManager::HistoryManager(Scene &scene)
  : Manager(tr("History"), scene)
  , m_model(scene.history())
{
  setObjectName(TYPE);
  auto view = std::make_unique<QListView>();
  m_view = view.get();
  set_widget(std::move(view));
  m_view->setModel(&m_model);
  connect(m_view, &QListView::doubleClicked, [&scene](const QModelIndex& index) {
    scene.history().set_index(index.row());
  });
  m_view->setSelectionMode(QAbstractItemView::NoSelection);
  connect(&m_model, &HistoryModel::rowsInserted, this, [this](const QModelIndex&, int row) {
    m_view->scrollTo(m_model.index(row));
  });
  connect(&m_model, &HistoryModel::dataChanged, this,
          [this](const QModelIndex& tl, const QModelIndex&)
  {
    m_view->scrollTo(tl);
  });
}

std::string HistoryManager::type() const { return TYPE; }

bool HistoryManager::perform_action(const std::string& name)
{
  LINFO << name;
  return false;
}

}  // namespace omm
