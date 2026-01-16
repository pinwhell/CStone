cmake_minimum_required(VERSION 3.15)

project (CStone)

add_compile_definitions(CAPSTONE_AARCH64_COMPAT_HEADER)

find_package(CBuildKit REQUIRED)


add_subdirectory(vendor)
add_subdirectory(src)

option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include)

install_cfgpkg(cstone
"
include(CMakeFindDependencyMacro)
find_dependency(fmt)
find_dependency(capstone)
include(\${CMAKE_CURRENT_LIST_DIR}/cstone-capstone-targets.cmake)
include(\${CMAKE_CURRENT_LIST_DIR}/cstone-cstone-targets.cmake)"
)