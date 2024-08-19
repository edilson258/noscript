cmake_minimum_required(VERSION 3.29.5)
project(
  NoScript
  VERSION 0.1.0
  DESCRIPTION "The LLVM Typescript compiler"
  LANGUAGES CXX)

file(GLOB_RECURSE files "src/*.cc")
add_executable(noscript ${files})
