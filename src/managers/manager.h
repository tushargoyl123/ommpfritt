#pragma once

#include <QDockWidget>
#include <QMenuBar>
#include <QVBoxLayout>
#include <memory>

#include "abstractfactory.h"
#include "keybindings/commandinterface.h"
#include "mainwindow/mainwindow.h"

namespace omm
{
class MainWindow;
class Scene;

class Manager
    : public QDockWidget
    , virtual public AbstractFactory<QString, false, Manager, Scene&>
    , virtual public CommandInterface
{
  Q_OBJECT  // Required for MainWindow::save_state

public:
  Manager(Manager&&) = delete;
  Manager(const Manager&) = delete;
  ~Manager() override;
  Manager& operator=(const Manager&) = delete;
  Manager& operator=(Manager&&) = delete;
  [[nodiscard]] Scene& scene() const;
  [[nodiscard]] bool is_visible() const;
  [[nodiscard]] bool is_locked() const
  {
    return m_is_locked;
  }

protected:
  Manager(const QString& title, Scene& scene);

  Scene& m_scene;
  void set_widget(std::unique_ptr<QWidget> widget);
  void keyPressEvent(QKeyEvent* e) override;

private:
  using QDockWidget::setWidget;  // use set_widget instead
  bool m_is_locked = false;

public Q_SLOTS:
  void set_locked(bool locked)
  {
    m_is_locked = locked;
  }
};

}  // namespace omm
