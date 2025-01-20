find_package(capstone REQUIRED)

add_library_ns(cstone capstone STATIC dm.cpp)
target_include_dir_iface(cstone-capstone PUBLIC ${capstone_INCLUDE_DIR} include)
install_target_and_headers(cstone capstone)
if(WIN32)
	target_link_libraries(cstone-capstone ${capstone_LIB_DIR}/capstone.lib)
endif()