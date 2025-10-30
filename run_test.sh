#!/bin/bash

conan build . -b=missing -s build_type=Debug -o build_tests=True

cd build/Debug
ctest --verbose
cd ..
cd ..
