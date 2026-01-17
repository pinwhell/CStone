cmake_minimum_required(VERSION 3.15)

project (CStone)

option(USE_STATIC_LINKING "Use static linking for the runtime" ON)

if (USE_STATIC_LINKING)
    if (MSVC)
        # MSVC specific static linking flag
        add_compile_options(/MT$<$<CONFIG:Debug>:d>)
    else()
        # GCC/Clang specific static linking flag
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    endif()
endif()




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
include(\${CMAKE_CURRENT_LIST_DIR}/cstone-cstone-targets.cmake)"
)