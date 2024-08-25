cmake_minimum_required(VERSION 3.29.5)
project(
  NoScript
  VERSION 0.1.0
  DESCRIPTION "JavaScript static type checker"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(
  -Wall # Enable most warnings
  -Wextra # Enable additional warnings
  -Werror # Treat warnings as errors
  -Wshadow # Warn when a variable declaration shadows another variable
  -Wnon-virtual-dtor # Warn when a class with virtual functions does not have a
                     # virtual destructor
  -pedantic # Enforce strict ISO C++ compliance
  -Wold-style-cast # Warn on C-style casts
  -Wcast-align # Warn on potential performance issues due to misaligned casts
  -Wunused # Warn on unused variables, parameters, etc.
  -Woverloaded-virtual # Warn when a base class function is hidden by a function
                       # with the same name in the derived class
  -Wconversion # Warn on implicit conversions that may change a value
  -Wsign-conversion # Warn on implicit conversions between signed and unsigned
                    # integers
  -Wnull-dereference # Warn if a null pointer dereference is detected
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

file(GLOB_RECURSE files "src/*.cc")
add_executable(noscript ${files})
