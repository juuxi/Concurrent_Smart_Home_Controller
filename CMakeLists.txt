cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0016 NEW)

project(Concurrent_Smart_Home_Controller)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_DEBUG_RUNTIME OFF)
set(DESKTOP src/desktop)
set(DEVICE src/device)
set(SERVER src/server)
set(COMMON src/common)
set(PROTOCOL src/protocol)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Boost REQUIRED COMPONENTS log log_setup)
find_package(GTest REQUIRED)
find_package(Protobuf REQUIRED)
set(CMAKE_AUTOMOC ON)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOCOL}/Messages.proto)

add_executable(smart_home_controller
    ${SERVER}/main.cpp
    ${SERVER}/DeviceServer.cpp
    ${DEVICE}/LightDevice.cpp
    ${DEVICE}/TemperatureDevice.cpp
    ${SERVER}/ThreadPool.cpp
    ${COMMON}/utils.c
    ${COMMON}/RabbitMqClient.cpp
    ${PROTO_SRCS} ${PROTO_HDRS}
)

add_executable(dashboard
    ${DESKTOP}/main.cpp
    ${DESKTOP}/SmartHomeDashboard.cpp
    ${COMMON}/utils.c
    ${COMMON}/RabbitMqClient.cpp
    ${PROTO_SRCS} ${PROTO_HDRS}
)

add_subdirectory(rabbitmq-c)
include_directories(rabbitmq-c/librabbitmq)
include_directories(rabbitmq-c/include)

add_custom_command(TARGET smart_home_controller POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/${SERVER}/server_config.ini"
    "$<TARGET_FILE_DIR:smart_home_controller>/server_config.ini"
)
add_custom_command(TARGET dashboard POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/${DESKTOP}/styles.qss"
    "$<TARGET_FILE_DIR:smart_home_controller>/styles.qss"
)
add_custom_command(TARGET dashboard POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/${DESKTOP}/dashboard_config.ini"
    "$<TARGET_FILE_DIR:smart_home_controller>/dashboard_config.ini"
)

# add_executable(tests 
#     test/DeviceTests.cpp
#     ${DEVICE}/LightDevice.cpp
#     ${DEVICE}/TemperatureDevice.cpp
# )

target_include_directories(smart_home_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(smart_home_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/desktop)
target_include_directories(smart_home_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/device)
target_include_directories(smart_home_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/server)
target_include_directories(smart_home_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/common)
target_link_libraries(smart_home_controller PRIVATE ${Boost_LIBRARIES})
target_link_libraries(smart_home_controller PRIVATE rabbitmq::rabbitmq)
target_link_libraries(smart_home_controller PRIVATE ${Protobuf_LIBRARIES})
target_include_directories(smart_home_controller PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(dashboard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/common)
target_include_directories(dashboard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/desktop)
target_link_libraries(dashboard PRIVATE Qt5::Widgets)
target_link_libraries(dashboard PRIVATE rabbitmq::rabbitmq)
target_link_libraries(dashboard PRIVATE ${Protobuf_LIBRARIES})
target_include_directories(dashboard PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/desktop)
# target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/device)
# target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/server)
# target_link_libraries(tests PRIVATE GTest::gmock_main)
# target_link_libraries(tests PRIVATE Qt5::Widgets)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
