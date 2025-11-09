cmake_minimum_required(VERSION 3.28)

project(NeoSnake LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

configure_file(${CMAKE_SOURCE_DIR}/header/resource_path.h.in
               ${GENERATED_DIR}/resource_path.h
               @ONLY)

include(FetchContent)
FetchContent_Declare(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        3.0.1
  GIT_SHALLOW    ON
  EXCLUDE_FROM_ALL
  SYSTEM
)
set(SFML_BUILD_EXAMPLES   OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

find_package(Threads REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/handlers/*.cpp
)

add_executable(NeoSnake ${SOURCES})
target_compile_features(NeoSnake PRIVATE cxx_std_17)

target_include_directories(NeoSnake PRIVATE
    ${GENERATED_DIR}
    ${CMAKE_SOURCE_DIR}/header
    ${CMAKE_SOURCE_DIR}/handlers
)

target_link_libraries(NeoSnake PRIVATE
    SFML::Graphics
)

add_custom_command(
  TARGET NeoSnake POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/resources
          $<TARGET_FILE_DIR:NeoSnake>/resources
  COMMENT "Copying resources next to the executable"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|AppleClang")
        target_compile_options(NeoSnake PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wunused -Werror -ggdb -O0)
        target_link_options(NeoSnake PRIVATE -ggdb)
    endif()
endif()
