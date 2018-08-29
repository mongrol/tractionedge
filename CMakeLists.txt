set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules") 
cmake_minimum_required (VERSION 2.8)
project(TRACTION)
file (GLOB TRACTION_SOURCES *.cpp)
Find_Package(SFML REQUIRED COMPONENTS System Graphics Window)

set(CMAKE_CXX_FLAGS "-ggdb -pg")
add_executable (tractionedge ${TRACTION_SOURCES})
target_link_libraries(tractionedge ${SFML_LIBRARY})

