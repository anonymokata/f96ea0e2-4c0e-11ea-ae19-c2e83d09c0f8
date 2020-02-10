#!/bin/bash

# Step 1. Check for Google Test framework
if [ ! -d "$DIRECTORY" ]; then
  git clone https://github.com/google/googletest/
fi

# Step 2. Check if build directory needs to be built
if [ ! -d "$DIRECTORY" ]; then
  mkdir -p build
fi

# Step 3. Initiate the build process
cd build
cmake ..
make

# Step 4. Copy generated application to root for easy access
cd ..
cp ./build/test/PointOfSale_test ./PointOfSale_test