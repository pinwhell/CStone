find_package(fmt REQUIRED)

file(GLOB_RECURSE CSTONE_SRCS *.cpp)

add_library_ns(cstone cstone STATIC ${CSTONE_SRCS})
target_include_dir_iface(cstone-cstone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include include)
target_link_libraries(cstone-cstone capstone fmt::fmt)
#install_target_and_headers(cstone cstone)