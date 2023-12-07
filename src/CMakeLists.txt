project(CallHandlingCenter)


set(PROJECT_SOURCES
    main.cpp
    CallCenter/CallCenter.hpp
    CallCenter/CallCenter.cpp
    CDR/CDR.hpp
    CDR/CDR.cpp
    Call/Call.hpp
    Call/Call.cpp
    Call/CallQueue.hpp
    Call/CallQueue.cpp
    Operator/Operator.hpp
    Operator/Operator.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Boost::system
        Boost::log
        Boost::thread
        nlohmann_json::nlohmann_json
)

target_include_directories(${PROJECT_NAME}
    PRIVATE 
        {CMAKE_CURRENT_SOURCE_DIR}
        Call
        CallCenter
        CDR
        Operator
)