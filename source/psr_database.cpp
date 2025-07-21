#include <string>
#include <iostream>

#include "psr_database/psr_database.hpp"
#include "psr_database/database.hpp"

exported_class::exported_class()
    : m_name {"psr_database"}
{
}

auto exported_class::name() const -> char const*
{
  return m_name.c_str();
}

bool exported_class::demo_database_operations() const
{
  try {
    // Create an in-memory database
    psr_database::Database db(":memory:");
    
    if (!db.is_open()) {
      std::cerr << "Failed to open database" << std::endl;
      return false;
    }
    
    // Create a sample table
    if (!db.execute("CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT, email TEXT)")) {
      std::cerr << "Failed to create table: " << db.get_last_error() << std::endl;
      return false;
    }
    
    // Insert some sample data
    {
      psr_database::Transaction trans(db);
      
      if (!db.execute("INSERT INTO users (name, email) VALUES ('John Doe', 'john@example.com')")) {
        std::cerr << "Failed to insert user 1: " << db.get_last_error() << std::endl;
        return false;
      }
      
      if (!db.execute("INSERT INTO users (name, email) VALUES ('Jane Smith', 'jane@example.com')")) {
        std::cerr << "Failed to insert user 2: " << db.get_last_error() << std::endl;
        return false;
      }
      
      if (!trans.commit()) {
        std::cerr << "Failed to commit transaction: " << db.get_last_error() << std::endl;
        return false;
      }
    }
    
    // Query the data
    auto results = db.query("SELECT id, name, email FROM users ORDER BY id");
    
    if (results.size() != 2) {
      std::cerr << "Expected 2 rows, got " << results.size() << std::endl;
      return false;
    }
    
    // Verify the data
    if (results[0].size() != 3 || results[0][1] != "John Doe" || results[0][2] != "john@example.com") {
      std::cerr << "First row data mismatch" << std::endl;
      return false;
    }
    
    if (results[1].size() != 3 || results[1][1] != "Jane Smith" || results[1][2] != "jane@example.com") {
      std::cerr << "Second row data mismatch" << std::endl;
      return false;
    }
    
    std::cout << "Database operations completed successfully!" << std::endl;
    std::cout << "Found " << results.size() << " users in the database" << std::endl;
    
    return true;
    
  } catch (const std::exception& e) {
    std::cerr << "Database demo failed with exception: " << e.what() << std::endl;
    return false;
  }
}
