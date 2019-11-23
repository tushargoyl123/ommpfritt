#pragma once

#include <memory>
#include <QWidget>
#include <set>

class QMenu;

namespace omm
{

class Property;
class Animator;
class AbstractPropertyOwner;

class AnimationButton : public QWidget
{
  Q_OBJECT
public:
  AnimationButton(Animator& animator, const std::map<AbstractPropertyOwner*, Property*>& properties,
                  QWidget* parent = nullptr);

  bool has_key() const;
  bool has_track() const;
  bool value_is_inconsistent() const;

public Q_SLOTS:
  void set_key();
  void remove_key();
  void remove_track();

Q_SIGNALS:
  void clicked();

protected:
  void resizeEvent(QResizeEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

private:
  Animator& m_animator;
  const std::map<AbstractPropertyOwner*, Property*> m_properties;
  std::unique_ptr<QMenu> make_interpolation_menu() const;
};

}  // namespace omm
