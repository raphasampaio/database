#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Build and run tests for PSR Database on Windows

.DESCRIPTION
    This script configures, builds, and runs the PSR Database project tests on Windows.
    It uses CMake with Visual Studio generator and runs the test suite using CTest.

.PARAMETER Configuration
    Build configuration (Debug or Release). Default is Release.

.PARAMETER Clean
    Whether to clean the build directory before building. Default is false.

.PARAMETER VerboseTests
    Whether to run tests with verbose output. Default is false.

.EXAMPLE
    .\build-and-test.ps1
    Build and run tests in Release configuration

.EXAMPLE
    .\build-and-test.ps1 -Configuration Debug -Clean -VerboseTests
    Clean build, build in Debug configuration, and run tests with verbose output
#>

param(
    [Parameter()]
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release",
    
    [Parameter()]
    [switch]$Clean,
    
    [Parameter()]
    [switch]$VerboseTests
)

# Set error handling
$ErrorActionPreference = "Stop"

# Colors for output
$Red = "`e[31m"
$Green = "`e[32m"
$Yellow = "`e[33m"
$Blue = "`e[34m"
$Reset = "`e[0m"

function Write-ColorOutput {
    param($Color, $Message)
    Write-Host "${Color}${Message}${Reset}"
}

function Write-Step {
    param($Message)
    Write-ColorOutput $Blue "==> $Message"
}

function Write-Success {
    param($Message)
    Write-ColorOutput $Green "✅ $Message"
}

function Write-Error {
    param($Message)
    Write-ColorOutput $Red "❌ $Message"
}

function Write-Warning {
    param($Message)
    Write-ColorOutput $Yellow "⚠️  $Message"
}

# Check if we're in the right directory
if (!(Test-Path "CMakeLists.txt")) {
    Write-Error "CMakeLists.txt not found. Please run this script from the project root directory."
    exit 1
}

Write-Step "Starting PSR Database build and test process"
Write-Host "Configuration: $Configuration"
Write-Host "Clean build: $Clean"
Write-Host "Verbose tests: $VerboseTests"
Write-Host ""

try {
    # Clean build directory if requested
    if ($Clean -and (Test-Path "build")) {
        Write-Step "Cleaning build directory"
        Remove-Item -Recurse -Force "build"
        Write-Success "Build directory cleaned"
    }

    # Configure the project
    Write-Step "Configuring project with CMake"
    
    & cmake -B build -G "Visual Studio 16 2019" -A x64 -Dpsr_database_DEVELOPER_MODE=ON -DBUILD_MCSS_DOCS=OFF
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configuration failed with exit code $LASTEXITCODE"
        exit 1
    }
    Write-Success "Project configured successfully"

    # Build the project
    Write-Step "Building project ($Configuration configuration)"
    
    & cmake --build build --config $Configuration
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed with exit code $LASTEXITCODE"
        exit 1
    }
    Write-Success "Project built successfully"

    # Run tests
    Write-Step "Running tests"
    
    if ($VerboseTests) {
        & ctest --test-dir build -C $Configuration --verbose
    } else {
        & ctest --test-dir build -C $Configuration
    }
    
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Tests failed with exit code $LASTEXITCODE"
        exit 1
    }
    Write-Success "All tests passed!"

    # Optional: Run the test executable directly to show it works
    Write-Step "Running test executable directly"
    $testExePath = "build\test\$Configuration\psr_database_test.exe"
    if (Test-Path $testExePath) {
        & $testExePath
        if ($LASTEXITCODE -eq 0) {
            Write-Success "Direct test execution completed successfully (exit code: 0)"
        } else {
            Write-Warning "Direct test execution returned exit code: $LASTEXITCODE"
        }
    } else {
        Write-Warning "Test executable not found at: $testExePath"
    }

    Write-Success "Build and test process completed successfully!"
    
} catch {
    Write-Error "An error occurred: $($_.Exception.Message)"
    exit 1
}
