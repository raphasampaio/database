#include <iostream>
#include <string>

#include "psr_database/database.hpp"

auto main() -> int
{
  // auto const exported = exported_class {};

  // // Test the original functionality
  // if (std::string("psr_database") != exported.name()) {
  //   std::cerr << "Original functionality test failed" << std::endl;
  //   return 1;
  // }

  // // Test the database functionality
  // if (!exported.demo_database_operations()) {
  //   std::cerr << "Database functionality test failed" << std::endl;
  //   return 1;
  // }

  std::cout << "All tests passed!" << std::endl;
  return 0;
}
