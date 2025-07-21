# PSR Database

[![codecov](https://codecov.io/gh/raphasampaio/database/graph/badge.svg?token=naQ3INKLPl)](https://codecov.io/gh/raphasampaio/database)

A modern C++17 library providing SQLite3 database integration with RAII principles and comprehensive error handling.

## Features

- 🗄️ **SQLite3 Integration**: Full SQLite3 database support with automatic source downloading
- 🔒 **RAII Design**: Automatic resource management for databases and transactions
- ⚡ **Modern C++**: C++17 features, move semantics, and type safety
- 🛡️ **Error Handling**: Comprehensive error reporting and status checking
- 🔄 **Transaction Support**: Both automatic (RAII) and manual transaction management
- 📊 **Flexible Querying**: Result-collecting and callback-based query methods
- 🔧 **SQLite Extensions**: FTS4, FTS5, RTREE, and JSON1 extensions enabled

## Quick Start

### Basic Database Operations

```cpp
#include "psr_database/database.hpp"

// Create an in-memory database
psr_database::Database db(":memory:");

// Create a table
db.execute("CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT, email TEXT)");

// Insert data with transaction safety
{
    psr_database::Transaction trans(db);
    db.execute("INSERT INTO users (name, email) VALUES ('John', 'john@example.com')");
    db.execute("INSERT INTO users (name, email) VALUES ('Jane', 'jane@example.com')");
    trans.commit();
}

// Query data
auto results = db.query("SELECT name, email FROM users");
for (const auto& row : results) {
    std::cout << row[0] << ": " << row[1] << std::endl;
}
```

### File-based Database

```cpp
// Create or open a file-based database
psr_database::Database db("my_app.sqlite3");

// Use exactly the same API as in-memory databases
```

## Building

### Windows (PowerShell)

```powershell
# Basic build
.\build-and-test.ps1

# Debug build with verbose tests
.\build-and-test.ps1 -Configuration Debug -VerboseTests

# Clean build
.\build-and-test.ps1 -Clean
```

### Windows (Command Prompt)

```batch
# Basic build
build-and-test.bat

# Debug build
build-and-test.bat Debug

# Clean build
build-and-test.bat Release clean
```

### Prerequisites

- CMake 3.14 or higher
- Visual Studio 2019 (or Build Tools)
- Internet connection (for SQLite3 download on first build)

## API Reference

### Database Class

The main `psr_database::Database` class provides the following methods:

#### Constructor
```cpp
Database(const std::string& db_path = ":memory:")
```
Creates a database connection. Use ":memory:" for in-memory databases.

#### Basic Operations
```cpp
bool execute(const std::string& sql)                    // Execute SQL without results
std::vector<std::vector<std::string>> query(const std::string& sql)  // Query with results
bool query_with_callback(const std::string& sql, callback_func)      // Query with callback
```

#### Status and Error Handling
```cpp
bool is_open() const                          // Check if database is open
std::string get_last_error() const           // Get last error message
int get_changes() const                       // Get affected row count
long long get_last_insert_rowid() const      // Get last inserted row ID
```

#### Transaction Management
```cpp
bool begin_transaction()                      // Begin transaction
bool commit_transaction()                     // Commit transaction
bool rollback_transaction()                   // Rollback transaction
```

### Transaction Class (RAII)

```cpp
{
    psr_database::Transaction trans(db);
    // Perform database operations
    // Automatically rolls back unless commit() is called
    trans.commit();  // Explicitly commit
}
```

## Examples

The `examples/` directory contains demonstration programs:

### SQLite Demo
```bash
# After building, run the demo
./build/examples/sqlite_demo
```

This demonstrates:
- Database creation and table setup
- Transaction usage
- Data insertion and querying
- Aggregation and filtering
- Error handling

## Error Handling

```cpp
psr_database::Database db(":memory:");

// Check execution success
if (!db.execute("CREATE TABLE test (id INTEGER)")) {
    std::cerr << "Error: " << db.get_last_error() << std::endl;
}

// Exception handling for constructor
try {
    psr_database::Database file_db("/invalid/path/db.sqlite");
} catch (const std::exception& e) {
    std::cerr << "Database creation failed: " << e.what() << std::endl;
}
```

## Advanced Usage

### Large Result Sets
For large result sets, use callback-based queries to avoid memory issues:

```cpp
db.query_with_callback("SELECT * FROM large_table", 
    [](const std::vector<std::string>& row) {
        // Process each row individually
        process_row(row);
        return true;  // Continue iteration
    });
```

### Performance Tips
- Use transactions for bulk operations
- Use prepared statements for repeated queries
- Consider using file-based databases for persistence
- Enable WAL mode for better concurrency (if needed)

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Run tests: `.\build-and-test.ps1 -VerboseTests`
5. Submit a pull request

## License

This project is licensed under the terms specified in the LICENSE file.
