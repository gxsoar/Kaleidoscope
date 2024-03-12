cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_FLAGS -std=c++20)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd
set(CMAKE_CXX_STANDARD 20)  # compile as C++20
set(CMAKE_CXX_STANDARD_REQUIRED 20) # require C++20 support

project(Kaleidoscope)

if(NOT DEFINED KALEDIOSCOPE_SANITIZER)
  set(KALEDIOSCOPE_SANITIZER address)
endif()

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_FXX_FLAGS_DEBUG}")

# includes
set(KALEDIOSCOPE_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${KALEDIOSCOPE_SRC_INCLUDE_DIR})

SET(KALEIDOSCOPE_LIBS lexer parser)

add_library(lexer OBJECT ${PROJECT_SOURCE_DIR}/src/lexer.cpp)
add_library(parser OBJECT ${PROJECT_SOURCE_DIR}/src/parse.cpp)

add_executable(Kaleidoscope src/main.cpp)

target_link_libraries(Kaleidoscope ${KALEIDOSCOPE_LIBS})
target_include_directories(Kaleidoscope  PUBLIC 
                            $<KALEIDOSCOPE_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include)

# # "make format"
# # "make check-format"
# string(CONCAT KALEDIOSCOPE_FORMAT_DIRS 
#               "${CMAKE_CURRENT_SOURCE_DIR}/src,"
#               "${CMAKE_CURRENT_SOURCE_DIR}/include"
# )

# 