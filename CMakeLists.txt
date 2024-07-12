cmake_minimum_required(VERSION 3.5)

project(libctlv LANGUAGES C VERSION 1.0 DESCRIPTION "A simple TLV utility for C/C++")

# Headers dir
include_directories(include/)
# Build cTLV as shared library
add_library(ctlv SHARED src/ctlv.c include/ctlv.h)
