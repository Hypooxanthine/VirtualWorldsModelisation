cmake_minimum_required (VERSION 3.8)

# ----- General compiler settings -----

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----- Solution definition -----

project ("MMV")

# ----- Subdirectories -----

add_subdirectory ("Vroom")
add_subdirectory("TP")

# ----- Testing -----

include(CTest)