#pragma once

#include <memory>
#include <map>
#include <QMainWindow>
#include "keybindings/keybindings.h"
#include "keybindings/commandinterface.h"

namespace omm
{

class Application;
class Scene;
class Manager;

class MainWindow : public QMainWindow, public CommandInterface
{
  Q_OBJECT
public:
  explicit MainWindow(Application& app);
  void restore_state();
  void save_state();
  void call(const std::string& command) override;
  static std::map<std::string, QKeySequence> default_bindings();
  static constexpr auto TYPE = "MainWindow";
  std::string type() const override;
  void keyPressEvent(QKeyEvent* e) override;

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  Application& m_app;
  void add_menu(const std::string& title, const std::vector<std::string>& actions);
};

}  // namespace omm
