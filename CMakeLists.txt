cmake_minimum_required(VERSION 3.9)
project(
  libudp
  VERSION "0.0.1"
  DESCRIPTION "Lightweight UDP package manipulation library"
  HOMEPAGE_URL "https://github.com/GlobularOne/libudp"
  LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###############################################################################
# Compiler/Linker configuration
###############################################################################

include(cmake/CompilerStandards.cmake)

set_c_standard()

include_directories(include)

###############################################################################
# Libraries
###############################################################################

include(cmake/Library.cmake)

generate_library(udp)

###############################################################################
# Tests
###############################################################################

# include(cmake/Tests.cmake)

# generate_build_tests()


###############################################################################
# Documentation
###############################################################################

# TODO

###############################################################################
# Installation
###############################################################################

include(cmake/Installation.cmake)

install_library(udp)

###############################################################################
# Packaging
###############################################################################

include(CPack)
