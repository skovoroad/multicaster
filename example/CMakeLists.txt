cmake_minimum_required(VERSION 3.0)

project(example_multicaster)

file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_RELEASE_LIBS ON)
find_package(Boost 1.68.0 REQUIRED COMPONENTS program_options system)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES} 
  multicaster
)

