#pragma once

#include <stack>
#include "managers/manager.h"
#include <memory>
#include <string>
#include "keybindings/commandinterface.h"
#include "common.h"

namespace omm
{

class CodeEdit;
class ReferenceLineEdit;

class PythonConsole : public Manager
{
  Q_OBJECT
public:
  explicit PythonConsole(Scene& scene);
  ~PythonConsole();

  static constexpr auto TYPE = QT_TRANSLATE_NOOP("any-context", "PythonConsole");
  std::string type() const override;
  void populate_menu(QMenu& menu) override;
  bool perform_action(const std::string &name) override;

protected:
  bool eventFilter(QObject* object, QEvent* event) override;

private:
  std::unique_ptr<QMenuBar> make_menu_bar();
  CodeEdit* m_commandline;
  CodeEdit* m_output;
  QVBoxLayout* m_layout;
  ReferenceLineEdit* m_associated_item_widget;
  void eval();
  bool accept(const void* associated_item) const;
  void clear();


  void get_previous_command();
  void get_next_command();
  void push_command(const std::string& command);

  std::list<std::string> m_command_stack;
  std::list<std::string>::iterator m_command_stack_pointer = m_command_stack.end();

  static constexpr Qt::KeyboardModifiers caption_modifiers = Qt::ControlModifier;

private Q_SLOTS:
  void on_output(const void* associated_item, std::string content, Stream stream);

private:
  // allocation on stack issues strange compiler warnings
  void* m_locals = nullptr;

};

}  // namespace omm
