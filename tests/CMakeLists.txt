project(CallHandlingCenter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target CDRTest)

set(src_list
    CDRTest.cpp
    ${CMAKE_SOURCE_DIR}/src/CDR/CDR.hpp
    ${CMAKE_SOURCE_DIR}/src/CDR/CDR.cpp
    ${CMAKE_SOURCE_DIR}/src/Call/Call.hpp
    ${CMAKE_SOURCE_DIR}/src/Call/Call.cpp
)

add_executable(${target} ${src_list})

target_link_libraries(${target}
    PRIVATE
        GTest::gtest
        Boost::system
        Boost::log
)

target_include_directories(${target}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/Call
        ${CMAKE_SOURCE_DIR}/src/CDR
)

set(target CallQueueTest)

set(src_list
    CallQueueTest.cpp
    ${CMAKE_SOURCE_DIR}/src/CDR/CDR.hpp
    ${CMAKE_SOURCE_DIR}/src/CDR/CDR.cpp
    ${CMAKE_SOURCE_DIR}/src/Call/Call.hpp
    ${CMAKE_SOURCE_DIR}/src/Call/Call.cpp
    ${CMAKE_SOURCE_DIR}/src/Call/CallQueue.hpp
    ${CMAKE_SOURCE_DIR}/src/Call/CallQueue.cpp
)

add_executable(${target} ${src_list})

target_link_libraries(${target}
    PRIVATE
        GTest::gtest
        Boost::log
        Boost::thread
        Boost::system
)

target_include_directories(${target}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/Call
        ${CMAKE_SOURCE_DIR}/src/CDR
)

set(target CallCenterConfigTest)

set(src_list
    CallCenterConfigTest.cpp
    ${CMAKE_SOURCE_DIR}/src/CallCenter/CallCenterConfig.hpp
)

add_executable(${target} ${src_list})

target_link_libraries(${target}
    PRIVATE
        GTest::gtest
        Boost::log
        nlohmann_json::nlohmann_json
)

target_include_directories(${target}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/CallCenter
)
