cmake_minimum_required(VERSION 3.10)
project(Undetected)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/App.cpp
    src/ConfigClass.cpp
    src/threadManager.cpp
    src/chrome.cpp
    src/siteScraper.cpp
    src/WebServer.cpp
    src/ConcurrentHashMap.cpp
    src/Logger.cpp
    src/util.cpp
)

# Define the executable
add_executable(Undetected ${SOURCES})

# Link static libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(Undetected
    ${CMAKE_SOURCE_DIR}/lib/libcurl.a
    ${CMAKE_SOURCE_DIR}/lib/libdpp.a
    ${CMAKE_SOURCE_DIR}/lib/libz.a
    ${CMAKE_SOURCE_DIR}/lib/libpugixml.a
    ${CMAKE_SOURCE_DIR}/lib/liblexbor_static.a
    ${CMAKE_SOURCE_DIR}/lib/libmongoose.a
    -static
    -pthread
    -static-libstdc++
    -static-libgcc
)

# Compiler options
if(MSVC)
    target_compile_options(Undetected PRIVATE /W4)
else()
    target_compile_options(Undetected PRIVATE -Wno-pedantic-ms-format)
endif()

# Installation rules
install(TARGETS Undetected DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY lib/ DESTINATION lib)
