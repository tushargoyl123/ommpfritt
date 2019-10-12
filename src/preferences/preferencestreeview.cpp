#include "preferences/preferencestreeview.h"
#include <QHeaderView>
#include "keybindings/keybindings.h"
#include "logging.h"
#include "preferences/preferencestreeviewdelegate.h"

namespace omm
{

PreferencesTreeView::PreferencesTreeView(QWidget* parent)
  : QTreeView(parent)
{
  connect(this, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(update_column_width()));
  connect(this, SIGNAL(expanded(const QModelIndex&)), this, SLOT(update_column_width()));
}

PreferencesTreeView::~PreferencesTreeView()
{
}

void PreferencesTreeView::
set_model(QAbstractItemModel& model,
          std::vector<std::unique_ptr<AbstractPreferencesTreeViewDelegate>> delegates)
{
  assert(this->model() == nullptr);
  setModel(&model);
  m_column_delegates = std::move(delegates);
  for (std::size_t i = 0; i < m_column_delegates.size(); ++i) {
    m_column_delegates.at(i)->set_model(model);
    setItemDelegateForColumn(i+1, m_column_delegates.at(i).get());
  }
  setSelectionMode(QAbstractItemView::NoSelection);
}

void PreferencesTreeView::transfer_editor_data_to_model()
{
  for (auto&& delegate : m_column_delegates) {
    delegate->transfer_editor_data_to_model();
  }
}

void PreferencesTreeView::resizeEvent(QResizeEvent* event)
{
  update_column_width();
  QTreeView::resizeEvent(event);
}

void PreferencesTreeView::update_column_width()
{
  const int required_width_name = sizeHintForColumn(0);
  const int required_width_sequence = sizeHintForColumn(1);
  const int available_width = viewport()->width();
  static constexpr int max_width_sequence = 350;

  const int excess = std::max(0, available_width - required_width_name - required_width_sequence);
  const int width_sequence = std::max(required_width_sequence,
                                  std::min(max_width_sequence, required_width_sequence + excess));
  const int width_name = std::max(available_width - width_sequence, required_width_name);

  setColumnWidth(0, width_name);
  const int n = header()->count();
  const double other_width = static_cast<double>(width_sequence) / static_cast<double>(n-1);
  for (int i = 1; i < n; ++i) {
    setColumnWidth(i, other_width);
  }
}

}  // namespace omm