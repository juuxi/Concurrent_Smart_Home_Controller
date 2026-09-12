cmake_minimum_required(VERSION 3.10)
project(Concurrent_Smart_Home_Controller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(smart_home_controller
    src/main.cpp
    src/DeviceServer.cpp
    src/LightDevice.cpp
)

target_include_directories(smart_home_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
