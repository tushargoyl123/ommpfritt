#include "scene/tree.h"
#include "scene/contextes.h"

namespace omm
{

template<typename T> Tree<T>::Tree(Scene& scene, std::unique_ptr<T> root)
  : Structure<T>(scene)
  , m_root(std::move(root))
{
}

template<typename T> Tree<T>::~Tree()
{
}

template<typename T> T& Tree<T>::root() const
{
  return *m_root;
}

template<typename T> void Tree<T>::move(TreeMoveContext<T>& context)
{
  assert(context.is_valid());
  Object& old_parent = context.subject.get().parent();

  const auto guards = observed_type::template transform<std::unique_ptr<AbstractRAIIGuard>>(
    [&context](auto* observer) { return observer->acquire_mover_guard(context); }
  );
  context.parent.get().adopt(old_parent.repudiate(context.subject), context.predecessor);

  this->invalidate_recursive();
}

template<typename T> void Tree<T>::insert(TreeOwningContext<T>& context)
{
  assert(context.subject.owns());

  const auto guards = observed_type::template transform<std::unique_ptr<AbstractRAIIGuard>>(
    [&context] (auto* observer) {
      return observer->acquire_inserter_guard(context.parent, context.get_insert_position());
    }
  );
  context.parent.get().adopt(context.subject.release(), context.predecessor);

  this->invalidate_recursive();
}

template<typename T> T& Tree<T>::insert(std::unique_ptr<T> item)
{
  size_t n = root().children().size();
  const auto guards = observed_type::template transform<std::unique_ptr<AbstractRAIIGuard>>(
    [this, n] (auto* observer) { return observer->acquire_inserter_guard(root(), n); }
  );

  T& ref = root().adopt(std::move(item));

  this->invalidate_recursive();
  return ref;
}

template<typename T> void Tree<T>::remove(TreeOwningContext<T>& context)
{
  assert(!context.subject.owns());

  const auto guards = observed_type::template transform<std::unique_ptr<AbstractRAIIGuard>>(
    [&context](auto* observer) { return observer->acquire_remover_guard(context.subject); }
  );
  context.subject.capture(context.parent.get().repudiate(context.subject));

  this->invalidate_recursive();
}

template<typename T> std::unique_ptr<T> Tree<T>::remove(T& t)
{
  const auto guards = observed_type::template transform<std::unique_ptr<AbstractRAIIGuard>>(
    [&t](auto* observer) { return observer->acquire_remover_guard(t); }
  );
  assert(!t.is_root());
  auto item = t.parent().repudiate(t);
  this->invalidate_recursive();
  return item;
}

template<typename T>
std::unique_ptr<T> Tree<T>::replace_root(std::unique_ptr<T> new_root)
{
  const auto guards = observed_type::template transform<std::unique_ptr<AbstractRAIIGuard>>(
    [this](auto* observer) { return observer->acquire_reseter_guard(); }
  );
  auto old_root = std::move(m_root);
  m_root = std::move(new_root);
  this->invalidate_recursive();
  return old_root;
}

template<typename T> std::set<T*> Tree<T>::items() const
{
  if (m_item_cache_is_dirty) {
    m_item_cache_is_dirty = false;
    m_item_cache = root().all_descendants();
  }
  return m_item_cache;
}

template<typename T> size_t Tree<T>::position(const T& item) const
{
  return item.position();
}

template<typename T> const T* Tree<T>::predecessor(const T& sibling) const
{
  assert(!sibling.is_root());
  const auto pos = position(sibling);
  if (pos == 0) {
    return nullptr;
  } else {
    return &sibling.parent().child(pos - 1);
  }
}

template<typename T> void Tree<T>::invalidate()
{
  m_item_cache_is_dirty = true;
}

template class Tree<Object>;

}  // namespace