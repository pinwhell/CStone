cmake_minimum_required(VERSION 3.15)

project(CStoneTests)

add_executable(CStoneTest TestMain.cpp)

target_link_libraries(CStoneTest PRIVATE cstone-cstone)

set_target_properties(CStoneTest PROPERTIES CXX_STANDARD 17)

add_test(NAME CStoneVerificationTest COMMAND CStoneTest)
