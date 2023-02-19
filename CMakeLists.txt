project("cpp-co")
cmake_minimum_required(VERSION 3.4.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(TARGET_WASM)
    set(CMAKE_SYSTEM_NAME WASI)
    set(CMAKE_SYSTEM_VERSION 1)
    set(CMAKE_SYSTEM_PROCESSOR wasm32)
    set(triple wasm32-wasi)

    set(WASI_SDK_PREFIX /Users/q540239/oss/wasi-sdk/build/install/opt/wasi-sdk)

    if(WIN32)
        set(WASI_HOST_EXE_SUFFIX ".exe")
    else()
        set(WASI_HOST_EXE_SUFFIX "")
    endif()

    set(CMAKE_C_COMPILER ${WASI_SDK_PREFIX}/bin/clang${WASI_HOST_EXE_SUFFIX})
    set(CMAKE_CXX_COMPILER ${WASI_SDK_PREFIX}/bin/clang++${WASI_HOST_EXE_SUFFIX})
    set(CMAKE_AR ${WASI_SDK_PREFIX}/bin/llvm-ar${WASI_HOST_EXE_SUFFIX})
    set(CMAKE_RANLIB ${WASI_SDK_PREFIX}/bin/llvm-ranlib${WASI_HOST_EXE_SUFFIX})
    set(CMAKE_SYSROOT ${WASI_SDK_PREFIX}/share/wasi-sysroot)
    set(CMAKE_C_COMPILER_TARGET ${triple})
    set(CMAKE_CXX_COMPILER_TARGET ${triple})

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

    set(CMAKE_CXX_LINK_FLAGS "-Wl,--allow-undefined,--export-table,--gc-sections,--no-entry -lstdc++ -z stack-size=8192 -nostartfiles")
    set(CMAKE_CXX_FLAGS "-fno-exceptions -fno-rtti -flto -DNDEBUG -O3")
else()
    set(CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/15.0.6/bin/clang++)
endif()

add_subdirectory(src)

if(TARGET_WASM)
    add_definitions(-DTARGET_WASM)
endif()

add_subdirectory(example)