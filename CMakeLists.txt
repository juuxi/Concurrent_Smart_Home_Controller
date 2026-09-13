cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0016 NEW)

project(Concurrent_Smart_Home_Controller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_DEBUG_RUNTIME OFF)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Boost REQUIRED COMPONENTS log log_setup)
set(CMAKE_AUTOMOC ON)

add_executable(smart_home_controller
    src/main.cpp
    src/DeviceServer.cpp
    src/LightDevice.cpp
    src/TemperatureDevice.cpp
    src/desktop/SmartHomeDashboard.cpp
)

target_include_directories(smart_home_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(smart_home_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/desktop)
target_link_libraries(smart_home_controller PRIVATE Qt5::Widgets)
target_link_libraries(smart_home_controller PRIVATE ${Boost_LIBRARIES})
