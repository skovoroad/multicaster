cmake_minimum_required(VERSION 3.0)
project(multicaster)

if(WIN32)
  set(FLAG_SUFF ${CMAKE_BUILD_TYPE})
  string(TOUPPER ${FLAG_SUFF} FLAG_SUFF)
  set(CMAKE_C_FLAGS_${FLAG_SUFF} "${CMAKE_C_FLAGS_${FLAG_SUFF}} /MD /MP")
  set(CMAKE_CXX_FLAGS_${FLAG_SUFF} "${CMAKE_CXX_FLAGS_${FLAG_SUFF}} /MD /MP")
  add_definitions(-DBOOST_ALL_NO_LIB)
endif()

set(CMAKE_CXX_STANDARD 17)
 
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_RELEASE_LIBS ON)
find_package(Boost 1.68.0 REQUIRED COMPONENTS date_time regex thread)

add_subdirectory(src)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
  set(INSTALL_GTEST OFF CACHE BOOL "Do not install gtest" FORCE)
  set(INSTALL_GMOCK OFF CACHE BOOL "Do not install gmock" FORCE)

  configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
  execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
            ${CMAKE_BINARY_DIR}/googletest-build)

  enable_testing()
  add_subdirectory(example)
  add_subdirectory(test)
endif()