#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "psr_database/psr_database_export.hpp"

struct sqlite3;
struct sqlite3_stmt;

namespace psr_database {

/**
 * @brief A simple wrapper around SQLite3 database
 * 
 * This class provides a C++ interface to SQLite3 database operations,
 * including opening/closing databases, executing SQL statements,
 * and handling prepared statements.
 */
class PSR_DATABASE_EXPORT Database
{
public:
    /**
     * @brief Construct a new Database object
     * 
     * @param db_path Path to the database file. Use ":memory:" for in-memory database
     */
    explicit Database(const std::string& db_path = ":memory:");
    
    /**
     * @brief Destroy the Database object and close the connection
     */
    ~Database();
    
    // Non-copyable but movable
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&& other) noexcept;
    Database& operator=(Database&& other) noexcept;
    
    /**
     * @brief Check if the database connection is open
     * 
     * @return true if connected, false otherwise
     */
    bool is_open() const;
    
    /**
     * @brief Execute a SQL statement without expecting results
     * 
     * @param sql The SQL statement to execute
     * @return true if successful, false otherwise
     */
    bool execute(const std::string& sql);
    
    /**
     * @brief Execute a SQL query and return results as a vector of rows
     * 
     * Each row is represented as a vector of strings.
     * 
     * @param sql The SQL query to execute
     * @return Vector of rows, where each row is a vector of column values
     */
    std::vector<std::vector<std::string>> query(const std::string& sql);
    
    /**
     * @brief Execute a SQL query with a callback for each row
     * 
     * @param sql The SQL query to execute
     * @param callback Function called for each row. Return false to stop iteration.
     * @return true if successful, false otherwise
     */
    bool query_with_callback(const std::string& sql, 
                           std::function<bool(const std::vector<std::string>&)> callback);
    
    /**
     * @brief Get the last error message
     * 
     * @return Error message string
     */
    std::string get_last_error() const;
    
    /**
     * @brief Get the number of rows affected by the last INSERT, UPDATE, or DELETE
     * 
     * @return Number of affected rows
     */
    int get_changes() const;
    
    /**
     * @brief Get the last inserted row ID
     * 
     * @return Last row ID, or 0 if no rows were inserted
     */
    long long get_last_insert_rowid() const;
    
    /**
     * @brief Begin a transaction
     * 
     * @return true if successful, false otherwise
     */
    bool begin_transaction();
    
    /**
     * @brief Commit the current transaction
     * 
     * @return true if successful, false otherwise
     */
    bool commit_transaction();
    
    /**
     * @brief Rollback the current transaction
     * 
     * @return true if successful, false otherwise
     */
    bool rollback_transaction();

private:
    sqlite3* m_db;
    std::string m_db_path;
    
    void close();
};

/**
 * @brief RAII wrapper for database transactions
 * 
 * Automatically begins a transaction on construction and rolls back
 * on destruction unless commit() is called.
 */
class PSR_DATABASE_EXPORT Transaction
{
public:
    /**
     * @brief Begin a transaction on the given database
     * 
     * @param db Database to start transaction on
     */
    explicit Transaction(Database& db);
    
    /**
     * @brief Destructor - rolls back if not committed
     */
    ~Transaction();
    
    // Non-copyable and non-movable
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    Transaction(Transaction&&) = delete;
    Transaction& operator=(Transaction&&) = delete;
    
    /**
     * @brief Commit the transaction
     * 
     * @return true if successful, false otherwise
     */
    bool commit();
    
    /**
     * @brief Check if the transaction is still active
     * 
     * @return true if active, false if committed or rolled back
     */
    bool is_active() const;

private:
    Database& m_db;
    bool m_committed;
};

} // namespace psr_database
