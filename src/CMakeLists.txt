
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} srcs)

add_library(co ${srcs})
target_include_directories(co PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})