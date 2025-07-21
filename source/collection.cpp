#include <iostream>
#include <stdexcept>

#include "psr_database/collection.hpp"

#include <sqlite3.h>

namespace psr_database
{

Collection::Collection() {}

Collection::~Collection() {}

Collection::Collection(Collection&& other) noexcept {}

Collection& Collection::operator=(Collection&& other) noexcept
{
  return *this;
}

}  // namespace psr_database
