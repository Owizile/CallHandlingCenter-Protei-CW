mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build ./src
cmake --build ./tests
./tests/CDRTest
./tests/CallQueueTest
./tests/CallCenterConfigTest
cd src
./CallHandlingCenter