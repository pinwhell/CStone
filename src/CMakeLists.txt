find_package(fmt REQUIRED)

file(GLOB_RECURSE CSTONE_SRCS *.cpp)

add_library_ns(cstone cstone STATIC ${CSTONE_SRCS})
target_compile_features(cstone-cstone PUBLIC cxx_std_20)
target_include_dir_iface(cstone-cstone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include include)
target_link_libraries(cstone-cstone capstone)
#install_target_and_headers(cstone cstone)