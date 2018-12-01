#pragma once

#include <memory>
#include <vector>
#include "tags/tag.h"

namespace omm
{

class TagOwner
{
public:
  virtual ~TagOwner();
  Tag& attach_tag(std::unique_ptr<Tag> tag, const Tag* predecessor);
  std::unique_ptr<Tag> detach_tag(Tag& tag);
  size_t n_tags() const;
  size_t get_insert_position(const Tag* tag_before_position) const;
  Tag& tag(size_t i) const;
  std::vector<Tag*> tags();
private:
  std::vector<std::unique_ptr<Tag>> m_tags;
};

}  // namespace
