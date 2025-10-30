$ErrorActionPreference = "Stop"

conan build . -b=missing -s build_type=Debug -o build_tests=True

Push-Location build\Debug
ctest --verbose
Pop-Location
