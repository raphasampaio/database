#include <iostream>
#include <stdexcept>

#include "psr_database/database.hpp"

#include <sqlite3.h>

namespace psr_database
{

Database::Database(const std::string& db_path)
    : m_db(nullptr)
    , m_db_path(db_path)
{
  int result = sqlite3_open(m_db_path.c_str(), &m_db);
  if (result != SQLITE_OK) {
    std::string error_msg = "Failed to open database: ";
    if (m_db) {
      error_msg += sqlite3_errmsg(m_db);
      sqlite3_close(m_db);
      m_db = nullptr;
    } else {
      error_msg += "Out of memory";
    }
    throw std::runtime_error(error_msg);
  }
}

Database::~Database()
{
  close();
}

Database::Database(Database&& other) noexcept
    : m_db(other.m_db)
    , m_db_path(std::move(other.m_db_path))
{
  other.m_db = nullptr;
}

Database& Database::operator=(Database&& other) noexcept
{
  if (this != &other) {
    close();
    m_db = other.m_db;
    m_db_path = std::move(other.m_db_path);
    other.m_db = nullptr;
  }
  return *this;
}

bool Database::is_open() const
{
  return m_db != nullptr;
}

bool Database::execute(const std::string& sql)
{
  if (!m_db) {
    return false;
  }

  char* error_msg = nullptr;
  int result = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &error_msg);

  if (result != SQLITE_OK) {
    if (error_msg) {
      std::cerr << "SQL error: " << error_msg << std::endl;
      sqlite3_free(error_msg);
    }
    return false;
  }

  return true;
}

std::vector<std::vector<std::string>> Database::query(const std::string& sql)
{
  std::vector<std::vector<std::string>> results;

  query_with_callback(sql,
                      [&results](const std::vector<std::string>& row)
                      {
                        results.push_back(row);
                        return true;  // Continue iteration
                      });

  return results;
}

bool Database::query_with_callback(const std::string& sql,
                                   std::function<bool(const std::vector<std::string>&)> callback)
{
  if (!m_db) {
    return false;
  }

  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

  if (result != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
    return false;
  }

  // Get column count
  int column_count = sqlite3_column_count(stmt);

  // Execute and fetch results
  while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
    std::vector<std::string> row;
    row.reserve(column_count);

    for (int i = 0; i < column_count; ++i) {
      const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
      row.emplace_back(text ? text : "");
    }

    if (!callback(row)) {
      break;  // Callback requested to stop
    }
  }

  sqlite3_finalize(stmt);

  if (result != SQLITE_DONE && result != SQLITE_ROW) {
    std::cerr << "Query execution failed: " << sqlite3_errmsg(m_db) << std::endl;
    return false;
  }

  return true;
}

std::string Database::get_last_error() const
{
  if (!m_db) {
    return "Database not open";
  }
  return sqlite3_errmsg(m_db);
}

int Database::get_changes() const
{
  if (!m_db) {
    return 0;
  }
  return sqlite3_changes(m_db);
}

long long Database::get_last_insert_rowid() const
{
  if (!m_db) {
    return 0;
  }
  return sqlite3_last_insert_rowid(m_db);
}

bool Database::begin_transaction()
{
  return execute("BEGIN TRANSACTION");
}

bool Database::commit_transaction()
{
  return execute("COMMIT");
}

bool Database::rollback_transaction()
{
  return execute("ROLLBACK");
}

void Database::close()
{
  if (m_db) {
    sqlite3_close(m_db);
    m_db = nullptr;
  }
}

// Transaction implementation

Transaction::Transaction(Database& db)
    : m_db(db)
    , m_committed(false)
{
  if (!m_db.begin_transaction()) {
    throw std::runtime_error("Failed to begin transaction: " + m_db.get_last_error());
  }
}

Transaction::~Transaction()
{
  if (!m_committed) {
    m_db.rollback_transaction();
  }
}

bool Transaction::commit()
{
  if (m_committed) {
    return false;
  }

  bool success = m_db.commit_transaction();
  if (success) {
    m_committed = true;
  }
  return success;
}

bool Transaction::is_active() const
{
  return !m_committed;
}

}  // namespace psr_database
