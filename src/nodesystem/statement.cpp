#include "nodesystem/statement.h"
#include "common.h"
#include "nodesystem/node.h"
#include "nodesystem/port.h"

namespace omm
{
NodeStatement::NodeStatement(const Node& node) : node(node)
{
}

std::set<const AbstractPort*> NodeStatement::defines() const
{
  return ::transform<const AbstractPort*>(node.ports<OutputPort>(), ::identity);
}

std::set<const AbstractPort*> NodeStatement::uses() const
{
  return ::transform<const AbstractPort*>(node.ports<InputPort>(), ::identity);
}

bool NodeStatement::is_connection() const
{
  return false;
}

ConnectionStatement::ConnectionStatement(const OutputPort& source, const InputPort& target)
    : source(source), target(target)
{
}

std::set<const AbstractPort*> ConnectionStatement::defines() const
{
  return {&target};
}

std::set<const AbstractPort*> ConnectionStatement::uses() const
{
  return {&source};
}

bool ConnectionStatement::is_connection() const
{
  return true;
}

bool Statement::operator<(const Statement& other) const
{
  static constexpr auto dont_care = false;  // could be true as well, value does not matter.

  static const auto intersect = [](const auto& set_a, const auto& set_b) {
    static_assert(std::is_same_v<std::decay_t<decltype(set_a)>, std::decay_t<decltype(set_b)>>);
    using T = std::decay_t<decltype(set_a)>;
    T intersection;
    std::set_intersection(set_a.begin(),
                          set_a.end(),
                          set_b.begin(),
                          set_b.end(),
                          std::inserter(intersection, intersection.begin()));
    return intersection;
  };

  const bool smaller = !intersect(this->defines(), other.uses()).empty();
  const bool greater = !intersect(other.defines(), this->uses()).empty();
  const bool conflict = !intersect(other.defines(), this->defines()).empty();
  if (smaller && greater) {
    LERROR << "dependency cycle!";
    return dont_care;
  } else if (conflict) {
    LERROR << "multiple declaration!";
    return dont_care;
  } else {
    return smaller;
  }
}

std::ostream& operator<<(std::ostream& ostream, const Statement& statement)
{
  const auto format = [](const auto& set) -> QStringList {
    return ::transform<QString, QList>(set, [](const auto* port) { return port->uuid(); });
  };
  ostream << QString("%1[%2 <= %3]")
                 .arg(statement.is_connection() ? "connection" : "node",
                      format(statement.defines()).join(", "),
                      format(statement.uses()).join(", "))
                 .toStdString();
  return ostream;
}
}  // namespace omm
