# SQLite3 Integration Example

This example demonstrates how to use the integrated SQLite3 functionality in the PSR Database project.

## Basic Usage

```cpp
#include "psr_database/database.hpp"
#include <iostream>

int main() {
    try {
        // Create a database (in-memory for this example)
        psr_database::Database db(":memory:");
        
        // Create a table
        if (!db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT, price REAL)")) {
            std::cerr << "Failed to create table: " << db.get_last_error() << std::endl;
            return 1;
        }
        
        // Insert data using a transaction
        {
            psr_database::Transaction trans(db);
            
            db.execute("INSERT INTO products (name, price) VALUES ('Laptop', 999.99)");
            db.execute("INSERT INTO products (name, price) VALUES ('Mouse', 29.99)");
            db.execute("INSERT INTO products (name, price) VALUES ('Keyboard', 79.99)");
            
            if (!trans.commit()) {
                std::cerr << "Failed to commit transaction" << std::endl;
                return 1;
            }
        }
        
        // Query data
        auto results = db.query("SELECT name, price FROM products WHERE price > 50 ORDER BY price DESC");
        
        std::cout << "Expensive products:" << std::endl;
        for (const auto& row : results) {
            std::cout << "- " << row[0] << ": $" << row[1] << std::endl;
        }
        
        // Use callback-based query for large result sets
        std::cout << "\nAll products:" << std::endl;
        db.query_with_callback("SELECT name, price FROM products ORDER BY name", 
                              [](const std::vector<std::string>& row) {
            std::cout << "- " << row[0] << ": $" << row[1] << std::endl;
            return true; // Continue iteration
        });
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## File-based Database

```cpp
// Create or open a file-based database
psr_database::Database db("my_database.sqlite3");

// The rest of the usage is the same as in-memory database
```

## Error Handling

The database wrapper provides several methods for error handling:

```cpp
psr_database::Database db(":memory:");

if (!db.execute("INVALID SQL")) {
    std::cout << "Error: " << db.get_last_error() << std::endl;
}

// Check affected rows
if (db.execute("UPDATE products SET price = price * 1.1")) {
    std::cout << "Updated " << db.get_changes() << " rows" << std::endl;
}

// Get last inserted row ID
if (db.execute("INSERT INTO products (name, price) VALUES ('New Item', 19.99)")) {
    std::cout << "Inserted row with ID: " << db.get_last_insert_rowid() << std::endl;
}
```

## Transaction Management

```cpp
psr_database::Database db(":memory:");

// Automatic rollback if not committed
{
    psr_database::Transaction trans(db);
    
    db.execute("INSERT INTO products (name, price) VALUES ('Test', 1.00)");
    
    // If we don't call trans.commit(), the transaction will be rolled back
    // when trans goes out of scope
    
    if (some_condition) {
        trans.commit(); // Only commit if condition is met
    }
}

// Manual transaction management
db.begin_transaction();
db.execute("INSERT INTO products (name, price) VALUES ('Manual', 2.00)");
if (error_occurred) {
    db.rollback_transaction();
} else {
    db.commit_transaction();
}
```

## Features

- **RAII Design**: Automatic resource management
- **Move Semantics**: Efficient transfer of database objects
- **Transaction Support**: Both automatic and manual transaction management
- **Error Handling**: Comprehensive error reporting
- **Flexible Querying**: Both result-collecting and callback-based queries
- **SQLite Extensions**: Enabled FTS4, FTS5, RTREE, and JSON1 extensions
