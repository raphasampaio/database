#include <string>

#include "psr_database/psr_database.hpp"

exported_class::exported_class()
    : m_name {"psr_database"}
{
}

auto exported_class::name() const -> char const*
{
  return m_name.c_str();
}
