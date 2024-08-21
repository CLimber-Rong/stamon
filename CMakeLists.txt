cmake_minimum_required(VERSION 3.22.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Stamon VERSION 2.4.8)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

enable_testing()

macro(compressing_binary target)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            set(filename ${EXECUTABLE_OUTPUT_PATH}/${target}.exe)
        else()
            set(filename ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/${target}.exe)
        endif()
    else()
        set(filename ${EXECUTABLE_OUTPUT_PATH}/${target})
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_STRIP} "${filename}"
        )
    endif()
endif()
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
include_directories(${PROJECT_SOURCE_DIR}/src/tac)

set(CMAKE_C_FLAGS_DEBUG "-g -Og -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -DDEBUG")

set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "-std=c17")
    set(CMAKE_CXX_FLAGS "-std=c++17")

    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -march=native")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
    
    else()
    
    set(CMAKE_C_FLAGS "/std:c17")
    set(CMAKE_CXX_FLAGS "/std:c++17")
endif()

add_executable(stamon src/Main.cpp)
compressing_binary(stamon)

add_executable(stamon-test test/test.cpp)
add_executable(lexer-test test/lexer/test.cpp)
add_executable(parser-test test/parser/test.cpp)
# add_executable(preprocessor-test test/preprocessor/test.cpp) # 这个没法通过编译啊

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/src/bin-include" src)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            file(TO_NATIVE_PATH "${EXECUTABLE_OUTPUT_PATH}/include" tgt)
            add_custom_command(TARGET stamon POST_BUILD
                COMMAND Xcopy "${src}" "${tgt}" /s /e /i
            )
        else()
            file(TO_NATIVE_PATH "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/include" tgt)
            add_custom_command(TARGET stamon POST_BUILD
                COMMAND Xcopy "${src}" "${tgt}" /s /e /i
            )
        endif()
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_custom_command(TARGET stamon POST_BUILD
            COMMAND cp -r -T "${PROJECT_SOURCE_DIR}/src/bin-include" "${EXECUTABLE_OUTPUT_PATH}/include"
        )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Apple")
        add_custom_command(TARGET stamon POST_BUILD
            mkdir "${EXECUTABLE_OUTPUT_PATH}/include"
            COMMAND cp -r "${PROJECT_SOURCE_DIR}/src/bin-include/" "${EXECUTABLE_OUTPUT_PATH}/include"
        )
endif()

# if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#         if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#             set(stamon_binary ${EXECUTABLE_OUTPUT_PATH}/stamon.exe)
#             set(test_binary ${EXECUTABLE_OUTPUT_PATH}/stamon-test.exe)
#         else()
#             set(stamon_binary ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/stamon.exe)
#             set(test_binary ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/stamon-test.exe)
#         endif()
# else()
#     set(stamon_binary ${EXECUTABLE_OUTPUT_PATH}/stamon)
#     set(test_binary ${EXECUTABLE_OUTPUT_PATH}/stamon-test)
# endif()

# add_test(NAME stamon
#     COMMAND ${stamon_binary} build code.st code.stvc
# )

# add_test(NAME test
#     COMMAND ${test_binary}
# )
