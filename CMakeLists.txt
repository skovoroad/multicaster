cmake_minimum_required(VERSION 3.4)

project(multicaster)

file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

SET(BOOST_ROOT /home/skovoroad/projects/3rd/boost_1_68_0)
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.68.0 REQUIRED COMPONENTS date_time regex thread)
#FIND_PACKAGE(Boost 1.68.0 REQUIRED)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
    target_link_libraries(${PROJECT_NAME} ${USED_LIBS})
endif()

