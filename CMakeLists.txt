cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Stamon VERSION 2.4.4)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

enable_testing()

macro(compressing_binary target)
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(filename ${EXECUTABLE_OUTPUT_PATH}/${target}.exe)
    else()
        set(filename ${EXECUTABLE_OUTPUT_PATH}/${target})
    endif()
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_STRIP} "${filename}"
        COMMAND upx -9 "${filename}"
    )
endmacro(compressing_binary)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/stdc_implemented)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/ast)
include_directories(${PROJECT_SOURCE_DIR}/src/data_type)
include_directories(${PROJECT_SOURCE_DIR}/src/vm)
include_directories(${PROJECT_SOURCE_DIR}/src/ir)
include_directories(${PROJECT_SOURCE_DIR}/src/compiler)
include_directories(${PROJECT_SOURCE_DIR}/src/sfn)

set(CMAKE_C_FLAGS "-std=c17")
set(CMAKE_CXX_FLAGS "-std=c++11")

set(CMAKE_C_FLAGS_DEBUG "-g -Og -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -DDEBUG")

set(CMAKE_C_FLAGS_RELEASE "-O2 -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native")

add_executable(stamon src/Main.cpp)
compressing_binary(stamon)

add_executable(stamon-test test/test.cpp)
compressing_binary(stamon-test)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(stamon_binary ${EXECUTABLE_OUTPUT_PATH}/stamon.exe)
    set(test_binary ${EXECUTABLE_OUTPUT_PATH}/stamon-test.exe)
else()
    set(stamon_binary ${EXECUTABLE_OUTPUT_PATH}/stamon)
    set(test_binary ${EXECUTABLE_OUTPUT_PATH}/stamon-test)
endif()

# add_test(NAME stamon
#     COMMAND ${stamon_binary} build code.st code.stvc
# )

# add_test(NAME test
#     COMMAND ${test_binary}
# )
