# Build and Test Scripts for Windows

This directory contains scripts to automate building and testing the PSR Database project on Windows.

## Available Scripts

### PowerShell Script: `build-and-test.ps1`

A feature-rich PowerShell script with colored output and comprehensive error handling.

#### Usage

```powershell
# Basic usage - builds in Release configuration
.\build-and-test.ps1

# Build in Debug configuration
.\build-and-test.ps1 -Configuration Debug

# Clean build (removes build directory first)
.\build-and-test.ps1 -Clean

# Run tests with verbose output
.\build-and-test.ps1 -VerboseTests

# Combine options
.\build-and-test.ps1 -Configuration Debug -Clean -VerboseTests
```

#### Parameters

- **Configuration**: Build configuration (`Debug` or `Release`). Default: `Release`
- **Clean**: Switch to clean the build directory before building
- **VerboseTests**: Switch to run tests with verbose output

#### Features

- ✅ Colored output with emojis for better readability
- ✅ Comprehensive error handling
- ✅ Automatic cleanup option
- ✅ Direct test execution verification
- ✅ Support for Debug and Release configurations

### Batch Script: `build-and-test.bat`

A simple batch file for users who prefer cmd or have restricted PowerShell execution policies.

#### Usage

```batch
# Basic usage - builds in Release configuration
build-and-test.bat

# Build in Debug configuration
build-and-test.bat Debug

# Clean build in Release configuration
build-and-test.bat Release clean

# Clean build in Debug configuration
build-and-test.bat Debug clean
```

#### Parameters

1. **Configuration** (optional): `Debug` or `Release`. Default: `Release`
2. **Clean** (optional): `clean` to remove build directory first

## Prerequisites

- **CMake** (version 3.14 or higher)
- **Visual Studio 2019** (or Visual Studio Build Tools)
- **Windows SDK** (automatically selected by CMake)

## What the Scripts Do

1. **Validate Environment**: Check that `CMakeLists.txt` exists in the current directory
2. **Clean Build** (if requested): Remove the existing `build` directory
3. **Configure**: Run CMake configuration with Visual Studio 2019 generator
4. **Build**: Compile the project in the specified configuration
5. **Test**: Run the test suite using CTest
6. **Verify**: Execute the test binary directly to confirm it works

## Output

The scripts will create a `build` directory containing:
- `Release/` or `Debug/`: Compiled library (`psr_database.lib`)
- `test/Release/` or `test/Debug/`: Test executable (`psr_database_test.exe`)

## Troubleshooting

### PowerShell Execution Policy

If you get an execution policy error when running the PowerShell script:

```powershell
# Temporarily allow script execution for current session
Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope Process

# Or run with bypass policy
powershell -ExecutionPolicy Bypass -File .\build-and-test.ps1
```

### Visual Studio Not Found

If CMake cannot find Visual Studio, ensure you have:
- Visual Studio 2019 with C++ development tools installed
- Or Visual Studio Build Tools 2019

### Build Errors

- Make sure you're running from the project root directory (where `CMakeLists.txt` is located)
- Try running with the `-Clean` option to start fresh
- Check that all prerequisites are installed

## Example Output

### Successful Run
```
==> Starting PSR Database build and test process
Configuration: Release
Clean build: True
Verbose tests: False

==> Cleaning build directory
✅ Build directory cleaned

==> Configuring project with CMake
-- Configuring done (10.0s)
-- Generating done (0.2s)
✅ Project configured successfully

==> Building project (Release configuration)
✅ Project built successfully

==> Running tests
1/1 Test #1: psr_database_test ................   Passed    0.25 sec
100% tests passed, 0 tests failed out of 1
✅ All tests passed!

==> Running test executable directly
✅ Direct test execution completed successfully (exit code: 0)

✅ Build and test process completed successfully!
```

## Integration with Development Workflow

You can integrate these scripts into your development workflow:

1. **Before committing**: Run `.\build-and-test.ps1 -Clean` to ensure everything builds and tests pass
2. **Debug issues**: Run `.\build-and-test.ps1 -Configuration Debug -VerboseTests` for detailed output
3. **CI/CD**: Use the batch script in automated environments where PowerShell might not be available
