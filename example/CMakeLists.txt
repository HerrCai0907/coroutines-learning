
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} srcs)

if(TARGET_WASM)
    add_executable(co_demo.wasm ${srcs})
    target_link_libraries(co_demo.wasm PUBLIC co)
else()
    add_executable(co_demo ${srcs})
    target_link_libraries(co_demo PUBLIC co)
endif()
