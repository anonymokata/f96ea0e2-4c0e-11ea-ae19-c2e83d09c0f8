# Point of Sale System

## Repository Layout
The repository for the project contains 3 directories. The docs directory contains any documentation that may exist regarding the project. A manual was generated via Doxygen for the API's defined in this project. The src directory contains the production source code for the project. The test directory contains all the test files for the project. The tests were broken up across a series of files based on what aspect of the system they were testing.

## Installation and Build
The project is written in C++ and utilizes the Google Test Framework for this project. The Google Test Framework provides the infrastructure for developing tests with minimal overhead. This allowed for focus to be placed on developing the tests rather than putting together the framework. The project build system is managed by cmake and handles the Point of Sale Test application and Google Test Framework.

First, the appropriate tools need to be download and installed from the appropriate repositories. This project was developed on a Ubuntu 18.04 system. Any debian based linux system should be compatible with these commands as well. The first command is to install the required tools.

`sudo apt-get install g++ cmake`

With the tools installed, the next step is to initiate the build process of the application and the associated test framework. A bash script was provided with the repository to automate the build process. Once complete, a PointOfSale_test application will be present in the root of the repository.

`./build.sh`

## Running the Application
Once built, the application can be exeuted just like any other application within Linux. There are no required arguments that need to be specified. 
The application will run through all the tests that have been developed. At the conclusion, the application will provide a summary on the number
of tests that were executed and how many passed.

`./PointOfSale_test`


Results:
```
[ RUN      ] WeightBasedItemTest.buyNGetMOffWithMarkdown
[       OK ] WeightBasedItemTest.buyNGetMOffWithMarkdown (0 ms)
[ RUN      ] WeightBasedItemTest.buyNGetMOffWithMarkdownWithLimit
[       OK ] WeightBasedItemTest.buyNGetMOffWithMarkdownWithLimit (0 ms)
[----------] 25 tests from WeightBasedItemTest (1 ms total)

[----------] Global test environment tear-down
[==========] 114 tests from 7 test suites ran. (8 ms total)
[  PASSED  ] 114 tests.
```