cmake_minimum_required(VERSION 3.16)
project(Bomberman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

include_directories(
  ${SDL3_INCLUDE_DIRS}
  ${SDL3_TTF_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/src
)

add_definitions(-DSDL_MAIN_USE_CALLBACKS)

file(GLOB SRC
  src/*.cpp
  src/*.h
)

add_executable(bomberman ${SRC})

target_link_libraries(bomberman 
  PRIVATE 
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf 
)
