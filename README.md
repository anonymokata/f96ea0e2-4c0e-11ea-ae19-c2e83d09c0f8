# Point of Sale System

## Tool Install
The build process for this application is dependant upon tools that are available from the default Ubuntu PPA repositories. The C++ compilation tool chain and cmake are required to handle the build process.
`sudo apt-get install g++ cmake`
`mkdir -p lib`
`git clone https://github.com/google/googletest/`

## Build Process
The point of sale system can be built by issuing the following command. The make system will automatically download the Google Test Framework which is utilized by the test application.
`mkdir -p build`
`cd build`
`cmake ..`
`make`