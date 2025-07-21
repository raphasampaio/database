@echo off
REM Build and run tests for PSR Database on Windows
REM Usage: build-and-test.bat [Debug|Release] [clean]

setlocal EnableDelayedExpansion

REM Default configuration
set CONFIG=Release
set CLEAN_BUILD=0

REM Parse command line arguments
if not "%1"=="" (
    if /i "%1"=="Debug" set CONFIG=Debug
    if /i "%1"=="Release" set CONFIG=Release
)

if not "%2"=="" (
    if /i "%2"=="clean" set CLEAN_BUILD=1
)

echo =^> Starting PSR Database build and test process
echo Configuration: %CONFIG%
echo Clean build: %CLEAN_BUILD%
echo.

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo Error: CMakeLists.txt not found. Please run this script from the project root directory.
    exit /b 1
)

REM Clean build directory if requested
if %CLEAN_BUILD%==1 (
    echo =^> Cleaning build directory
    if exist "build" rmdir /s /q "build"
    echo Build directory cleaned
)

REM Configure the project
echo =^> Configuring project with CMake
cmake -B build -G "Visual Studio 16 2019" -A x64 -Dpsr_database_DEVELOPER_MODE=ON -DBUILD_MCSS_DOCS=OFF
if errorlevel 1 (
    echo Error: CMake configuration failed
    exit /b 1
)
echo Project configured successfully

REM Build the project
echo =^> Building project (%CONFIG% configuration)
cmake --build build --config %CONFIG%
if errorlevel 1 (
    echo Error: Build failed
    exit /b 1
)
echo Project built successfully

REM Run tests
echo =^> Running tests
ctest --test-dir build -C %CONFIG% --verbose
if errorlevel 1 (
    echo Error: Tests failed
    exit /b 1
)
echo All tests passed!

REM Run the test executable directly
echo =^> Running test executable directly
set "TEST_EXE=build\test\%CONFIG%\psr_database_test.exe"
if exist "%TEST_EXE%" (
    "%TEST_EXE%"
    if errorlevel 1 (
        echo Warning: Direct test execution returned error code !errorlevel!
    ) else (
        echo Direct test execution completed successfully (exit code: 0)
    )
) else (
    echo Warning: Test executable not found at: %TEST_EXE%
)

echo.
echo Build and test process completed successfully!
