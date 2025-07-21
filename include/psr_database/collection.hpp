#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "psr_database/psr_database_export.hpp"

struct sqlite3;
struct sqlite3_stmt;

namespace psr_database
{

class PSR_DATABASE_EXPORT Collection
{
public:
  explicit Collection();

  ~Collection();

  // Non-copyable but movable
  Collection(const Collection&) = delete;
  Collection& operator=(const Collection&) = delete;
  Collection(Collection&& other) noexcept;
  Collection& operator=(Collection&& other) noexcept;
};

}  // namespace psr_database
