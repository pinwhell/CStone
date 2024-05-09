file(GLOB_RECURSE CSTONE_SRCS *.cpp)

add_library(CStone STATIC ${CSTONE_SRCS})
target_include_directories(CStone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(CStone capstone fmt)