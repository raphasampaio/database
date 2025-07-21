#include <string>

#include "psr_database/psr_database.hpp"

auto main() -> int
{
  auto const exported = exported_class {};

  return std::string("psr_database") == exported.name() ? 0 : 1;
}
