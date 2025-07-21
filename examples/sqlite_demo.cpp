#include <iostream>
#include "psr_database/database.hpp"

int main() {
    std::cout << "=== PSR Database SQLite3 Integration Demo ===" << std::endl;
    
    try {
        // Create an in-memory database
        psr_database::Database db(":memory:");
        std::cout << "✅ Database created successfully" << std::endl;
        
        // Create a sample table
        if (!db.execute(R"(
            CREATE TABLE employees (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                department TEXT NOT NULL,
                salary REAL NOT NULL,
                hire_date TEXT NOT NULL
            )
        )")) {
            std::cerr << "❌ Failed to create table: " << db.get_last_error() << std::endl;
            return 1;
        }
        std::cout << "✅ Table 'employees' created" << std::endl;
        
        // Insert sample data using transaction
        {
            psr_database::Transaction trans(db);
            
            db.execute("INSERT INTO employees (name, department, salary, hire_date) VALUES ('Alice Johnson', 'Engineering', 85000.0, '2023-01-15')");
            db.execute("INSERT INTO employees (name, department, salary, hire_date) VALUES ('Bob Smith', 'Marketing', 65000.0, '2023-02-20')");
            db.execute("INSERT INTO employees (name, department, salary, hire_date) VALUES ('Carol Davis', 'Engineering', 92000.0, '2022-11-10')");
            db.execute("INSERT INTO employees (name, department, salary, hire_date) VALUES ('David Wilson', 'Sales', 58000.0, '2023-03-05')");
            
            if (!trans.commit()) {
                std::cerr << "❌ Failed to commit transaction" << std::endl;
                return 1;
            }
        }
        std::cout << "✅ Sample data inserted (" << db.get_changes() << " rows affected)" << std::endl;
        
        // Query all employees
        std::cout << "\n📊 All Employees:" << std::endl;
        auto all_employees = db.query("SELECT name, department, salary FROM employees ORDER BY salary DESC");
        
        for (const auto& row : all_employees) {
            std::cout << "  " << row[0] << " (" << row[1] << ") - $" << row[2] << std::endl;
        }
        
        // Query with filtering
        std::cout << "\n💰 High-earning Employees (>$80,000):" << std::endl;
        db.query_with_callback(
            "SELECT name, department, salary FROM employees WHERE salary > 80000 ORDER BY salary DESC",
            [](const std::vector<std::string>& row) {
                std::cout << "  " << row[0] << " (" << row[1] << ") - $" << row[2] << std::endl;
                return true;
            }
        );
        
        // Demonstrate aggregation
        auto dept_stats = db.query(R"(
            SELECT 
                department, 
                COUNT(*) as count, 
                ROUND(AVG(salary), 2) as avg_salary,
                MAX(salary) as max_salary
            FROM employees 
            GROUP BY department 
            ORDER BY avg_salary DESC
        )");
        
        std::cout << "\n📈 Department Statistics:" << std::endl;
        for (const auto& row : dept_stats) {
            std::cout << "  " << row[0] << ": " << row[1] << " employees, "
                      << "avg $" << row[2] << ", max $" << row[3] << std::endl;
        }
        
        // Demonstrate update operation
        if (db.execute("UPDATE employees SET salary = salary * 1.05 WHERE department = 'Engineering'")) {
            std::cout << "\n💸 Engineering salaries increased by 5% (" << db.get_changes() << " employees affected)" << std::endl;
        }
        
        std::cout << "\n✅ SQLite3 integration demo completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Demo failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
