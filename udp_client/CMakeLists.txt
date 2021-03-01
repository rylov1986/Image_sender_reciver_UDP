cmake_minimum_required(VERSION 3.1)

project(udp_client LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)

set(SOURCES main.cpp udp_client.cpp)
set(HEADERS udp_client.h)

add_executable(udp_client
  ${SOURCES} ${HEADERS}
)
target_link_libraries(udp_client Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)
