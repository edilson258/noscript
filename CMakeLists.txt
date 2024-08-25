cmake_minimum_required(VERSION 3.29.5)
project(
  NoScript
  VERSION 0.1.0
  DESCRIPTION "JavaScript static type checker"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE files "src/*.cc")
add_executable(noscript ${files})
