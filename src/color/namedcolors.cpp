#include "color/namedcolors.h"
#include <ostream>
#include <iomanip>
#include "logging.h"
#include "serializers/abstractserializer.h"

namespace omm
{

int NamedColors::rowCount(const QModelIndex& parent) const
{
  assert(!parent.isValid());
  return m_named_colors.size();
}

QVariant NamedColors::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }
  const auto& [ name, color ] = m_named_colors[index.row()];
  switch (role) {
  case Qt::BackgroundRole:
    return color.to_qcolor();
  case Qt::DisplayRole:
    return QString::fromStdString(name);
  default:
    return QVariant();
  }
}

bool NamedColors::setData(const QModelIndex& index, const QVariant& data, int role)
{
  if (!index.isValid() || role != Qt::EditRole) {
    return false;
  }

  const std::string name = data.toString().toStdString();
  if (name.empty() || has_color(name)) {
    return false;
  } else {
    m_named_colors[index.row()].first = name;
    Q_EMIT dataChanged(index, index, { Qt::DisplayRole });
    return true;
  }
}

Color* NamedColors::resolve(const std::string& name)
{
  const auto it = std::find_if(m_named_colors.begin(), m_named_colors.end(),
                               [name](const auto& pair)
  {
    return pair.first == name;
  });
  if (it == m_named_colors.end()) {
    return nullptr;
  } else {
    return &it->second;
  }
}

const Color* NamedColors::resolve(const std::string& name) const
{
  return const_cast<const Color*>(const_cast<NamedColors*>(this)->resolve(name));
}

bool NamedColors::resolve(const std::string& name, Color& color) const
{
  const Color* c = resolve(name);
  if (c != nullptr) {
    color = *c;
    return true;
  } else {
    return false;
  }
}

Color NamedColors::color(const QModelIndex& index) const
{
  assert(index.isValid());
  assert(index.model() == this);
  return m_named_colors[index.row()].second;
}

bool NamedColors::has_color(const std::string& name) const
{
  const auto it = std::find_if(m_named_colors.begin(), m_named_colors.end(),
                               [name](const auto& pair)
  {
    return pair.first == name;
  });
  return it != m_named_colors.end();
}

Qt::ItemFlags NamedColors::flags(const QModelIndex& index) const
{
  Q_UNUSED(index)
  return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void NamedColors::set_color(const QModelIndex& index, const Color& color)
{
  assert(index.isValid() && index.model() == this);
  m_named_colors[index.row()].second = color;
  Q_EMIT dataChanged(index, index, { Qt::BackgroundRole, Qt::ForegroundRole });
}

void NamedColors::serialize(AbstractSerializer& serializer, const Serializable::Pointer& p) const
{
  serializer.start_array(m_named_colors.size(), p);
  for (std::size_t i = 0; i < m_named_colors.size(); ++i) {
    const auto& [name, color] = m_named_colors[i];
    serializer.set_value(name, Serializable::make_pointer(p, i, "name"));
    serializer.set_value(color, Serializable::make_pointer(p, i, "color"));
  }
  serializer.end_array();
}

void NamedColors::deserialize(AbstractDeserializer& deserializer, const Serializable::Pointer& p)
{
  const std::size_t n = deserializer.array_size(make_pointer(p));
  beginResetModel();
  m_named_colors.clear();
  m_named_colors.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    const auto name = deserializer.get_string(make_pointer(p, i, "name"));
    const auto color = deserializer.get_color(make_pointer(p, i, "color"));
    m_named_colors.push_back(std::pair(name, color));
  }
  endResetModel();
}

QModelIndex NamedColors::add(const Color& color)
{
  const auto default_name = [this]() {
    const std::string default_name = tr("Unnamed Color").toStdString();
    std::string candidate = default_name;
    std::size_t i = 0;
    while (has_color(candidate)) {
      i += 1;
      std::ostringstream ostream;
      ostream << default_name << "." << std::setw(3) << std::setfill('0') << i;
      candidate = ostream.str();
    }
    return candidate;
  };
  const std::size_t n = m_named_colors.size();
  beginInsertRows(QModelIndex(), n, n);
  m_named_colors.push_back(std::pair(default_name(), color));
  endInsertRows();
  return index(n, 0, QModelIndex());
}

void NamedColors::remove(const QModelIndex& index)
{
  assert(index.isValid());
  assert(index.model() == this);
  const std::size_t n = index.row();
  beginRemoveRows(QModelIndex(), n, n);
  m_named_colors.erase(m_named_colors.begin() + n);
  endRemoveRows();
}

}  // namespace omm