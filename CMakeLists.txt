cmake_minimum_required(VERSION 3.14)

project(imap LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test Network)

add_library(imap SHARED
  src/imap_global.h
  src/imapclient.cpp
  src/imapclient.h
)

# test
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

enable_testing(true)

add_executable(mytest test/test_imap.cpp)
add_test(NAME mytest COMMAND mytest)

target_link_libraries(mytest PRIVATE Qt::Test)

target_link_libraries(imap PRIVATE Qt${QT_VERSION_MAJOR}::Core  Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Network)

target_compile_definitions(imap PRIVATE IMAPCLIENT_LIBRARY)
