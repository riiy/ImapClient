project(imaptest LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

enable_testing(true)

add_executable(imaptest test_imap.cpp)
add_test(NAME imaptest COMMAND imaptest)

target_link_libraries(imaptest PRIVATE Qt::Test)